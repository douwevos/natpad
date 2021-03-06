/*
   File:    jaglibrarymatcher.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jul 20, 2014
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

#include "jaglibrarymatcher.h"
#include "jaglibrarycontentwo.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagLibraryMatcher"
#include <logging/catlog.h>

struct _JagLibraryMatcherPrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagLibraryMatcher, jag_library_matcher, G_TYPE_OBJECT, // @suppress("Unused static function")
		G_ADD_PRIVATE(JagLibraryMatcher)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_MATCHER, l_node_matcher_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_library_matcher_class_init(JagLibraryMatcherClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_library_matcher_init(JagLibraryMatcher *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagLibraryMatcher *instance = JAG_LIBRARY_MATCHER(object);
//	JagLibraryMatcherPrivate *priv = jag_library_matcher_get_instance_private(instance);
	G_OBJECT_CLASS(jag_library_matcher_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_library_matcher_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagLibraryMatcher *jag_library_matcher_new() {
	JagLibraryMatcher *result = g_object_new(JAG_TYPE_LIBRARY_MATCHER, NULL);
	cat_ref_anounce(result);
	return result;
}


/********************* start MooIContentMap implementation *********************/

static gboolean l_node_matcher_matches(MooINodeMatcher *self, struct _MooNodeWo *node, gpointer userdata) {
	cat_log_debug("matching:node=%o", node);
	JagLibraryContentWo *content = (JagLibraryContentWo *) moo_node_wo_get_content(node, jag_library_content_wo_key());
	gboolean result = FALSE;
	if (content!=NULL) {
		result = content == (JagLibraryContentWo *) userdata;
	}
	return result;
}


static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface) {
	iface->matches = l_node_matcher_matches;
}

/********************* end MooIContentMap implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
