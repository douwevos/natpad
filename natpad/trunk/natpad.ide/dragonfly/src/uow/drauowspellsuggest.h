/*
   File:    drauowspellsuggest.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 30, 2016
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

#ifndef UOW_DRAUOWSPELLSUGGEST_H_
#define UOW_DRAUOWSPELLSUGGEST_H_

#include "../dralinetagwo.h"
#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define DRA_TYPE_UOW_SPELL_SUGGEST              (dra_uow_spell_suggest_get_type())
#define DRA_UOW_SPELL_SUGGEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_uow_spell_suggest_get_type(), DraUowSpellSuggest))
#define DRA_UOW_SPELL_SUGGEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_UOW_SPELL_SUGGEST, DraUowSpellSuggestClass))
#define DRA_IS_UOW_SPELL_SUGGEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_UOW_SPELL_SUGGEST))
#define DRA_IS_UOW_SPELL_SUGGEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_UOW_SPELL_SUGGEST))
#define DRA_UOW_SPELL_SUGGEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_UOW_SPELL_SUGGEST, DraUowSpellSuggestClass))


typedef struct _DraUowSpellSuggest               DraUowSpellSuggest;
typedef struct _DraUowSpellSuggestPrivate        DraUowSpellSuggestPrivate;
typedef struct _DraUowSpellSuggestClass          DraUowSpellSuggestClass;


struct _DraUowSpellSuggest {
	ChaUow parent;
};

struct _DraUowSpellSuggestClass {
	ChaUowClass parent_class;
};


GType dra_uow_spell_suggest_get_type();

DraUowSpellSuggest *dra_uow_spell_suggest_new(CatStringWo *suggest_word, DraLineTagWo *spell_tag);

G_END_DECLS

#endif /* UOW_DRAUOWSPELLSUGGEST_H_ */
