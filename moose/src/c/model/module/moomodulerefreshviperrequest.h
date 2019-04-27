#include <caterpillar.h>
#include <worm.h>
#include "../../mooservice.h"

G_BEGIN_DECLS

#define MOO_TYPE_MODULE_REFRESH_VIPER_REQUEST              (moo_module_refresh_viper_request_get_type())
#define MOO_MODULE_REFRESH_VIPER_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_module_refresh_viper_request_get_type(), MooModuleRefreshViperRequest))
#define MOO_MODULE_REFRESH_VIPER_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_MODULE_REFRESH_VIPER_REQUEST, MooModuleRefreshViperRequestClass))
#define MOO_IS_MODULE_REFRESH_VIPER_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_MODULE_REFRESH_VIPER_REQUEST))
#define MOO_IS_MODULE_REFRESH_VIPER_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_MODULE_REFRESH_VIPER_REQUEST))
#define MOO_MODULE_REFRESH_VIPER_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_MODULE_REFRESH_VIPER_REQUEST, MooModuleRefreshViperRequestClass))

typedef struct _MooModuleRefreshViperRequest               MooModuleRefreshViperRequest;
typedef struct _MooModuleRefreshViperRequestPrivate        MooModuleRefreshViperRequestPrivate;
typedef struct _MooModuleRefreshViperRequestClass          MooModuleRefreshViperRequestClass;

struct _MooModuleRefreshViperRequest {
	WorRequest parent;
};

struct _MooModuleRefreshViperRequestClass {
	WorRequestClass parent_class;
};

GType moo_module_refresh_viper_request_get_type();

MooModuleRefreshViperRequest *moo_module_refresh_viper_request_new(MooService *moo_service, long long module_id, gboolean force_refresh);

G_END_DECLS
