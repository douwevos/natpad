/*
   File:    dratagpopup.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Nov 30, 2016
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

#ifndef HIGHLIGHTING_DRATAGPOPUP_H_
#define HIGHLIGHTING_DRATAGPOPUP_H_

#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define DRA_TYPE_TAG_POPUP              (dra_tag_popup_get_type())
#define DRA_TAG_POPUP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_tag_popup_get_type(), DraTagPopup))
#define DRA_TAG_POPUP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_TAG_POPUP, DraTagPopupClass))
#define DRA_IS_TAG_POPUP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_TAG_POPUP))
#define DRA_IS_TAG_POPUP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_TAG_POPUP))
#define DRA_TAG_POPUP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_TAG_POPUP, DraTagPopupClass))


typedef struct _DraTagPopup               DraTagPopup;
typedef struct _DraTagPopupPrivate        DraTagPopupPrivate;
typedef struct _DraTagPopupClass          DraTagPopupClass;


struct _DraTagPopup {
	GObject parent;
};

struct _DraTagPopupClass {
	GObjectClass parent_class;
};


GType dra_tag_popup_get_type();

DraTagPopup *dra_tag_popup_new(ChaEditor *editor, int xpos, int ypos);

void dra_tag_popup_show(DraTagPopup *popup, CatStringWo *text);

G_END_DECLS

#endif /* HIGHLIGHTING_DRATAGPOPUP_H_ */
