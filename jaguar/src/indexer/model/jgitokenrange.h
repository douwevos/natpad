/*
   File:    jgitokenrange.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 16, 2014
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

#ifndef JGITOKENRANGE_H_
#define JGITOKENRANGE_H_

#include <caterpillar.h>
#include <jaguarparser.h>

G_BEGIN_DECLS

#define JGI_TYPE_TOKEN_RANGE              (jgi_token_range_get_type())
#define JGI_TOKEN_RANGE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jgi_token_range_get_type(), JgiTokenRange))
#define JGI_TOKEN_RANGE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JGI_TYPE_TOKEN_RANGE, JgiTokenRangeClass))
#define JGI_IS_TOKEN_RANGE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JGI_TYPE_TOKEN_RANGE))
#define JGI_IS_TOKEN_RANGE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JGI_TYPE_TOKEN_RANGE))
#define JGI_TOKEN_RANGE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JGI_TYPE_TOKEN_RANGE, JgiTokenRangeClass))

typedef struct _JgiTokenRange               JgiTokenRange;
typedef struct _JgiTokenRangePrivate        JgiTokenRangePrivate;
typedef struct _JgiTokenRangeClass          JgiTokenRangeClass;


struct _JgiTokenRange {
	GObject parent;
};

struct _JgiTokenRangeClass {
	GObjectClass parent_class;
};

GType jgi_token_range_get_type();

JgiTokenRange *jgi_token_range_new(CatArrayWo *e_token_list, int first_index, int last_index);

int jgi_token_range_get_first_index(JgiTokenRange *token_range);
int jgi_token_range_get_last_index(JgiTokenRange *token_range);

JagPToken *jgi_token_range_get_first(JgiTokenRange *token_range);
JagPToken *jgi_token_range_get_last(JgiTokenRange *token_range);

CatArrayWo *e_jgi_token_range_get_raw_tokens(JgiTokenRange *token_range);

CatIIterator *jgi_token_range_iterator(JgiTokenRange *token_range);

void jgi_token_range_dump(JgiTokenRange *token_range);

G_END_DECLS

#endif /* JGITOKENRANGE_H_ */
