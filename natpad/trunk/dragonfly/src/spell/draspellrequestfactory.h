/*
   File:    draspellrequestfactory.h
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

#ifndef SPELL_DRASPELLREQUESTFACTORY_H_
#define SPELL_DRASPELLREQUESTFACTORY_H_

#include <caterpillar.h>
#include "draspellhelper.h"

G_BEGIN_DECLS

#define DRA_TYPE_SPELL_REQUEST_FACTORY              (dra_spell_request_factory_get_type())
#define DRA_SPELL_REQUEST_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_spell_request_factory_get_type(), DraSpellRequestFactory))
#define DRA_SPELL_REQUEST_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_SPELL_REQUEST_FACTORY, DraSpellRequestFactoryClass))
#define DRA_IS_SPELL_REQUEST_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_SPELL_REQUEST_FACTORY))
#define DRA_IS_SPELL_REQUEST_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_SPELL_REQUEST_FACTORY))
#define DRA_SPELL_REQUEST_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_SPELL_REQUEST_FACTORY, DraSpellRequestFactoryClass))


typedef struct _DraSpellRequestFactory               DraSpellRequestFactory;
typedef struct _DraSpellRequestFactoryPrivate        DraSpellRequestFactoryPrivate;
typedef struct _DraSpellRequestFactoryClass          DraSpellRequestFactoryClass;


struct _DraSpellRequestFactory {
	GObject parent;
};

struct _DraSpellRequestFactoryClass {
	GObjectClass parent_class;
};


GType dra_spell_request_factory_get_type();

DraSpellRequestFactory *dra_spell_request_factory_new(DraSpellHelper *spell_helper, CatStringWo *a_slot_key);

G_END_DECLS

#endif /* SPELL_DRASPELLREQUESTFACTORY_H_ */
