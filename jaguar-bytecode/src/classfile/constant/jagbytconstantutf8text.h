/*
   File:    jagbytconstantutf8text.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Feb 1, 2012
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

#ifndef JAGBYTCONSTANTUTF8TEXT_H_
#define JAGBYTCONSTANTUTF8TEXT_H_

#define JAG_BYT_CONSTANT_UTF_TEXT_ID  1

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_CONSTANT_UTF8_TEXT              (jag_byt_constant_utf8_text_get_type())
#define JAG_BYT_CONSTANT_UTF8_TEXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_constant_utf8_text_get_type(), JagBytConstantUtf8Text))
#define JAG_BYT_CONSTANT_UTF8_TEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_CONSTANT_UTF8_TEXT, JagBytConstantUtf8TextClass))
#define JAG_BYT_IS_CONSTANT_UTF8_TEXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_CONSTANT_UTF8_TEXT))
#define JAG_BYT_IS_CONSTANT_UTF8_TEXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_CONSTANT_UTF8_TEXT))
#define JAG_BYT_CONSTANT_UTF8_TEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_CONSTANT_UTF8_TEXT, JagBytConstantUtf8TextClass))

typedef struct _JagBytConstantUtf8Text               JagBytConstantUtf8Text;
typedef struct _JagBytConstantUtf8TextPrivate        JagBytConstantUtf8TextPrivate;
typedef struct _JagBytConstantUtf8TextClass          JagBytConstantUtf8TextClass;


struct _JagBytConstantUtf8Text {
	GObject parent;
};

struct _JagBytConstantUtf8TextClass {
	GObjectClass parent_class;
};


GType jag_byt_constant_utf8_text_get_type();

JagBytConstantUtf8Text *jag_byt_constant_utf8_text_new(CatStringWo *a_text);

CatStringWo *jag_byt_constant_utf8_text_get_value(JagBytConstantUtf8Text *constant_ut8);

G_END_DECLS

#endif /* JAGBYTCONSTANTUTF8TEXT_H_ */
