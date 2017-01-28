/*
 * catimonitored.h
 *
 *  Created on: Jul 2, 2011
 *      Author: superman
 */

#ifndef CATIMONITORED_H_
#define CATIMONITORED_H_

#include <glib-object.h>

#define CAT_TYPE_IMONITORED                 (cat_imonitored_get_type ())
#define CAT_IMONITORED(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), CAT_TYPE_IMONITORED, CatIMonitored))
#define CAT_IS_IMONITORED(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_IMONITORED))
#define CAT_IMONITORED_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), CAT_TYPE_IMONITORED, CatIMonitoredInterface))


typedef struct _CatIMonitored               CatIMonitored; /* dummy object */
typedef struct _CatIMonitoredInterface      CatIMonitoredInterface;

struct _CatIMonitoredInterface {
  GTypeInterface parent_iface;

};

GType cat_imonitored_get_type(void);


#endif /* CATIMONITORED_H_ */
