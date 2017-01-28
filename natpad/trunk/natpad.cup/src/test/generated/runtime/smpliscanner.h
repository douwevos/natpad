#ifndef SMPLISCANNER_H_
#define SMPLISCANNER_H_

#include <caterpillar.h>
#include "smpltoken.h"

G_BEGIN_DECLS

#define SMPL_TYPE_ISCANNER                 (smpl_iscanner_get_type ())
#define SMPL_ISCANNER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SMPL_TYPE_ISCANNER, SmplIScanner))
#define SMPL_IS_ISCANNER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SMPL_TYPE_ISCANNER))
#define SMPL_ISCANNER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), SMPL_TYPE_ISCANNER, SmplIScannerInterface))


typedef struct _SmplIScanner               SmplIScanner;
typedef struct _SmplIScannerInterface      SmplIScannerInterface;

struct _SmplIScannerInterface {
  GTypeInterface parent_iface;

  SmplToken *(*next_token) (SmplIScanner *self);
  
  SmplToken *(*createToken)(SmplIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);
  
};

GType smpl_iscanner_get_type (void);

SmplToken *smpl_iscanner_next_token(SmplIScanner *scanner);
SmplToken *smpl_iscanner_create_token(SmplIScanner *scanner, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);


G_END_DECLS


#endif /* SMPLISCANNER_H_ */
