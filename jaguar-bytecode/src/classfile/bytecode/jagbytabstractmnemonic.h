/*
   File:    jagbytabstractmnemonic.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 2, 2012
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

#ifndef JAGBYTABSTRACTMNEMONIC_H_
#define JAGBYTABSTRACTMNEMONIC_H_

#include "jagbytoperation.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_ABSTRACT_MNEMONIC              (jag_byt_abstract_mnemonic_get_type())
#define JAG_BYT_ABSTRACT_MNEMONIC(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_abstract_mnemonic_get_type(), JagBytAbstractMnemonic))
#define JAG_BYT_ABSTRACT_MNEMONIC_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_ABSTRACT_MNEMONIC, JagBytAbstractMnemonicClass))
#define JAG_BYT_IS_ABSTRACT_MNEMONIC(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_ABSTRACT_MNEMONIC))
#define JAG_BYT_IS_ABSTRACT_MNEMONIC_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_ABSTRACT_MNEMONIC))
#define JAG_BYT_ABSTRACT_MNEMONIC_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_ABSTRACT_MNEMONIC, JagBytAbstractMnemonicClass))
#define JAG_BYT_ABSTRACT_MNEMONIC_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_ABSTRACT_MNEMONIC, JagBytAbstractMnemonicPrivate))

typedef struct _JagBytAbstractMnemonic               JagBytAbstractMnemonic;
typedef struct _JagBytAbstractMnemonicPrivate        JagBytAbstractMnemonicPrivate;
typedef struct _JagBytAbstractMnemonicClass          JagBytAbstractMnemonicClass;


struct _JagBytAbstractMnemonic {
	GObject parent;
	JagBytAbstractMnemonicPrivate *priv;
};

struct _JagBytAbstractMnemonicClass {
	GObjectClass parent_class;
};


GType jag_byt_abstract_mnemonic_get_type();

void jag_byt_abstract_mnemonic_construct(JagBytAbstractMnemonic *abstract_mnemonic, JagBytOperation operation, int offset, int length);

G_END_DECLS

#endif /* JAGBYTABSTRACTMNEMONIC_H_ */
