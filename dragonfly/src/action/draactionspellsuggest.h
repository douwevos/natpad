/*
   File:    draactionspellsuggest.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 29, 2016
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

#ifndef ACTION_DRAACTIONSPELLSUGGEST_H_
#define ACTION_DRAACTIONSPELLSUGGEST_H_

#include "../draeditorpanel.h"
#include "../dralinetagwo.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_SPELL_SUGGEST              (dra_action_spell_suggest_get_type())
#define DRA_ACTION_SPELL_SUGGEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_spell_suggest_get_type(), DraActionSpellSuggest))
#define DRA_ACTION_SPELL_SUGGEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_SPELL_SUGGEST, DraActionSpellSuggestClass))
#define DRA_IS_ACTION_SPELL_SUGGEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_SPELL_SUGGEST))
#define DRA_IS_ACTION_SPELL_SUGGEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_SPELL_SUGGEST))
#define DRA_ACTION_SPELL_SUGGEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_SPELL_SUGGEST, DraActionSpellSuggestClass))


typedef struct _DraActionSpellSuggest               DraActionSpellSuggest;
typedef struct _DraActionSpellSuggestPrivate        DraActionSpellSuggestPrivate;
typedef struct _DraActionSpellSuggestClass          DraActionSpellSuggestClass;


struct _DraActionSpellSuggest {
	LeaAction parent;
};

struct _DraActionSpellSuggestClass {
	LeaActionClass parent_class;
};


GType dra_action_spell_suggest_get_type();

DraActionSpellSuggest *dra_action_spell_suggest_new(DraEditorPanel *editor_panel, CatStringWo *suggest_word, DraLineTagWo *spell_tag);

G_END_DECLS

#endif /* ACTION_DRAACTIONSPELLSUGGEST_H_ */
