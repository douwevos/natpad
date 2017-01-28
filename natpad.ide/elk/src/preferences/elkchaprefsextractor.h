/*
   File:    elkchaprefsextractor.h
   Project: elk
   Author:  Douwe Vos
   Date:    Jun 26, 2015
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

#ifndef ELKCHAPREFSEXTRACTOR_H_
#define ELKCHAPREFSEXTRACTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_CHA_PREFS_EXTRACTOR              (elk_cha_prefs_extractor_get_type())
#define ELK_CHA_PREFS_EXTRACTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_cha_prefs_extractor_get_type(), ElkChaPrefsExtractor))
#define ELK_CHA_PREFS_EXTRACTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_CHA_PREFS_EXTRACTOR, ElkChaPrefsExtractorClass))
#define ELK_IS_CHA_PREFS_EXTRACTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_CHA_PREFS_EXTRACTOR))
#define ELK_IS_CHA_PREFS_EXTRACTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_CHA_PREFS_EXTRACTOR))
#define ELK_CHA_PREFS_EXTRACTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_CHA_PREFS_EXTRACTOR, ElkChaPrefsExtractorClass))


typedef struct _ElkChaPrefsExtractor               ElkChaPrefsExtractor;
typedef struct _ElkChaPrefsExtractorClass          ElkChaPrefsExtractorClass;


struct _ElkChaPrefsExtractor {
	GObject parent;
};

struct _ElkChaPrefsExtractorClass {
	GObjectClass parent_class;
};


GType elk_cha_prefs_extractor_get_type();

ElkChaPrefsExtractor *elk_cha_prefs_extractor_new();

G_END_DECLS


#endif /* ELKCHAPREFSEXTRACTOR_H_ */
