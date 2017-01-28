/*
   File:    leadroptotabproposal.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 23, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef LEADROPTOTABPROPOSAL_H_
#define LEADROPTOTABPROPOSAL_H_

#include <caterpillar.h>
#include "../leasurface.h"

G_BEGIN_DECLS

#define LEA_TYPE_DROP_TO_TAB_PROPOSAL              (lea_drop_to_tab_proposal_get_type())
#define LEA_DROP_TO_TAB_PROPOSAL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_drop_to_tab_proposal_get_type(), LeaDropToTabProposal))
#define LEA_DROP_TO_TAB_PROPOSAL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_DROP_TO_TAB_PROPOSAL, LeaDropToTabProposalClass))
#define LEA_IS_DROP_TO_TAB_PROPOSAL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_DROP_TO_TAB_PROPOSAL))
#define LEA_IS_DROP_TO_TAB_PROPOSAL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_DROP_TO_TAB_PROPOSAL))
#define LEA_DROP_TO_TAB_PROPOSAL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_DROP_TO_TAB_PROPOSAL, LeaDropToTabProposalClass))


typedef struct _LeaDropToTabProposal               LeaDropToTabProposal;
typedef struct _LeaDropToTabProposalPrivate        LeaDropToTabProposalPrivate;
typedef struct _LeaDropToTabProposalClass          LeaDropToTabProposalClass;


struct _LeaDropToTabProposal {
	GObject parent;
};

struct _LeaDropToTabProposalClass {
	GObjectClass parent_class;
};


GType lea_drop_to_tab_proposal_get_type();

LeaDropToTabProposal *lea_drop_to_tab_proposal_new(LeaSurface *surface, int panel_idx);

LeaSurface *lea_drop_to_tab_proposal_get_surface(LeaDropToTabProposal *drop_proposal);
int lea_drop_to_tab_proposal_get_tab_index(LeaDropToTabProposal *drop_proposal);

G_END_DECLS

#endif /* LEADROPTOTABPROPOSAL_H_ */
