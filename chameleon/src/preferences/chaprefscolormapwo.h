/*
   File:    chaprefscolormapwo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 19, 2014
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

#ifndef CHAPREFSCOLORMAPWO_H_
#define CHAPREFSCOLORMAPWO_H_

#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CHA_TYPE_PREFS_COLOR_MAP_WO              (cha_prefs_color_map_wo_get_type())
#define CHA_PREFS_COLOR_MAP_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_prefs_color_map_wo_get_type(), ChaPrefsColorMapWo))
#define CHA_PREFS_COLOR_MAP_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_PREFS_COLOR_MAP_WO, ChaPrefsColorMapWoClass))
#define CHA_IS_PREFS_COLOR_MAP_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_PREFS_COLOR_MAP_WO))
#define CHA_IS_PREFS_COLOR_MAP_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_PREFS_COLOR_MAP_WO))
#define CHA_PREFS_COLOR_MAP_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_PREFS_COLOR_MAP_WO, ChaPrefsColorMapWoClass))


#define CHA_COLOR_BACKGROUND        0
#define CHA_COLOR_FOREGROUND        1
#define CHA_COLOR_FOREGROUND        1

#define CHA_COLOR_CURRENT_LINE      16
#define CHA_COLOR_BLOCK_SELECTION   17

typedef struct _ChaPrefsColorMapWo               ChaPrefsColorMapWo;
typedef struct _ChaPrefsColorMapWoPrivate        ChaPrefsColorMapWoPrivate;
typedef struct _ChaPrefsColorMapWoClass          ChaPrefsColorMapWoClass;

struct _ChaColorEntry {
	double red, green, blue;
};

typedef struct _ChaColorEntry ChaColorEntry;

struct _ChaColorEntryInt {
	guint16 red, green, blue;
};

typedef struct _ChaColorEntryInt ChaColorEntryInt;


struct _ChaPrefsColorMapWo {
	CatWo parent;
};

struct _ChaPrefsColorMapWoClass {
	CatWoClass parent_class;
};


GType cha_prefs_color_map_wo_get_type();

ChaPrefsColorMapWo *cha_prefs_color_map_wo_new();

void cha_prefs_color_map_wo_set_cairo_color(const ChaPrefsColorMapWo *color_map, cairo_t *cairo, int color_idx);

int cha_prefs_color_map_wo_count(const ChaPrefsColorMapWo *color_map);
ChaColorEntry cha_prefs_color_map_wo_get_color(const ChaPrefsColorMapWo *color_map, int color_idx);
ChaColorEntryInt cha_prefs_color_map_wo_get_color_int(const ChaPrefsColorMapWo *color_map, int color_idx);

void cha_prefs_color_map_wo_set_entry_count(ChaPrefsColorMapWo *e_color_map, int entry_count);
void cha_prefs_color_map_wo_set_color_at(ChaPrefsColorMapWo *e_color_map, double red, double green, double blue, int color_idx);

CAT_WO_BASE_H(ChaPrefsColorMapWo, cha_prefs_color_map_wo);

G_END_DECLS

#endif /* CHAPREFSCOLORMAPWO_H_ */
