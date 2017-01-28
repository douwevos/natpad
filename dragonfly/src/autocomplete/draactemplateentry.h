/*
   File:    draactemplateentry.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 20, 2015
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

#ifndef AUTOCOMPLETE_DRAACTEMPLATEENTRY_H_
#define AUTOCOMPLETE_DRAACTEMPLATEENTRY_H_

#include "draacentry.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_AC_TEMPLATE_ENTRY              (dra_ac_template_entry_get_type())
#define DRA_AC_TEMPLATE_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_ac_template_entry_get_type(), DraAcTemplateEntry))
#define DRA_AC_TEMPLATE_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_AC_TEMPLATE_ENTRY, DraAcTemplateEntryClass))
#define DRA_IS_AC_TEMPLATE_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_AC_TEMPLATE_ENTRY))
#define DRA_IS_AC_TEMPLATE_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_AC_TEMPLATE_ENTRY))
#define DRA_AC_TEMPLATE_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_AC_TEMPLATE_ENTRY, DraAcTemplateEntryClass))


typedef struct _DraAcTemplateEntry               DraAcTemplateEntry;
typedef struct _DraAcTemplateEntryPrivate        DraAcTemplateEntryPrivate;
typedef struct _DraAcTemplateEntryClass          DraAcTemplateEntryClass;


struct _DraAcTemplateEntry {
	DraAcEntry parent;
};

struct _DraAcTemplateEntryClass {
	DraAcEntryClass parent_class;
};


GType dra_ac_template_entry_get_type();

DraAcTemplateEntry *dra_ac_template_entry_new(CatStringWo *a_txt, CatStringWo *a_replace_with);

G_END_DECLS

#endif /* AUTOCOMPLETE_DRAACTEMPLATEENTRY_H_ */
