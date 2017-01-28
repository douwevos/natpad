/*
   File:    draspellchecker.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 26, 2016
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

#ifndef SPELL_DRASPELLCHECKER_H_
#define SPELL_DRASPELLCHECKER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_SPELL_CHECKER              (dra_spell_checker_get_type())
#define DRA_SPELL_CHECKER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_spell_checker_get_type(), DraSpellChecker))
#define DRA_SPELL_CHECKER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_SPELL_CHECKER, DraSpellCheckerClass))
#define DRA_IS_SPELL_CHECKER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_SPELL_CHECKER))
#define DRA_IS_SPELL_CHECKER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_SPELL_CHECKER))
#define DRA_SPELL_CHECKER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_SPELL_CHECKER, DraSpellCheckerClass))


typedef struct _DraSpellChecker               DraSpellChecker;
typedef struct _DraSpellCheckerPrivate        DraSpellCheckerPrivate;
typedef struct _DraSpellCheckerClass          DraSpellCheckerClass;


typedef struct _DraSpellWord DraSpellWord;

struct _DraSpellWord {
	CatStringWo *word;
	long row;
	int column_start, column_end;
};


struct _DraSpellChecker {
	GObject parent;
};

struct _DraSpellCheckerClass {
	GObjectClass parent_class;
};


GType dra_spell_checker_get_type();

DraSpellChecker *dra_spell_checker_new(CatIUtf8Scanner *input);
DraSpellChecker *dra_spell_checker_new_at(CatIUtf8Scanner *input, int column, long long row);


const DraSpellWord dra_spell_checker_next_word(DraSpellChecker *checker);

G_END_DECLS

#endif /* SPELL_DRASPELLCHECKER_H_ */
