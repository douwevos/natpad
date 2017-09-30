/*
   File:    chammappagewo.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Apr 23, 2015
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

#ifndef CHAMMAPPAGEWO_H_
#define CHAMMAPPAGEWO_H_

#include "../chapagewo.h"
#include "../chammap.h"
#include "../../charset/chaiconverter.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_MMAP_PAGE_WO              (cha_mmap_page_wo_get_type())
#define CHA_MMAP_PAGE_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_mmap_page_wo_get_type(), ChaMMapPageWo))
#define CHA_MMAP_PAGE_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_MMAP_PAGE_WO, ChaMMapPageWoClass))
#define CHA_IS_MMAP_PAGE_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_MMAP_PAGE_WO))
#define CHA_IS_MMAP_PAGE_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_MMAP_PAGE_WO))
#define CHA_MMAP_PAGE_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_MMAP_PAGE_WO, ChaMMapPageWoClass))

typedef struct _ChaMMapPageWo               ChaMMapPageWo;
typedef struct _ChaMMapPageWoPrivate        ChaMMapPageWoPrivate;
typedef struct _ChaMMapPageWoClass          ChaMMapPageWoClass;


struct _ChaMMapPageWo {
	ChaPageWo parent;
};

struct _ChaMMapPageWoClass {
	ChaPageWoClass parent_class;
};


GType cha_mmap_page_wo_get_type();

ChaMMapPageWo *cha_mmap_page_wo_new(ChaIConverter *converter, ChaMMap *map, gsize offset, gsize length, short line_count);

CAT_WO_BASE_H(ChaMMapPageWo,cha_mmap_page_wo);

G_END_DECLS

#endif /* CHAMMAPPAGEWO_H_ */
