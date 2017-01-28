/*
   File:    draspellrequest.h
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

#ifndef SPELL_DRASPELLREQUEST_H_
#define SPELL_DRASPELLREQUEST_H_

#include <caterpillar.h>
#include <chameleon.h>
#include <worm.h>

G_BEGIN_DECLS

#define DRA_TYPE_SPELL_REQUEST              (dra_spell_request_get_type())
#define DRA_SPELL_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_spell_request_get_type(), DraSpellRequest))
#define DRA_SPELL_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_SPELL_REQUEST, DraSpellRequestClass))
#define DRA_IS_SPELL_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_SPELL_REQUEST))
#define DRA_IS_SPELL_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_SPELL_REQUEST))
#define DRA_SPELL_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_SPELL_REQUEST, DraSpellRequestClass))


typedef struct _DraSpellRequest               DraSpellRequest;
typedef struct _DraSpellRequestPrivate        DraSpellRequestPrivate;
typedef struct _DraSpellRequestClass          DraSpellRequestClass;


struct _DraSpellRequest {
	WorRequest parent;
};

struct _DraSpellRequestClass {
	WorRequestClass parent_class;
};


GType dra_spell_request_get_type();

DraSpellRequest *dra_spell_request_new(ChaDocument *document, ChaRevisionWo *a_new_revision, CatStringWo *slot_key);

G_END_DECLS

#endif /* SPELL_DRASPELLREQUEST_H_ */
