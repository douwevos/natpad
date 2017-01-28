/*
   File:    leadropcardinalproposal.h
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

#ifndef LEADROPCARDINALPROPOSAL_H_
#define LEADROPCARDINALPROPOSAL_H_

#include "../leairegion.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_DROP_CARDINAL_PROPOSAL              (lea_drop_cardinal_proposal_get_type())
#define LEA_DROP_CARDINAL_PROPOSAL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_drop_cardinal_proposal_get_type(), LeaDropCardinalProposal))
#define LEA_DROP_CARDINAL_PROPOSAL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_DROP_CARDINAL_PROPOSAL, LeaDropCardinalProposalClass))
#define LEA_IS_DROP_CARDINAL_PROPOSAL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_DROP_CARDINAL_PROPOSAL))
#define LEA_IS_DROP_CARDINAL_PROPOSAL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_DROP_CARDINAL_PROPOSAL))
#define LEA_DROP_CARDINAL_PROPOSAL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_DROP_CARDINAL_PROPOSAL, LeaDropCardinalProposalClass))


typedef struct _LeaDropCardinalProposal               LeaDropCardinalProposal;
typedef struct _LeaDropCardinalProposalPrivate        LeaDropCardinalProposalPrivate;
typedef struct _LeaDropCardinalProposalClass          LeaDropCardinalProposalClass;

typedef enum { LEA_CP_WEST, LEA_CP_NORTH, LEA_CP_EAST, LEA_CP_SOUTH, LEA_CP_COVER, LEA_CP_NONE } LeaCardinalPlacement;


struct _LeaDropCardinalProposal {
	GObject parent;
};

struct _LeaDropCardinalProposalClass {
	GObjectClass parent_class;
};


GType lea_drop_cardinal_proposal_get_type();

LeaDropCardinalProposal *lea_drop_cardinal_proposal_new(LeaIRegion *region, int level, LeaCardinalPlacement placement);

LeaCardinalPlacement lea_drop_cardinal_proposal_get_placement(LeaDropCardinalProposal *proposal);

LeaIRegion *lea_drop_cardinal_proposal_get_region(LeaDropCardinalProposal *proposal);

G_END_DECLS

#endif /* LEADROPCARDINALPROPOSAL_H_ */
