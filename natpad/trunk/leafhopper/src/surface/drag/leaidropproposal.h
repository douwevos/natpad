/*
   File:    leaidropproposal.h
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


#ifndef LEAIDROPPROPOSAL_H_
#define LEAIDROPPROPOSAL_H_

#include <gdk/gdk.h>
#include <gtk/gtk.h>

#define LEA_TYPE_IDROP_PROPOSAL                 (lea_idrop_proposal_get_type())
#define LEA_IDROP_PROPOSAL(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), LEA_TYPE_IDROP_PROPOSAL, LeaIDropProposal))
#define LEA_IS_IDROP_PROPOSAL(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), LEA_TYPE_IDROP_PROPOSAL))
#define LEA_IDROP_PROPOSAL_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), LEA_TYPE_IDROP_PROPOSAL, LeaIDropProposalInterface))


typedef struct _LeaIDropProposal               LeaIDropProposal; /* dummy object */
typedef struct _LeaIDropProposalInterface      LeaIDropProposalInterface;

struct _LeaIDropProposalInterface {
	GTypeInterface parent_iface;

	void (*paint)(LeaIDropProposal *self, cairo_t *cairo);

	void (*invalidate)(LeaIDropProposal *self, GdkWindow *window);
	gboolean (*equal)(LeaIDropProposal *self, LeaIDropProposal *other);
};

GType lea_idrop_proposal_get_type(void);


void lea_idrop_proposal_paint(LeaIDropProposal *self, cairo_t *cairo);

void lea_idrop_proposal_invalidate(LeaIDropProposal *self, GdkWindow *window);
gboolean lea_idrop_proposal_equal(LeaIDropProposal *self, LeaIDropProposal *other);


#endif /* LEAIDROPPROPOSAL_H_ */
