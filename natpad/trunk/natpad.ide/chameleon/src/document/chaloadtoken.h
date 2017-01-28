/*
   File:    chaloadtoken.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jun 22, 2015
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

#ifndef DOCUMENT_CHALOADTOKEN_H_
#define DOCUMENT_CHALOADTOKEN_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_LOAD_TOKEN              (cha_load_token_get_type())
#define CHA_LOAD_TOKEN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_load_token_get_type(), ChaLoadToken))
#define CHA_LOAD_TOKEN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_LOAD_TOKEN, ChaLoadTokenClass))
#define CHA_IS_LOAD_TOKEN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_LOAD_TOKEN))
#define CHA_IS_LOAD_TOKEN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_LOAD_TOKEN))
#define CHA_LOAD_TOKEN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_LOAD_TOKEN, ChaLoadTokenClass))


typedef struct _ChaLoadToken               ChaLoadToken;
typedef struct _ChaLoadTokenPrivate        ChaLoadTokenPrivate;
typedef struct _ChaLoadTokenClass          ChaLoadTokenClass;


struct _ChaLoadToken {
	GObject parent;
};

struct _ChaLoadTokenClass {
	GObjectClass parent_class;
};


GType cha_load_token_get_type();

ChaLoadToken *cha_load_token_new();

void cha_load_token_set_loaded_pages(ChaLoadToken *token, CatArrayWo *loaded_pages, ChaLoadToken *follow_up_token);

G_END_DECLS

#endif /* DOCUMENT_CHALOADTOKEN_H_ */
