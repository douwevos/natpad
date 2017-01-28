/*
 * catimonitored.c
 *
 *  Created on: Jul 2, 2011
 *      Author: superman
 */

#include "catimonitored.h"

static void cat_imonitored_interface_init (gpointer klass) {
//	CatIMonitoredInterface *iface = (CatIMonitoredInterface*) klass;
}


GType cat_imonitored_get_type (void) {
	static volatile gsize g_define_type_id__volatile = 0;
	if (g_once_init_enter (&g_define_type_id__volatile)) {
		static const GTypeInfo info = {
			sizeof (CatIMonitoredInterface),
			cat_imonitored_interface_init,   /* base_init */
			NULL,   /* base_finalize */
		};

		GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE, g_intern_static_string ("CatIMonitored"), &info,0);
		g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
	}
	return g_define_type_id__volatile;
}
