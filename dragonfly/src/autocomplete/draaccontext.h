/*
   File:    draaccontext.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 18, 2015
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

#ifndef AUTOCOMPLETE_DRAACCONTEXT_H_
#define AUTOCOMPLETE_DRAACCONTEXT_H_

#include "draiaccontextlistener.h"
#include "draacentry.h"
#include <chameleon.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_AC_CONTEXT              (dra_ac_context_get_type())
#define DRA_AC_CONTEXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_ac_context_get_type(), DraAcContext))
#define DRA_AC_CONTEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_AC_CONTEXT, DraAcContextClass))
#define DRA_IS_AC_CONTEXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_AC_CONTEXT))
#define DRA_IS_AC_CONTEXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_AC_CONTEXT))
#define DRA_AC_CONTEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_AC_CONTEXT, DraAcContextClass))


typedef struct _DraAcContext               DraAcContext;
typedef struct _DraAcContextPrivate        DraAcContextPrivate;
typedef struct _DraAcContextClass          DraAcContextClass;


struct _DraAcContext {
	GObject parent;
};

struct _DraAcContextClass {
	GObjectClass parent_class;
};


GType dra_ac_context_get_type();

DraAcContext *dra_ac_context_new(ChaEditor *editor);


gboolean dra_ac_context_set_next_revision(DraAcContext *context, ChaRevisionWo *a_new_revision);

ChaCursorWo *dra_ac_context_get_filter_cursor(DraAcContext *context);
ChaCursorWo *dra_ac_context_get_start_cursor(DraAcContext *context);
ChaCursorWo *dra_ac_context_get_end_cursor(DraAcContext *context);

ChaEditor *dra_ac_context_get_editor(DraAcContext *context);

ChaRevisionReader *dra_ac_context_create_revision_reader(DraAcContext *context);

void dra_ac_context_get_cursor_location(DraAcContext *context, int *column, long long int *row);
ChaCursorWo *dra_ac_context_create_cursor(DraAcContext *context, int column_x_bytes, long long int row);

void dra_ac_context_provider_content_requested(DraAcContext *context);
void dra_ac_context_provider_finished(DraAcContext *context);
void dra_ac_context_wait_for_providers_to_finish(DraAcContext *context);

void dra_ac_add_entry(DraAcContext *context, DraAcEntry *entry);

DraAcEntry *dra_ac_context_get_at(DraAcContext *context, int index);
int dra_ac_context_count(DraAcContext *context);

CatIIterator *dra_ac_context_iterator(DraAcContext *context);

void dra_ac_context_add_listener(DraAcContext *context, DraIAcContextListener *listener);

G_END_DECLS

#endif /* AUTOCOMPLETE_DRAACCONTEXT_H_ */
