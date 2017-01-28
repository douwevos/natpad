/*
   File:    jagprefscompilerpanel.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 28, 2014
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

#ifndef JAGPREFSCOMPILERPANEL_H_
#define JAGPREFSCOMPILERPANEL_H_

#include <caterpillar.h>
#include <cow.h>
#include <viper.h>

G_BEGIN_DECLS

#define JAG_TYPE_PREFS_COMPILER_PANEL              (jag_prefs_compiler_panel_get_type())
#define JAG_PREFS_COMPILER_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_prefs_compiler_panel_get_type(), JagPrefsCompilerPanel))
#define JAG_PREFS_COMPILER_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PREFS_COMPILER_PANEL, JagPrefsCompilerPanelClass))
#define JAG_IS_PREFS_COMPILER_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PREFS_COMPILER_PANEL))
#define JAG_IS_PREFS_COMPILER_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PREFS_COMPILER_PANEL))
#define JAG_PREFS_COMPILER_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PREFS_COMPILER_PANEL, JagPrefsCompilerPanelClass))


typedef struct _JagPrefsCompilerPanel               JagPrefsCompilerPanel;
typedef struct _JagPrefsCompilerPanelPrivate        JagPrefsCompilerPanelPrivate;
typedef struct _JagPrefsCompilerPanelClass          JagPrefsCompilerPanelClass;


struct _JagPrefsCompilerPanel {
	CowPanel parent;
};

struct _JagPrefsCompilerPanelClass {
	CowPanelClass parent_class;
};


GType jag_prefs_compiler_panel_get_type();

JagPrefsCompilerPanel *jag_prefs_compiler_panel_new(VipService *vip_service);

G_END_DECLS

#endif /* JAGPREFSCOMPILERPANEL_H_ */
