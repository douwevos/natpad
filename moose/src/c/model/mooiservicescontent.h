/*
   File:    mooiservicescontent.h
   Project: moose
   Author:  Douwe Vos
   Date:    Mar 14, 2014
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

#ifndef MOOISERVICESCONTENT_H_
#define MOOISERVICESCONTENT_H_

#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define MOO_TYPE_ISERVICES_CONTENT                 (moo_iservices_content_get_type())
#define MOO_ISERVICES_CONTENT(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), MOO_TYPE_ISERVICES_CONTENT, MooIServicesContent))
#define MOO_IS_ISERVICES_CONTENT(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), MOO_TYPE_ISERVICES_CONTENT))
#define MOO_ISERVICES_CONTENT_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), MOO_TYPE_ISERVICES_CONTENT, MooIServicesContentInterface))

typedef struct _MooIServicesContent               MooIServicesContent; /* dummy object */
typedef struct _MooIServicesContentInterface      MooIServicesContentInterface;

struct _MooNodeWo;
struct _MooService;

struct _MooIServicesContentInterface {
	GTypeInterface parent_iface;

	gboolean (*canRefresh)(MooIServicesContent *self);
	void (*refresh)(MooIServicesContent *self, struct _MooService *moo_service, struct _MooNodeWo *node);

	gboolean (*canRename)(MooIServicesContent *self);
	gboolean (*rename)(MooIServicesContent *self, struct _MooService *moo_service, struct _MooNodeWo *node, LeaFrame *frame);

	gboolean (*canDelete)(MooIServicesContent *self);
	gboolean (*delete)(MooIServicesContent *self, struct _MooService *moo_service, struct _MooNodeWo *node, LeaFrame *frame);

};

GType moo_iservices_content_get_type(void);

CatStringWo *moo_iservices_content_key();

gboolean moo_iservices_content_can_refresh(MooIServicesContent *self);
void moo_iservices_content_refresh(MooIServicesContent *self, struct _MooService *moo_service, struct _MooNodeWo *node);

gboolean moo_iservices_content_can_rename(MooIServicesContent *self);
gboolean moo_iservices_content_rename(MooIServicesContent *self, struct _MooService *moo_service, struct _MooNodeWo *node, LeaFrame *frame);

gboolean moo_iservices_content_can_delete(MooIServicesContent *self);
gboolean moo_iservices_content_delete(MooIServicesContent *self, struct _MooService *moo_service, struct _MooNodeWo *node, LeaFrame *frame);

G_END_DECLS

#endif /* MOOISERVICESCONTENT_H_ */
