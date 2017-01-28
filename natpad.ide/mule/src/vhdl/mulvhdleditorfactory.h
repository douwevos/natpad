/*
   File:    mulvhdleditorfactory.h
   Project: mule
   Author:  Douwe Vos
   Date:    Jul 5, 2015
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

#ifndef VHDL_MULVHDLEDITORFACTORY_H_
#define VHDL_MULVHDLEDITORFACTORY_H_

#include "mulvhdleditorconnector.h"
#include <elk.h>
#include <caterpillar.h>
#include <dragonfly.h>
#include <leafhopper.h>
#include <viper.h>
#include <cow.h>

G_BEGIN_DECLS

#define MUL_TYPE_VHDL_EDITOR_FACTORY              (mul_vhdl_editor_factory_get_type())
#define MUL_VHDL_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_vhdl_editor_factory_get_type(), MulVhdlEditorFactory))
#define MUL_VHDL_EDITOR_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_VHDL_EDITOR_FACTORY, MulVhdlEditorFactoryClass))
#define MUL_IS_VHDL_EDITOR_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_VHDL_EDITOR_FACTORY))
#define MUL_IS_VHDL_EDITOR_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_VHDL_EDITOR_FACTORY))
#define MUL_VHDL_EDITOR_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_VHDL_EDITOR_FACTORY, MulVhdlEditorFactoryClass))


typedef struct _MulVhdlEditorFactory               MulVhdlEditorFactory;
typedef struct _MulVhdlEditorFactoryPrivate        MulVhdlEditorFactoryPrivate;
typedef struct _MulVhdlEditorFactoryClass          MulVhdlEditorFactoryClass;


struct _MulVhdlEditorFactory {
	GObject parent;
};

struct _MulVhdlEditorFactoryClass {
	GObjectClass parent_class;
};


GType mul_vhdl_editor_factory_get_type();

MulVhdlEditorFactory *mul_vhdl_editor_factory_new(LeaIPanelOwner *panel_owner, MulVhdlEditorConnector *connector, ElkDocumentIO *document_io, VipIFile *document_file);

G_END_DECLS

#endif /* VHDL_MULVHDLEDITORFACTORY_H_ */
