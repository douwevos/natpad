/*
   File:    mulephpeditorfactory.h
   Project: mule
   Author:  Douwe Vos
   Date:    Dec 11, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef PHP_MULPHPEDITORFACTORY_H_
#define PHP_MULPHPEDITORFACTORY_H_

#include "mulphpeditorconnector.h"
#include <elk.h>
#include <caterpillar.h>
#include <dragonfly.h>
#include <leafhopper.h>
#include <viper.h>
#include <cow.h>

G_BEGIN_DECLS

#define MUL_TYPE_PHP_EDITOR_FACTORY              (mul_php_editor_factory_get_type())
#define MUL_PHP_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_php_editor_factory_get_type(), MulPhpEditorFactory))
#define MUL_PHP_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_PHP_EDITOR_FACTORY, MulPhpEditorFactoryClass))
#define MUL_IS_PHP_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_PHP_EDITOR_FACTORY))
#define MUL_IS_PHP_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_PHP_EDITOR_FACTORY))
#define MUL_PHP_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_PHP_EDITOR_FACTORY, MulPhpEditorFactoryClass))


typedef struct _MulPhpEditorFactory               MulPhpEditorFactory;
typedef struct _MulPhpEditorFactoryPrivate        MulPhpEditorFactoryPrivate;
typedef struct _MulPhpEditorFactoryClass          MulPhpEditorFactoryClass;


struct _MulPhpEditorFactory {
	GObject parent;
};

struct _MulPhpEditorFactoryClass {
	GObjectClass parent_class;
};


GType mul_php_editor_factory_get_type();

MulPhpEditorFactory *mul_php_editor_factory_new(LeaIPanelOwner *panel_owner, MulPhpEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file);

G_END_DECLS

#endif /* PHP_MULPHPEDITORFACTORY_H_ */
