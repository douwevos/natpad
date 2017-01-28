/*
   File:    draspellhelper.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 27, 2016
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

#ifndef SPELL_DRASPELLHELPER_H_
#define SPELL_DRASPELLHELPER_H_

#include "../drakeywordprinter.h"
#include "../preferences/drapreferenceswo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_SPELL_HELPER              (dra_spell_helper_get_type())
#define DRA_SPELL_HELPER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_spell_helper_get_type(), DraSpellHelper))
#define DRA_SPELL_HELPER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_SPELL_HELPER, DraSpellHelperClass))
#define DRA_IS_SPELL_HELPER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_SPELL_HELPER))
#define DRA_IS_SPELL_HELPER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_SPELL_HELPER))
#define DRA_SPELL_HELPER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_SPELL_HELPER, DraSpellHelperClass))


typedef struct _DraSpellHelper               DraSpellHelper;
typedef struct _DraSpellHelperPrivate        DraSpellHelperPrivate;
typedef struct _DraSpellHelperClass          DraSpellHelperClass;


struct _DraSpellHelper {
	GObject parent;
};

struct _DraSpellHelperClass {
	GObjectClass parent_class;
};


GType dra_spell_helper_get_type();

DraSpellHelper *dra_spell_helper_new();

DraSpellHelper *dra_spell_helper_reconfigure(DraSpellHelper *spell_helper, DraPreferencesWo *a_prefs);

void dra_spell_helper_scan(DraSpellHelper *spell_helper, DraKeywordPrinter *line_tag_printer, CatStringWo *text, int column_start, long long row_start);

CatArrayWo *dra_spell_helper_enlist_corrections(DraSpellHelper *spell_helper, CatStringWo *word);

G_END_DECLS

#endif /* SPELL_DRASPELLHELPER_H_ */
