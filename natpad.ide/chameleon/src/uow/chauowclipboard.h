/*
   File:    chauowclipboard.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    May 9, 2015
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

#ifndef UOW_CHAUOWCLIPBOARD_H_
#define UOW_CHAUOWCLIPBOARD_H_

#include "chauow.h"
#include <caterpillar.h>

G_BEGIN_DECLS

typedef enum {
		CHA_UOW_CLIPBOARD_CUT,
		CHA_UOW_CLIPBOARD_COPY,
		CHA_UOW_CLIPBOARD_PASTE
	} ChaUowClipboardAction;

#define CHA_TYPE_UOW_CLIPBOARD              (cha_uow_clipboard_get_type())
#define CHA_UOW_CLIPBOARD(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_uow_clipboard_get_type(), ChaUowClipboard))
#define CHA_UOW_CLIPBOARD_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_UOW_CLIPBOARD, ChaUowClipboardClass))
#define CHA_IS_UOW_CLIPBOARD(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_UOW_CLIPBOARD))
#define CHA_IS_UOW_CLIPBOARD_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_UOW_CLIPBOARD))
#define CHA_UOW_CLIPBOARD_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_UOW_CLIPBOARD, ChaUowClipboardClass))


typedef struct _ChaUowClipboard               ChaUowClipboard;
typedef struct _ChaUowClipboardPrivate        ChaUowClipboardPrivate;
typedef struct _ChaUowClipboardClass          ChaUowClipboardClass;


struct _ChaUowClipboard {
	ChaUow parent;
};

struct _ChaUowClipboardClass {
	ChaUowClass parent_class;
};


GType cha_uow_clipboard_get_type();

ChaUowClipboard *cha_uow_clipboard_new(ChaUowClipboardAction action, gboolean marked_lines_only);

G_END_DECLS

#endif /* UOW_CHAUOWCLIPBOARD_H_ */
