/*
   File:    jagbytecodereader.h
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


#ifndef JAGBYTECODEREADER_H_
#define JAGBYTECODEREADER_H_

#include "classfile/jagbytclassfile.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_BYTECODE_READER              (jag_bytecode_reader_get_type())
#define JAG_BYTECODE_READER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_bytecode_reader_get_type(), JagBytecodeReader))
#define JAG_BYTECODE_READER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_BYTECODE_READER, JagBytecodeReaderClass))
#define JAG_IS_BYTECODE_READER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_BYTECODE_READER))
#define JAG_IS_BYTECODE_READER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_BYTECODE_READER))
#define JAG_BYTECODE_READER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_BYTECODE_READER, JagBytecodeReaderClass))
#define JAG_BYTECODE_READER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_BYTECODE_READER, JagBytecodeReaderPrivate))

typedef struct _JagBytecodeReader               JagBytecodeReader;
typedef struct _JagBytecodeReaderPrivate        JagBytecodeReaderPrivate;
typedef struct _JagBytecodeReaderClass          JagBytecodeReaderClass;


struct _JagBytecodeReader {
	GObject parent;
	JagBytecodeReaderPrivate *priv;
};

struct _JagBytecodeReaderClass {
	GObjectClass parent_class;
};


GType jag_bytecode_reader_get_type();

JagBytecodeReader *jag_bytecode_reader_new(CatIInputStream *bytecode_stream);

JagBytClassfile *jag_bytecode_reader_parse_bytecode(JagBytecodeReader *reader);

G_END_DECLS



#endif /* JAGBYTECODEREADER_H_ */
