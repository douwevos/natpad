/*
   File:    jagjarreader.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jan 25, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "jagjarreader.h"
#include <caterpillar.h>
#include <fcntl.h>
#include <unistd.h>
//#include "zlib-1.1.3/zlib.h"
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#include <zlib.h>

#ifdef G_OS_WIN32
#include <windows.h>
#endif


#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagJarReader"
#include <logging/catlog.h>


#define MAX_DIR_HEADER_COUNT           100000
#define MAX_UNCOMPRESSED_SIZE          0x200000
#define BUF_SIZE                       0x10000

struct _JagJarReaderPrivate {
	VipPath *path;
	CatIInputStream *stream;
	char *buffer;

	int central_dir_header_count;
	JagJarCentralDirHeader *central_dir_headers;

	JagJarEndOfCentralDir central_dir_end;


	int iter_offset;
	CatStringWo *e_uncompressed_data;
	int uncompressed_offset;
};


/********** CatIIterator **********/


#define JAG_TYPE_JAR_READER_ENTRY_ITER            (jag_jar_reader_entry_iter_get_type())
#define JAG_JAR_READER_ENTRY_ITER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_jar_reader_entry_iter_get_type(), JagJarReaderEntryIter))
#define JAG_JAR_READER_ENTRY_ITER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_JAR_READER_ENTRY_ITER, JagJarReaderEntryIterClass))
#define JAG_IS_JAR_READER_ENTRY_ITER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_JAR_READER_ENTRY_ITER))
#define JAG_IS_JAR_READER_ENTRY_ITER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_JAR_READER_ENTRY_ITER))
#define JAG_JAR_READER_ENTRY_ITER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_JAR_READER_ENTRY_ITER, JagJarReaderEntryIterClass))


typedef struct _JagJarReaderEntryIter	      JagJarReaderEntryIter;

typedef struct _JagJarReaderEntryIterClass	  JagJarReaderEntryIterClass;


struct _JagJarReaderEntryIter {
	GObject parent;
	JagJarReader *jar_reader;
	int offset;
};

struct _JagJarReaderEntryIterClass {
	GObjectClass parent_class;
};


GType jag_jar_reader_entry_iter_get_type(void);

static void l_iterator_interface_init(CatIIteratorInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagJarReaderEntryIter, jag_jar_reader_entry_iter, G_TYPE_OBJECT, { // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IITERATOR, l_iterator_interface_init);
});

static void l_dispose_iter(GObject *object);

static void jag_jar_reader_entry_iter_class_init(JagJarReaderEntryIterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose_iter;
}

static void jag_jar_reader_entry_iter_init(JagJarReaderEntryIter *array_iter) {
}

static void l_dispose_iter(GObject *object) {
	JagJarReaderEntryIter *instance = JAG_JAR_READER_ENTRY_ITER(object);
	cat_unref_ptr(instance->jar_reader);
}

/********** CatIIterator **********/



static void l_input_stream_iface_init(CatIInputStreamInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagJarReader, jag_jar_reader, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagJarReader)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IINPUT_STREAM, l_input_stream_iface_init)
})

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_free_header(JagJarCentralDirHeader *local_header);

