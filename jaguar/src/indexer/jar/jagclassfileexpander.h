/*
   File:    jagclassfileexpander.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 28, 2014
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

#ifndef JAGCLASSFILEEXPANDER_H_
#define JAGCLASSFILEEXPANDER_H_

#include <caterpillar.h>
#include <worm.h>

G_BEGIN_DECLS

#define JAG_TYPE_CLASSFILE_EXPANDER              (jag_classfile_expander_get_type())
#define JAG_CLASSFILE_EXPANDER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_classfile_expander_get_type(), JagClassfileExpander))
#define JAG_CLASSFILE_EXPANDER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_CLASSFILE_EXPANDER, JagClassfileExpanderClass))
#define JAG_IS_CLASSFILE_EXPANDER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_CLASSFILE_EXPANDER))
#define JAG_IS_CLASSFILE_EXPANDER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_CLASSFILE_EXPANDER))
#define JAG_CLASSFILE_EXPANDER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_CLASSFILE_EXPANDER, JagClassfileExpanderClass))


typedef struct _JagClassfileExpander               JagClassfileExpander;
typedef struct _JagClassfileExpanderPrivate        JagClassfileExpanderPrivate;
typedef struct _JagClassfileExpanderClass          JagClassfileExpanderClass;


struct _JagClassfileExpander {
	GObject parent;
};

struct _JagClassfileExpanderClass {
	GObjectClass parent_class;
};


GType jag_classfile_expander_get_type();

JagClassfileExpander *jag_classfile_expander_new(WorQueue *queue);

G_END_DECLS

#endif /* JAGCLASSFILEEXPANDER_H_ */
