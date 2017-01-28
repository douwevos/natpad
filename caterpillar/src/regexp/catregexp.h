/*
   File:    catregexp.h
   Project: Natpad
   Author:  Douwe Vos
   Date:    Dec 12, 2008
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2008 Douwe Vos.

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

#ifndef CATREGEXP_H_
#define CATREGEXP_H_

#include <glib-object.h>
#include "../woo/catstringwo.h"
#include "catregexpnode.h"

G_BEGIN_DECLS

#define CAT_TYPE_REGEXP            (cat_regexp_get_type())
#define CAT_REGEXP(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_regexp_get_type(), CatRegexp))
#define CAT_REGEXP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_REGEXP, CatRegexpClass))
#define CAT_IS_REGEXP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_REGEXP))
#define CAT_IS_REGEXP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_REGEXP))
#define CAT_REGEXP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_REGEXP, CatRegexpClass))


typedef struct _CatRegexp         CatRegexp;

typedef struct _CatRegexpClass    CatRegexpClass;


struct _CatRegexp {
	GObject parent;
	gshort hook_version;
	CatStringWo *a_pattern;
	CatRegexpNode *root;
	int token_count;
};


struct _CatRegexpClass {
	GObjectClass parent_class;
};


CatRegexp *cat_regexp_new(CatStringWo *a_pattern);

gboolean cat_regexp_match(CatRegexp *th, CatStringWo *a_text);

G_END_DECLS

#endif /* CATREGEXP_H_ */
