/*
   File:    jagjarreader.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jan 27, 2012
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


#ifndef JAGJARREADER_H_
#define JAGJARREADER_H_

#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define JAG_TYPE_JAR_READER              (jag_jar_reader_get_type())
#define JAG_JAR_READER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_jar_reader_get_type(), JagJarReader))
#define JAG_JAR_READER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_JAR_READER, JagJarReaderClass))
#define JAG_IS_JAR_READER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_JAR_READER))
#define JAG_IS_JAR_READER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_JAR_READER))
#define JAG_JAR_READER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_JAR_READER, JagJarReaderClass))
#define JAG_JAR_READER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_JAR_READER, JagJarReaderPrivate))

typedef struct _JagJarReader               JagJarReader;
typedef struct _JagJarReaderPrivate        JagJarReaderPrivate;
typedef struct _JagJarReaderClass          JagJarReaderClass;


struct _JagJarReader {
	GObject parent;
	JagJarReaderPrivate *priv;
};

struct _JagJarReaderClass {
	GObjectClass parent_class;
};



typedef struct _JagJarEndOfCentralDir   JagJarEndOfCentralDir;

struct _JagJarEndOfCentralDir {
	uint16_t nrOfThisDisc;
	uint16_t nrOfStartCdDisc;
	uint16_t nrOfCdsOnDisc;
	uint16_t nrOfCdsTotal;
	uint32_t sizeOfCentralDir;
	uint32_t centralDirStartOffset;
	CatStringWo *a_comment;
};



typedef struct _JagJarCentralDirHeader   JagJarCentralDirHeader;

struct _JagJarCentralDirHeader {
	uint16_t versionMadeBy;
	uint16_t versionNeeded;
	uint16_t generalPurposeFlags;
	uint16_t compressionMethod;
	uint16_t lastModTime;
	uint16_t lastModDate;
	uint32_t checksum;
	uint32_t compressedSize;
	uint32_t uncompressedSize;
	uint16_t discNrFileStart;
	uint16_t internalFileAttributes;
	uint32_t externalFileAttributes;
	uint32_t offsetLocalHeader;
	CatStringWo *a_fileName;
	CatStringWo *a_fileComment;
	uint16_t extraFieldLength;
	char *extraField;
};

GType jag_jar_reader_get_type();

JagJarReader *jag_jar_reader_new(VipPath *path);
JagJarReader *jag_jar_reader_new_from_stream(CatIInputStream *stream);


VipPath *jag_jar_reader_get_path(JagJarReader *jar_reader);

gboolean jag_jar_reader_init_directory(JagJarReader *jar_reader);


CatStringWo *jag_jar_reader_read_entry(JagJarReader *jar_reader, JagJarCentralDirHeader *entry_header, gboolean nowrap);


CatIIterator *jag_jar_reader_entry_iterator(JagJarReader *jar_reader);

JagJarCentralDirHeader *jag_jar_reader_find_entry_by_path(JagJarReader *jar_reader, CatStringWo *a_entry_path);


void jag_jar_reader_dump_dir_entry(JagJarCentralDirHeader *entry_header, gboolean as_local);

void jag_jar_reader_dump_local_entry(JagJarReader *jar_reader, JagJarCentralDirHeader *entry_header);


G_END_DECLS



#endif /* JAGJARREADER_H_ */
