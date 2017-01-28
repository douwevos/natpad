/*
   File:    jagdecicrossrule.h
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


#ifndef JAGDECICROSSRULE_H_
#define JAGDECICROSSRULE_H_

#include <glib-object.h>

#define JAG_DEC_TYPE_ICROSS_RULE                 (jag_dec_icross_rule_get_type())
#define JAG_DEC_ICROSS_RULE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_DEC_TYPE_ICROSS_RULE, JagDecICrossRule))
#define JAG_DEC_IS_ICROSS_RULE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_DEC_TYPE_ICROSS_RULE))
#define JAG_DEC_ICROSS_RULE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_DEC_TYPE_ICROSS_RULE, JagDecICrossRuleInterface))


typedef struct _JagDecICrossRule               JagDecICrossRule; /* dummy object */
typedef struct _JagDecICrossRuleInterface      JagDecICrossRuleInterface;

struct _JagDecICrossRuleInterface {
	GTypeInterface parent_iface;

	gboolean (*apply)(JagDecICrossRule *self);
};

GType jag_dec_icross_rule_get_type(void);

gboolean jag_dec_icross_rule_apply(JagDecICrossRule *self);

#endif /* JAGDECICROSSRULE_H_ */
