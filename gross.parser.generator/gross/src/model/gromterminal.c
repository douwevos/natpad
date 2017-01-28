
/*
   File:    gromterminal.c
   Project: gross
   Author:  Douwe Vos
   Date:    May 25, 2016
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

#include "gromterminal.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroMTerminal"
#include <logging/catlog.h>


struct _GroMTerminalPrivate {
	int precedence_side;
	int precedence_num;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroMTerminal, gro_m_terminal, GRO_TYPE_M_SYMBOL,
		G_ADD_PRIVATE(GroMTerminal)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_m_terminal_class_init(GroMTerminalClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_m_terminal_init(GroMTerminal *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(gro_m_terminal_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_m_terminal_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroMTerminal *gro_m_terminal_new(int index, GroBnfString *name) {
	GroMTerminal *result = g_object_new(GRO_TYPE_M_TERMINAL, NULL);
	cat_ref_anounce(result);
	gro_m_symbol_construct((GroMSymbol *) result, index, name);
	GroMTerminalPrivate *priv = gro_m_terminal_get_instance_private(result);
	priv->precedence_side = -1;
	priv->precedence_num = -1;
	return result;
}


int gro_m_terminal_get_precedence_num(GroMTerminal *term) {
	GroMTerminalPrivate *priv = gro_m_terminal_get_instance_private(term);
	return priv->precedence_num;
}

int gro_m_terminal_get_precedence_side(GroMTerminal *term) {
	GroMTerminalPrivate *priv = gro_m_terminal_get_instance_private(term);
	return priv->precedence_side;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroMTerminal *instance = GRO_M_TERMINAL(self);
	cat_string_wo_format(append_to, "Term[id=%d, '%O']", gro_m_symbol_get_index((GroMSymbol *) instance), gro_m_symbol_get_name((GroMSymbol *) instance));
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
