/*
   File:    jagbytopmonitor.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 4, 2012
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

#ifndef JAGBYTOPMONITOR_H_
#define JAGBYTOPMONITOR_H_

#include "../jagbytabstractmnemonic.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_OP_MONITOR              (jag_byt_op_monitor_get_type())
#define JAG_BYT_OP_MONITOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_op_monitor_get_type(), JagBytOpMonitor))
#define JAG_BYT_OP_MONITOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_OP_MONITOR, JagBytOpMonitorClass))
#define JAG_BYT_IS_OP_MONITOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_OP_MONITOR))
#define JAG_BYT_IS_OP_MONITOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_OP_MONITOR))
#define JAG_BYT_OP_MONITOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_OP_MONITOR, JagBytOpMonitorClass))

typedef struct _JagBytOpMonitor               JagBytOpMonitor;
typedef struct _JagBytOpMonitorClass          JagBytOpMonitorClass;


struct _JagBytOpMonitor {
	JagBytAbstractMnemonic parent;
};

struct _JagBytOpMonitorClass {
	JagBytAbstractMnemonicClass parent_class;
};


GType jag_byt_op_monitor_get_type();

JagBytOpMonitor *jag_byt_op_monitor_new(JagBytOperation operation, int offset);

G_END_DECLS

#endif /* JAGBYTOPMONITOR_H_ */