static void jag_jar_reader_class_init(JagJarReaderClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_jar_reader_init(JagJarReader *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagJarReader *instance = JAG_JAR_READER(object);
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(instance);
	cat_unref_ptr(priv->stream);

	cat_unref_ptr(priv->path);
	cat_free_ptr(priv->buffer);
	cat_unref_ptr(priv->e_uncompressed_data);


	if (priv->central_dir_header_count>0 && priv->central_dir_headers) {
		int idx;
		for(idx=0; idx<priv->central_dir_header_count; idx++) {
			l_free_header(priv->central_dir_headers+idx);
		}
	}
	priv->central_dir_header_count = 0;
	cat_free_ptr(priv->central_dir_headers);

	G_OBJECT_CLASS(jag_jar_reader_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_jar_reader_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagJarReader *jag_jar_reader_new(VipPath *path) {
	JagJarReader *result = g_object_new(JAG_TYPE_JAR_READER, NULL);
	cat_ref_anounce(result);
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(result);
	priv->path = cat_ref_sink_ptr(path);
	priv->buffer = g_malloc0(BUF_SIZE+5);

	CatStringWo *a_fname = vip_path_to_string(path);
	priv->stream = (CatIInputStream *) cat_file_input_stream_new(a_fname);
	priv->central_dir_header_count = 0;
	priv->central_dir_headers = NULL;
	priv->e_uncompressed_data = NULL;
	priv->uncompressed_offset = -1;
	return result;
}

JagJarReader *jag_jar_reader_new_from_stream(CatIInputStream *stream) {
	JagJarReader *result = g_object_new(JAG_TYPE_JAR_READER, NULL);
	cat_ref_anounce(result);
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(result);
	priv->path = NULL;
	priv->buffer = g_malloc0(BUF_SIZE+5);

	priv->stream = cat_ref_ptr(stream);
	priv->central_dir_header_count = 0;
	priv->central_dir_headers = NULL;
	priv->e_uncompressed_data = NULL;
	priv->uncompressed_offset = -1;
	return result;
}

VipPath *jag_jar_reader_get_path(JagJarReader *jar_reader) {
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(jar_reader);
	return priv->path;
}


#define READ_SHORT2(off) ((uint16_t) ((((uint16_t) buffer[off] & 0xFF)+((((uint16_t) buffer[off+1])<<8) & 0xFF00)))
#define READ_SHORT(off) ((((uint16_t) buffer[off]) & 0xFF)+((((uint16_t) buffer[off+1])<<8) & 0xFF00))

#define READ_INT(off) ((((uint32_t) buffer[off]) & 0xFF)  +  ((((uint32_t) buffer[off+1])<<8) & 0xFF00)  +  ((((uint32_t) buffer[off+2])<<16) & 0xFF0000)  +  ((((uint32_t) buffer[off+3])<<24) & 0xFF000000))


static void l_free_header(JagJarCentralDirHeader *local_header) {
	cat_unref_ptr(local_header->a_fileComment);
	cat_unref_ptr(local_header->a_fileName);
//	cat_free_ptr(local_header->extraField);
}


static gboolean l_read_central_directory_end(JagJarReader *jar_reader, int buffer_offset) {
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(jar_reader);
	char *buffer = priv->buffer+buffer_offset;
	priv->central_dir_end.nrOfThisDisc = READ_SHORT(4);
	priv->central_dir_end.nrOfStartCdDisc = READ_SHORT(6);
	priv->central_dir_end.nrOfCdsOnDisc = READ_SHORT(8);
	priv->central_dir_end.nrOfCdsTotal = READ_SHORT(10);
	priv->central_dir_end.sizeOfCentralDir = READ_INT(12);
	priv->central_dir_end.centralDirStartOffset = READ_INT(16);

	cat_log_debug("sizeOfCentralDir=%d, centralDirStartOffset=%d, nrOfCdsTotal=%d", priv->central_dir_end.sizeOfCentralDir, priv->central_dir_end.centralDirStartOffset, priv->central_dir_end.nrOfCdsTotal);
//	cat_string_wo_new_with_len();

	return TRUE;
}

static gboolean l_read_all_bytes(CatIInputStream *stream, char *buffer, int count) {
	gboolean result = TRUE;
	int total_read = 0;
	int left = count;
	cat_log_detail("count=%d", count);
	long long seek_start = cat_iinput_stream_seek(stream, CAT_SEEK_FORWARD, 0);

	while(left>0) {
		int real_read_count = cat_iinput_stream_read_length(stream, buffer, left);
		cat_log_trace("left=%d, real_read_count=%d", left, real_read_count);
		if (real_read_count<=0) {
			result = FALSE;
			break;
		}
		total_read += real_read_count;
		if (real_read_count==left) {
			break;
		}
		buffer+=real_read_count;
		left-=real_read_count;
	}
	long long seek_end = cat_iinput_stream_seek(stream, CAT_SEEK_FORWARD, 0);

	int realy_read = (int) seek_end-seek_start;
	cat_log_trace("realy_read=%d, total_read=%d, left=%d", realy_read, total_read, left);
	if (realy_read==count) {
		result = TRUE;
	}
	if (!result) {
		cat_log_error("count=%d, total_read=%d, left=%d", count, total_read, left);
		// TODO cat_iinput_stream should have its own error handling
		switch(errno) {
			case EAGAIN : { cat_log_error("EAGAIN: The O_NONBLOCK flag is set for the file descriptor and the thread would be delayed."); } break;
			case EBADF : { cat_log_error("EBADF: The O_NONBLOCK flag is set for the file descriptor and the thread would be delayed."); } break;
//			case EBADMSG : { cat_log_error("EBADMSG: The file is a STREAM file that is set to control-normal mode and the message waiting to be read includes a control part."); } break;
			case EINTR : { cat_log_error("EINTR: The read operation was terminated due to the receipt of a signal, and no data was transferred."); } break;
			case EINVAL : { cat_log_error("EINVAL: The offset argument is invalid. The value is negative."); } break;
			case EIO : { cat_log_error("EIO: A physical I/O error has occurred."); } break;
			case EISDIR : { cat_log_error("EISDIR: The fildes argument refers to a directory and the implementation does not allow the directory to be read using read() or pread()."); } break;
//			case EOVERFLOW : { cat_log_error("EOVERFLOW: "); } break;
			case ENXIO : { cat_log_error("ENXIO: A request was outside the capabilities of the device."); } break;
			default : { cat_log_error("???: %d", errno); } break;
		}
	}
	return result;
}

static gboolean l_read_central_directory_headers(JagJarReader *jar_reader) {
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(jar_reader);

	cat_iinput_stream_seek(priv->stream, CAT_SEEK_SET, priv->central_dir_end.centralDirStartOffset);

	JagJarCentralDirHeader *header = priv->central_dir_headers;
	int header_idx = priv->central_dir_header_count;
	char *buffer =priv->buffer;
	while(header_idx-->0) {
		if (!l_read_all_bytes(priv->stream, priv->buffer, 46)) {
			cat_log_error("could not read 46 bytes");
		}
		header->versionMadeBy = READ_SHORT(4);
		header->versionNeeded = READ_SHORT(6);
		header->generalPurposeFlags = READ_SHORT(8);
		header->compressionMethod = READ_SHORT(10);
		header->lastModTime = READ_SHORT(12);
		header->lastModDate = READ_SHORT(14);
		header->checksum = READ_INT(16);
		header->compressedSize = READ_INT(20);
		header->uncompressedSize = READ_INT(24);
		header->extraFieldLength = READ_SHORT(30);
		header->discNrFileStart = READ_SHORT(34);
		header->internalFileAttributes = READ_SHORT(36);
		header->externalFileAttributes = READ_INT(38);
		header->offsetLocalHeader = READ_INT(42);

		int fileNameLength = (int) READ_SHORT(28);
		int fileCommentLength = (int) READ_SHORT(32);
		int extraFieldLength = (int) header->extraFieldLength;

		int addLength = fileNameLength + fileCommentLength + extraFieldLength;
		if (addLength<=BUF_SIZE) {
			if (!l_read_all_bytes(priv->stream, priv->buffer, addLength)) {
				cat_log_error("could not read %d bytes", addLength);
				return FALSE;
			}
			header->a_fileName = cat_string_wo_anchor(cat_string_wo_new_with_len(priv->buffer, fileNameLength), 0);

			if (fileCommentLength>0) {
				header->a_fileComment = cat_string_wo_anchor(cat_string_wo_new_with_len(priv->buffer+ fileNameLength + extraFieldLength, fileCommentLength), 0);
			} else {
				header->a_fileComment = NULL;
			}
		} else {
			if (!l_read_all_bytes(priv->stream, priv->buffer, fileNameLength)) {
				cat_log_error("could not read %d bytes", fileNameLength);
				return FALSE;
			}
			header->a_fileName = cat_string_wo_anchor(cat_string_wo_new_with_len(priv->buffer, fileNameLength), 0);
			if (extraFieldLength>0) {
				if (!l_read_all_bytes(priv->stream, priv->buffer, extraFieldLength)) {
					cat_log_error("could not read %d bytes", extraFieldLength);
					return FALSE;
				}
			}
			if (fileCommentLength>0) {
				if (!l_read_all_bytes(priv->stream, priv->buffer, fileCommentLength)) {
					cat_log_error("could not read %d bytes", addLength);
					return FALSE;
				}
				header->a_fileComment = cat_string_wo_anchor(cat_string_wo_new_with_len(priv->buffer, fileCommentLength), 0);
			} else {
				header->a_fileComment = NULL;
			}
		}
		cat_log_debug("filename[%d]=%s", header_idx, cat_string_wo_getchars(header->a_fileName));
		header++;
	}


//	char *tess = jag_jar_read_entry(jar_reader, fd, priv->central_dir_headers, TRUE);

	return TRUE;
}

gboolean jag_jar_reader_init_directory(JagJarReader *jar_reader) {
	gboolean result = FALSE;
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(jar_reader);
	if (priv->stream != NULL) {
		long long length = cat_iinput_stream_seek(priv->stream, CAT_SEEK_SET_FROM_END, 0);
		cat_log_debug("length=%ld", length);
		if (length>0) {
			long long ftry = length-4096;
			if (ftry<0) {
				ftry = 0;
			}

//			long long seeked_at =
					cat_iinput_stream_seek(priv->stream, CAT_SEEK_SET, ftry);
//			cat_log_debug("seeking to=%ld, seeked_at=%ld", ftry, seeked_at);

			int read_count = (int) (length-ftry);

			if (!l_read_all_bytes(priv->stream, priv->buffer, read_count)) {
				cat_log_error("could not read %d bytes", read_count);
				return FALSE;
			}

//			cat_dump_hex(priv->buffer, read_count);

			int pointer = read_count-22;
			while(pointer>=0) {
				if (priv->buffer[pointer]==0x50) {

					if ((priv->buffer[pointer+1]==0x4b) &&
							(priv->buffer[pointer+2]==0x05) &&
							(priv->buffer[pointer+3]==0x06)) {

						int comment_length = priv->buffer[pointer+20] + (priv->buffer[pointer+21]<<8);
						cat_log_debug("comment_length=%d, read_count=%d, length=%d", comment_length, read_count, length);
						comment_length = length-(read_count-pointer)+22+comment_length;

						if (comment_length==length) {
							result = TRUE;
							break;
						}
					}
				}
				pointer--;
			}
			// TODO make repeat loop for zip files with large comments
			if (result) {
				if (!l_read_central_directory_end(jar_reader, pointer)) {
					return FALSE;
				}

				priv->central_dir_header_count = priv->central_dir_end.nrOfCdsTotal;
				if (priv->central_dir_header_count<0 || priv->central_dir_header_count>MAX_DIR_HEADER_COUNT) {
					cat_log_error("priv->central_dir_header_count exceeds limit: val=%d", priv->central_dir_header_count);
					return FALSE;
				} else {
					priv->central_dir_headers = g_malloc(sizeof(JagJarCentralDirHeader)*priv->central_dir_header_count);
					if (!l_read_central_directory_headers(jar_reader)) {
						return FALSE;
					}
				}
			}
		}

	}
	cat_log_debug("result=%d", result);
	return result;
}



static gboolean l_read_local_header(JagJarReader *jar_reader, JagJarCentralDirHeader *local_header) {
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(jar_reader);
	char *buffer = priv->buffer;
	if (!l_read_all_bytes(priv->stream, priv->buffer, 30)) {
		cat_log_error("was not able to read 30 bytes");
		return FALSE;
	}

	local_header->versionNeeded = READ_SHORT(4);
	local_header->generalPurposeFlags = READ_SHORT(6);
	local_header->compressionMethod = READ_SHORT(8);
	local_header->lastModTime = READ_SHORT(10);
	local_header->lastModDate = READ_SHORT(12);
	local_header->checksum = READ_INT(14);
	local_header->compressedSize = READ_INT(18);
	local_header->uncompressedSize = READ_INT(22);

	local_header->extraFieldLength = READ_SHORT(28);

	int fileNameLength = (int) READ_SHORT(26);

	int extraFieldLength = (int) local_header->extraFieldLength;

	int addLength = fileNameLength + extraFieldLength;
	if (addLength<=BUF_SIZE) {
		if (!l_read_all_bytes(priv->stream, priv->buffer, addLength)) {
			cat_log_error("was not able to read %d bytes", addLength);
			return FALSE;
		}

		local_header->a_fileName = cat_string_wo_new_with_len(priv->buffer, fileNameLength);
	} else {
		if (!l_read_all_bytes(priv->stream, priv->buffer, fileNameLength)) {
			cat_log_error("was not able to read %d bytes", fileNameLength);
			return FALSE;
		}
		local_header->a_fileName = cat_string_wo_new_with_len(priv->buffer, fileNameLength);
		if (extraFieldLength>0) {
			if (!l_read_all_bytes(priv->stream, priv->buffer, extraFieldLength)) {
				cat_log_error("was not able to read %d bytes", extraFieldLength);
				return FALSE;
			}
		}
	}
	local_header->a_fileName = cat_string_wo_anchor(local_header->a_fileName, 0);
	cat_log_debug("loc-fname=%s", cat_string_wo_getchars(local_header->a_fileName));
	return TRUE;
}

CatStringWo *jag_jar_reader_read_entry(JagJarReader *jar_reader, JagJarCentralDirHeader *entry_header, gboolean nowrap) {
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(jar_reader);
	gboolean success = TRUE;
	z_stream *strm = calloc(1, sizeof(z_stream));
	int initResult = inflateInit2(strm, nowrap ? -MAX_WBITS : MAX_WBITS);
	cat_log_debug("initResult:%d", initResult);
	if (initResult != Z_OK) {
		free(strm);
		return NULL;
	}

	JagJarCentralDirHeader local_header;
	local_header.extraField = NULL;
	local_header.a_fileComment = NULL;
	local_header.a_fileName = NULL;

//	long long seeked_at =
			cat_iinput_stream_seek(priv->stream, CAT_SEEK_SET, entry_header->offsetLocalHeader);

	if (!l_read_local_header(jar_reader, &local_header)) {
		free(strm);
		return NULL;
	}
	cat_log_debug("local::compressedSize=%d, uncompressedSize=%d", local_header.compressedSize, local_header.uncompressedSize);
	cat_log_debug("indir::compressedSize=%d, uncompressedSize=%d", entry_header->compressedSize, entry_header->uncompressedSize);

	cat_log_debug("local:name=%s", cat_string_wo_getchars(local_header.a_fileName));

	char *output = NULL;

	if (entry_header->uncompressedSize>MAX_UNCOMPRESSED_SIZE) {
		cat_log_error("uncompressed size exceeds limit: uncompressedSize=%d", entry_header->uncompressedSize);
	} else {
		output = g_malloc(entry_header->uncompressedSize+5);

		char *buffer = priv->buffer;


		int output_left = entry_header->uncompressedSize;
		int input_left = entry_header->compressedSize;
		if (output_left==input_left) {	// TODO the official code uses getMethod(jzEntry) .. this tests whether the csize is 0. Use the official method and test if the the csize is read from the local header or from the header in the central dir
			// STORED
			if (!l_read_all_bytes(priv->stream, output, input_left)) {
				cat_log_error("could not read %d bytes", input_left);
				success = FALSE;
			}
		} else {
			// DEFLATED
	//		int output_offset = 0;
			gboolean keep_reading = TRUE;
			strm->next_out = (Bytef *) output;
			strm->avail_out = entry_header->uncompressedSize+5;
			int null_read_max_loop = 5;
			while(keep_reading) {
				int bytes_to_read = input_left > BUF_SIZE ? BUF_SIZE : input_left;

				int bytes_read = cat_iinput_stream_read_length(priv->stream, buffer, bytes_to_read);
				if (bytes_read <= 0) {
					if ((null_read_max_loop--)==0) {
						keep_reading = FALSE;
						cat_log_error("kept reading nothing");
						success = FALSE;
						break;
					}
				}

				strm->next_in  = (Bytef *) buffer;
				if ((input_left-bytes_read) <=0) {
					buffer[bytes_read] = 0;
					buffer[bytes_read+1] = 0;
					buffer[bytes_read+2] = 0;
					buffer[bytes_read+3] = 0;
					buffer[bytes_read+4] = 0;
					strm->avail_in  = bytes_read+5;
				} else {
					strm->avail_in  = bytes_read;
				}


				cat_log_debug("before inflate: next-in=%p, avail-in=%d, next-out=%p, avail-out=%d", strm->next_in, strm->avail_in, strm->next_out, strm->avail_out);
				int ret = inflate(strm, Z_FULL_FLUSH);
				if (ret==Z_STREAM_END) {
					keep_reading = FALSE;
				} else if (ret!=Z_OK) {
					keep_reading = FALSE;
					cat_log_error("inflate returned: ret=%d", ret);
					success = FALSE;
				}
				cat_log_debug("after inflate: next-in=%p, avail-in=%d, next-out=%p, avail-out=%d", strm->next_in, strm->avail_in, strm->next_out, strm->avail_out);
				input_left -= bytes_read;
			}
		}
	}
//	cat_log_debug("inflate:res=%d", ret);

//	int fh = open("/home/superman/testinflater.class", O_CREAT|O_WRONLY|O_TRUNC, DEFFILEMODE);
//	cat_log_debug("fh=%d, error:%d", fh, errno);
//	int count = write(fh, result, entry_header->uncompressedSize);
//	cat_log_debug("wrote:%d bytes", count);
//	cat_log_debug("error:%d", errno);
//
//	close(fh);

//	ZIP_GetEntry();

	l_free_header(&local_header);
	inflateEnd(strm);
	free(strm);
	if (success) {
		CatStringWo *e_res = cat_string_wo_new_nocopy(output, entry_header->uncompressedSize);
		return e_res;
	} else {
		cat_free_ptr(output);
	}
	return NULL;
}

CatIIterator *jag_jar_reader_entry_iterator(JagJarReader *jar_reader) {
	JagJarReaderEntryIter *result = g_object_new(JAG_TYPE_JAR_READER_ENTRY_ITER, NULL);
	result->jar_reader  = cat_ref_ptr(jar_reader);
	result->offset = 0;
	return (CatIIterator *) result;
}

JagJarCentralDirHeader *jag_jar_reader_find_entry_by_path(JagJarReader *jar_reader, CatStringWo *a_entry_path) {
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(jar_reader);
	int offset = 0;
	for(offset=0; offset<priv->central_dir_header_count; offset++) {
		JagJarCentralDirHeader *entry = priv->central_dir_headers+offset;
		if (cat_string_wo_equal(entry->a_fileName, a_entry_path)) {
			return entry;
		}
	}
	return NULL;
}


void jag_jar_reader_dump_dir_entry(JagJarCentralDirHeader *entry_header, gboolean as_local) {
	cat_log_print("DUMP", "-------------------------------- %p", entry_header);
	if (!as_local) {
		cat_log_print("DUMP", "versionMadeBy:          %d", entry_header->versionMadeBy);
	}
	cat_log_print("DUMP", "versionNeeded:          %d", entry_header->versionNeeded);
	cat_log_print("DUMP", "generalPurposeFlags:    %d", entry_header->generalPurposeFlags);
	cat_log_print("DUMP", "compressionMethod:      %d", entry_header->compressionMethod);
	cat_log_print("DUMP", "lastModTime:            %d", entry_header->lastModTime);
	cat_log_print("DUMP", "lastModDate:            %d", entry_header->lastModDate);
	cat_log_print("DUMP", "checksum:               %d", entry_header->checksum);
	cat_log_print("DUMP", "compressedSize:         %d", entry_header->compressedSize);
	cat_log_print("DUMP", "uncompressedSize:       %d", entry_header->uncompressedSize);
	if (!as_local) {
		cat_log_print("DUMP", "discNrFileStart:        %d", entry_header->discNrFileStart);
		cat_log_print("DUMP", "internalFileAttributes: %d", entry_header->internalFileAttributes);
		cat_log_print("DUMP", "externalFileAttributes: %d", entry_header->externalFileAttributes);
		cat_log_print("DUMP", "offsetLocalHeader:      %d", entry_header->offsetLocalHeader);
	}
	cat_log_print("DUMP", "fileName:               %s", (entry_header->a_fileName==NULL ? "<NULL>" : cat_string_wo_getchars(entry_header->a_fileName)));
	if (!as_local) {
		cat_log_print("DUMP", "fileComment:            %s", (entry_header->a_fileComment==NULL ? "<NULL>" : cat_string_wo_getchars(entry_header->a_fileComment)));
	}
//	cat_log_print("DUMP", "extraFieldLength:       %s", (entry_header->extraField==NULL ? "<NULL>" : entry_header->extraField));
}

void jag_jar_reader_dump_local_entry(JagJarReader *jar_reader, JagJarCentralDirHeader *entry_header) {
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(jar_reader);

//	long long length =
			cat_iinput_stream_seek(priv->stream, CAT_SEEK_SET, entry_header->offsetLocalHeader);

	JagJarCentralDirHeader local_header;
	if (!l_read_local_header(jar_reader, &local_header)) {
		cat_log_error("could not read local header");
		return;
	}
	jag_jar_reader_dump_dir_entry(&local_header, TRUE);
}


static gboolean l_ensure_has_uncompressed(JagJarReader *reader) {
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(reader);
	cat_log_debug("priv->uncompressed_offset=%d", priv->uncompressed_offset);
	if (priv->uncompressed_offset==-1) {
		priv->uncompressed_offset = 0;
		JagJarCentralDirHeader *header = (JagJarCentralDirHeader *) priv->central_dir_headers+priv->iter_offset;
		cat_log_debug("Extracting:%o, iter_offset=%d", header->a_fileName, priv->iter_offset);
		priv->e_uncompressed_data = jag_jar_reader_read_entry(reader, header, TRUE);
	}
	return priv->e_uncompressed_data!=NULL;
}

static int l_iface_read(CatIInputStream *self) {
	JagJarReader *reader = JAG_JAR_READER(self);
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(reader);
	int result = -1;
	cat_log_debug("reader=%p, read", reader);
	if (l_ensure_has_uncompressed(reader)) {
		if (priv->uncompressed_offset<cat_string_wo_length(priv->e_uncompressed_data)) {
			result = 0xFF & (int) cat_string_wo_char_at(priv->e_uncompressed_data, priv->uncompressed_offset++);
		}
	}
	return result;
}

static int l_iface_read_length(CatIInputStream *self, char *data, int length) {
	JagJarReader *reader = JAG_JAR_READER(self);
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(reader);
	cat_log_debug("reader=%p, data=%p, length=%d", reader, data, length);
	int result = -1;
	if (l_ensure_has_uncompressed(reader)) {
		if (priv->uncompressed_offset<cat_string_wo_length(priv->e_uncompressed_data)) {


			if (priv->uncompressed_offset+length>cat_string_wo_length(priv->e_uncompressed_data)) {
				length = cat_string_wo_length(priv->e_uncompressed_data) - priv->uncompressed_offset;
			}
			if (length>0) {
				const char *source_data = cat_string_wo_getchars(priv->e_uncompressed_data);
				memcpy(data, source_data+priv->uncompressed_offset, length);
			}
			priv->uncompressed_offset += length;
			result = length;
		}
	}
	return result;
}

static long long l_seek(CatIInputStream *self, CatSeekType seek_type, long long seek_offset) {
	JagJarReader *reader = JAG_JAR_READER(self);
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(reader);
	long long result = -1;
	if (l_ensure_has_uncompressed(reader)) {
		int new_offset = priv->uncompressed_offset;
		switch(seek_type) {
			case CAT_SEEK_FORWARD : {
				new_offset += (int) seek_offset;
			} break;
			case CAT_SEEK_SET : {
				new_offset = (int) seek_offset;
			} break;
			case CAT_SEEK_SET_FROM_END : {
				new_offset = cat_string_wo_length(priv->e_uncompressed_data) - seek_offset;
			} break;
		}
		if (new_offset<0) {
			new_offset = 0;
		} else if (new_offset>cat_string_wo_length(priv->e_uncompressed_data)) {
			new_offset = cat_string_wo_length(priv->e_uncompressed_data);
		}
		result = (int) new_offset;
		priv->uncompressed_offset = new_offset;
		cat_log_debug("reader=%p, seek_type=%d, seek_offset=%ld, result=%d, data_length=%d", reader, seek_type, seek_offset, result, cat_string_wo_length(priv->e_uncompressed_data));
	}
	return result;
}

static void l_input_stream_iface_init(CatIInputStreamInterface *iface) {
	iface->read = l_iface_read;
	iface->readLength = l_iface_read_length;
	iface->seek = l_seek;

}



static gboolean l_iter_has_next(CatIIterator *self) {
	JagJarReaderEntryIter *iter = JAG_JAR_READER_ENTRY_ITER(self);
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(iter->jar_reader);
	return iter->offset<priv->central_dir_header_count;
}

static gboolean l_iter_is_last(CatIIterator *self) {
	JagJarReaderEntryIter *iter = JAG_JAR_READER_ENTRY_ITER(self);
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(iter->jar_reader);
	return iter->offset+1==priv->central_dir_header_count;
}

static gpointer l_iter_next(CatIIterator *self) {
	JagJarReaderEntryIter *iter = JAG_JAR_READER_ENTRY_ITER(self);
	JagJarReaderPrivate *priv = jag_jar_reader_get_instance_private(iter->jar_reader);
	priv->iter_offset = iter->offset;
	cat_unref_ptr(priv->e_uncompressed_data);
	priv->uncompressed_offset = -1;
	JagJarCentralDirHeader *header = (JagJarCentralDirHeader *) priv->central_dir_headers+iter->offset++;
	return header;
}


static void l_iterator_interface_init(CatIIteratorInterface *iface) {
	iface->has_next = l_iter_has_next;
	iface->next = l_iter_next;
	iface->is_last = l_iter_is_last;
}
