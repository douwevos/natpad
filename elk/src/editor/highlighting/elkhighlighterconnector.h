/*
   File:    elkhighlighterconnector.h
   Project: elk
   Author:  Douwe Vos
   Date:    Feb 9, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#ifndef ELKHIGHLIGHTERCONNECTOR_H_
#define ELKHIGHLIGHTERCONNECTOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_HIGHLIGHTER_CONNECTOR              (elk_highlighter_connector_get_type())
#define ELK_HIGHLIGHTER_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_highlighter_connector_get_type(), ElkHighlighterConnector))
#define ELK_HIGHLIGHTER_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_HIGHLIGHTER_CONNECTOR, ElkHighlighterConnectorClass))
#define ELK_IS_HIGHLIGHTER_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_HIGHLIGHTER_CONNECTOR))
#define ELK_IS_HIGHLIGHTER_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_HIGHLIGHTER_CONNECTOR))
#define ELK_HIGHLIGHTER_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_HIGHLIGHTER_CONNECTOR, ElkHighlighterConnectorClass))

typedef struct _ElkHighlighterConnector               ElkHighlighterConnector;
typedef struct _ElkHighlighterConnectorPrivate        ElkHighlighterConnectorPrivate;
typedef struct _ElkHighlighterConnectorClass          ElkHighlighterConnectorClass;


struct _ElkHighlighterConnector {
	GObject parent;
};

struct _ElkHighlighterConnectorClass {
	GObjectClass parent_class;
};


GType elk_highlighter_connector_get_type();

void elk_highlighter_connector_construct(ElkHighlighterConnector *connector, CatStringWo *a_markup_hook_key);


CatStringWo *elk_highlighter_connector_get_markup_hook_key(ElkHighlighterConnector *connector);


void elk_highlighter_connector_increase_usage(ElkHighlighterConnector *connector);

gboolean elk_highlighter_connector_decrease_usage(ElkHighlighterConnector *connector);

G_END_DECLS

#endif /* ELKHIGHLIGHTERCONNECTOR_H_ */
