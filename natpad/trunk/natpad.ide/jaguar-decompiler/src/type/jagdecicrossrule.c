/*
   File:    jagdecicrossrule.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 7, 2012
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

#include "jagdecicrossrule.h"

G_DEFINE_INTERFACE(JagDecICrossRule, jag_dec_icross_rule, G_TYPE_OBJECT);

static void jag_dec_icross_rule_default_init (JagDecICrossRuleInterface *klass) {
}

gboolean jag_dec_icross_rule_apply(JagDecICrossRule *self) {
	g_return_val_if_fail(JAG_DEC_IS_ICROSS_RULE(self), FALSE);
	return JAG_DEC_ICROSS_RULE_GET_INTERFACE(self)->apply(self);
}

