/*
   File:    leaidropproposal.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    May 14, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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


#include "leaidropproposal.h"

static void lea_idrop_proposal_interface_init(gpointer klass) {
//	LeaIDropProposalInterface *iface = (LeaIDropProposalInterface*) klass;
}


GType lea_idrop_proposal_get_type (void) {
	static volatile gsize g_define_type_id__volatile = 0;
	if (g_once_init_enter(&g_define_type_id__volatile)) {
		static const GTypeInfo info = {
				sizeof (LeaIDropProposalInterface),
				lea_idrop_proposal_interface_init,   /* base_init */
				NULL,   /* base_finalize */
		};

		GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE, g_intern_static_string ("LeaIDropProposal"), &info,0);
		g_once_init_leave(&g_define_type_id__volatile, g_define_type_id);
	}
	return g_define_type_id__volatile;
}


void lea_idrop_proposal_paint(LeaIDropProposal *self, cairo_t *cairo) {
	g_return_if_fail(LEA_IS_IDROP_PROPOSAL(self));
	LEA_IDROP_PROPOSAL_GET_INTERFACE(self)->paint(self, cairo);
}


void lea_idrop_proposal_invalidate(LeaIDropProposal *self, GdkWindow *window) {
	g_return_if_fail(LEA_IS_IDROP_PROPOSAL(self));
	LEA_IDROP_PROPOSAL_GET_INTERFACE(self)->invalidate(self, window);
}


gboolean lea_idrop_proposal_equal(LeaIDropProposal *self, LeaIDropProposal *other) {
	if (self==other) {
		return TRUE;
	}
	if (self==NULL) {
		return FALSE;
	}
	g_return_val_if_fail(LEA_IS_IDROP_PROPOSAL(self), FALSE);
	return LEA_IDROP_PROPOSAL_GET_INTERFACE(self)->equal(self, other);
}

