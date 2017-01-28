/*
   File:    mulrusteditorfactory.h
   Project: mule
   Author:  Douwe Vos
   Date:    Jan 9, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef RUST_MULRUSTEDITORFACTORY_H_
#define RUST_MULRUSTEDITORFACTORY_H_

#include "mulrusteditorconnector.h"
#include <elk.h>
#include <caterpillar.h>
#include <dragonfly.h>
#include <leafhopper.h>
#include <viper.h>
#include <cow.h>

G_BEGIN_DECLS

#define MUL_TYPE_RUST_EDITOR_FACTORY              (mul_rust_editor_factory_get_type())
#define MUL_RUST_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_rust_editor_factory_get_type(), MulRustEditorFactory))
#define MUL_RUST_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_RUST_EDITOR_FACTORY, MulRustEditorFactoryClass))
#define MUL_IS_RUST_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_RUST_EDITOR_FACTORY))
#define MUL_IS_RUST_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_RUST_EDITOR_FACTORY))
#define MUL_RUST_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_RUST_EDITOR_FACTORY, MulRustEditorFactoryClass))


typedef struct _MulRustEditorFactory               MulRustEditorFactory;
typedef struct _MulRustEditorFactoryPrivate        MulRustEditorFactoryPrivate;
typedef struct _MulRustEditorFactoryClass          MulRustEditorFactoryClass;


struct _MulRustEditorFactory {
	GObject parent;
};

struct _MulRustEditorFactoryClass {
	GObjectClass parent_class;
};


GType mul_rust_editor_factory_get_type();

MulRustEditorFactory *mul_rust_editor_factory_new(LeaIPanelOwner *panel_owner, MulRustEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file);

G_END_DECLS

#endif /* RUST_MULRUSTEDITORFACTORY_H_ */
