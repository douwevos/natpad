/*
   File:    catwo.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Oct 6, 2014
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


#ifndef CATWO_H_
#define CATWO_H_

#include <glib-object.h>

G_BEGIN_DECLS

#ifndef __attribute_warn_unused_result__
#define __attribute_warn_unused_result__
#endif

#define CAT_TYPE_WO              (cat_wo_get_type())
#define CAT_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_wo_get_type(), CatWo))
#define CAT_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_WO, CatWoClass))
#define CAT_IS_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_WO))
#define CAT_IS_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_WO))
#define CAT_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_WO, CatWoClass))


enum _CatCloneDepth {

	/* An objects is only cloned when it is non-anchored. The result will be anchored. */
	CAT_CLONE_DEPTH_AS_ANCHORED,

	/* An objects is only cloned when it is non-anchored */
	CAT_CLONE_DEPTH_NONE,

	/* An object is always cloned, its children are cloned using CAT_CLONE_DEPTH_NONE */
	CAT_CLONE_DEPTH_MAIN,

	/* An object is always cloned, as well are its children */
	CAT_CLONE_DEPTH_FULL
};

typedef enum _CatCloneDepth CatCloneDepth;


typedef struct _CatWo               CatWo;
typedef struct _CatWoPrivate        CatWoPrivate;
typedef struct _CatWoClass          CatWoClass;

struct CatWoInfo;

struct _CatWo {
	GObject parent;
};

#define CAT_WO_CLONE_AS_ANCHORED

struct _CatWoClass {
	GObjectClass parent_class;
	CatWo *(*anchor)(CatWo *wo, int version);
	void (*anchorChildren)(CatWo *wo, int version);
//	gboolean (*checkForModifications)(CatWo *wo) __attribute__ ((deprecated));
	gboolean (*equal)(const CatWo *wo_a, const CatWo *wo_b);

	/* use cloneContent instead */
	CatWo *(*clone)(const CatWo *wo_source, CatCloneDepth clone_depth) __attribute__ ((deprecated));
	CatWo *(*constructEditable)(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
	void (*releaseEditInfo)(CatWo *wo, struct CatWoInfo *info);

	CatWo *(*cloneContent)(CatWo *e_uninitialized, const CatWo *wo_source);
	struct CatWoInfo *(*createEditInfo)(CatWo *e_wo, const CatWo *original, struct CatWoInfo *info);

};


GType cat_wo_get_type();

void cat_wo_construct(CatWo *wo, gboolean editable);

CatWo *cat_wo_create_editable(CatWo *wo);
CatWo *cat_wo_anchor(CatWo *wo, int version);
CatWo *cat_wo_get_original(CatWo *wo);
int cat_wo_get_version(const CatWo *wo);
void cat_wo_set_version(CatWo *e_wo, int version);
gboolean cat_wo_is_anchored(const CatWo *wo);
gboolean cat_wo_equal(const CatWo *wo_a, const CatWo *wo_b);
CatWo *cat_wo_clone(const CatWo *wo_source, CatCloneDepth clone_depth);



#define CAT_WO_BASE_H(tp, pre) \
	tp *pre##_create_editable(tp *instance); \
	tp *pre##_anchor(tp *instance, int version); \
	tp *pre##_get_original(tp *instance); \
	gboolean pre##_is_anchored(const tp *instance); \
	gboolean pre##_equal(const tp *instance_a, const tp *instance_b); \
	tp *pre##_clone(const tp *source, CatCloneDepth clone_depth); \


#define CAT_WO_BASE_C(tp, pre) \
	tp *pre##_create_editable(tp *instance) { \
		return (tp *) cat_wo_create_editable((CatWo *) instance); \
	} \
	tp *pre##_anchor(tp *instance, int version) { \
		cat_log_debug("anchoring:%p", instance); \
		return (tp *) cat_wo_anchor((CatWo *) instance, version); \
	} \
	tp *pre##_get_original(tp *instance) { \
		return (tp *) cat_wo_get_original((CatWo *) instance); \
	} \
	gboolean pre##_is_anchored(const tp *instance) { \
		return cat_wo_is_anchored((const CatWo *) instance); \
	} \
	gboolean pre##_equal(const tp *instance_a, const tp *instance_b) { \
		return cat_wo_equal((const CatWo *) instance_a, (const CatWo *) instance_b); \
	} \
	tp *pre##_clone(const tp *source, CatCloneDepth clone_depth) { \
		return (tp *) cat_wo_clone((const CatWo *) source, clone_depth); \
	} \


G_END_DECLS

#endif /* CATWO_H_ */
