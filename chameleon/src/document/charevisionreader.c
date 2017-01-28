/*
   File:    charevisionreader.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Mar 24, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "charevisionreader.h"
#include "chalinewo.h"
#include "chapagewo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaRevisionReader"
#include <logging/catlog.h>

struct _ChaRevisionReaderPrivate {
	ChaRevisionWo *a_revision;
	int cur_page_index, end_page_index;
	int cur_page_line_index, end_page_line_index;
	gboolean has_end;

	int cur_char_index;
	ChaPageWo *a_cur_page;
	ChaLineWo *a_cur_line;
	ChaLineEnd forced_line_end;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_scanner_iface_init(CatIUtf8ScannerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaRevisionReader, cha_revision_reader, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaRevisionReader)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_IUTF8_SCANNER, l_scanner_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_revision_reader_class_init(ChaRevisionReaderClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_revision_reader_init(ChaRevisionReader *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaRevisionReader *instance = CHA_REVISION_READER(object);
	ChaRevisionReaderPrivate *priv = cha_revision_reader_get_instance_private(instance);
	cat_unref_ptr(priv->a_revision);
	cat_unref_ptr(priv->a_cur_line);
	G_OBJECT_CLASS(cha_revision_reader_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_revision_reader_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaRevisionReader *cha_revision_reader_new(ChaRevisionWo *a_revision, ChaLineLocationWo *start, ChaLineLocationWo *end) {
	ChaRevisionReader *result = g_object_new(CHA_TYPE_REVISION_READER, NULL);
	cat_ref_anounce(result);
	ChaRevisionReaderPrivate *priv = cha_revision_reader_get_instance_private(result);
	priv->a_revision = cat_ref_ptr(a_revision);
	if (start) {
		priv->cur_page_index = cha_line_location_wo_get_page_index(start);
		priv->cur_page_line_index = cha_line_location_wo_get_page_line_index(start);
	} else {
		priv->cur_page_index = 0;
		priv->cur_page_line_index = 0;
	}
	priv->has_end = end!=NULL;
	if (end) {
		priv->end_page_index = cha_line_location_wo_get_page_index(end);
		priv->end_page_line_index = cha_line_location_wo_get_page_line_index(end);
	} else {
		priv->end_page_index = cha_revision_wo_page_count(a_revision)-1;
		if (priv->end_page_index>=0) {
			ChaPageWo *lp = cha_revision_wo_page_at(a_revision, priv->end_page_index);
			priv->end_page_line_index = cha_page_wo_line_count(lp);
		} else {
			priv->end_page_index = 0;
			priv->end_page_line_index = 0;
		}
	}


	int page_cnt = cha_revision_wo_page_count(a_revision);
	if (priv->cur_page_index<page_cnt) {
		priv->a_cur_page = cha_revision_wo_page_at(priv->a_revision, priv->cur_page_index);
		int line_cnt = cha_page_wo_line_count(priv->a_cur_page);
		if (priv->cur_page_line_index<line_cnt) {
			priv->a_cur_line = cha_page_wo_line_at(priv->a_cur_page, priv->cur_page_line_index);
		} else {
			priv->a_cur_line = NULL;
		}
	} else {
		priv->a_cur_page = NULL;
		priv->a_cur_line = NULL;
	}
	priv->cur_char_index = 0;
	return result;
}

void cha_revision_reader_set_forced_line_end(ChaRevisionReader *reader, ChaLineEnd forced_line_end) {
	ChaRevisionReaderPrivate *priv = cha_revision_reader_get_instance_private(reader);
	priv->forced_line_end = forced_line_end;

}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaRevisionReader *instance = CHA_REVISION_READER(self);
	ChaRevisionReaderPrivate *priv = cha_revision_reader_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p pgidx=%d, pglnidx=%d, chidx=%d]", iname, self, priv->cur_page_index, priv->cur_page_line_index, priv->cur_char_index);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

/********************* start CatIUtf8Scanner implementation *********************/

static gunichar l_scan_utf8_char(CatIUtf8Scanner *self, CatStreamStatus *status) {
	ChaRevisionReader *instance = CHA_REVISION_READER(self);
	ChaRevisionReaderPrivate *priv = cha_revision_reader_get_instance_private(instance);
	while(TRUE) {
		if (priv->a_cur_line) {
			CatStringWo *text = cha_line_wo_get_text(priv->a_cur_line);
			gunichar nch = cat_string_wo_unichar_at(text, &(priv->cur_char_index));
			if (nch==-1) {
				int nidx = priv->cur_char_index - cat_string_wo_length(text);
				cat_log_debug("nidx=%d", nidx);
				if (nidx>=0) {
					ChaLineEnd le;
					if (priv->forced_line_end!=CHA_LINE_END_NONE) {
						le = priv->forced_line_end;
					} else {
						le = cha_line_wo_get_line_end(priv->a_cur_line);
					}
					switch(le) {
						case CHA_LINE_END_CR :
							if (nidx==0) {
								priv->cur_char_index++;
								return (gunichar) 13;
							}
							break;
						case CHA_LINE_END_LF :
							if (nidx==0) {
								priv->cur_char_index++;
								return (gunichar) 10;
							}
							break;
						case CHA_LINE_END_CRLF :
							if (nidx==0) {
								priv->cur_char_index++;
								return (gunichar) 13;
							} else if (nidx==1) {
								priv->cur_char_index++;
								return (gunichar) 10;
							}
							break;
						case CHA_LINE_END_LFCR :
							if (nidx==0) {
								priv->cur_char_index++;
								return (gunichar) 10;
							} else if (nidx==1) {
								priv->cur_char_index++;
								return (gunichar) 13;
							}
							break;
						case CHA_LINE_END_NONE :
							break;
					}
					priv->cur_char_index = 0;
					priv->cur_page_line_index++;
					cat_unref_ptr(priv->a_cur_line);
					continue;
				}
			}
			return nch;
		} else if (priv->a_cur_page) {
			if ((priv->cur_page_line_index>=priv->end_page_line_index)
					&& (priv->cur_page_index==priv->end_page_index)) {
				*status = CAT_STREAM_CLOSED;
				return -1;
			}
			priv->a_cur_line = cha_page_wo_line_at(priv->a_cur_page, priv->cur_page_line_index);
			if (priv->a_cur_line==NULL) {
				priv->cur_page_index++;
				priv->a_cur_page = cha_revision_wo_page_at(priv->a_revision, priv->cur_page_index);
				priv->cur_page_line_index = 0;
			}
		} else {
			*status = CAT_STREAM_CLOSED;
			return -1;
		}
	}
	return -1;
}


static void l_scanner_iface_init(CatIUtf8ScannerInterface *iface) {
	iface->scan_utf8_char = l_scan_utf8_char;
}

/********************* end CatIUtf8Scanner implementation *********************/
