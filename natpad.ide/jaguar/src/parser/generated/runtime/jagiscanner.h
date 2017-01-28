#ifndef JAGISCANNER_H_
#define JAGISCANNER_H_

#include <caterpillar.h>
#include "jagtoken.h"

G_BEGIN_DECLS

#define JAG_TYPE_ISCANNER                 (jag_iscanner_get_type ())
#define JAG_ISCANNER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), JAG_TYPE_ISCANNER, JagIScanner))
#define JAG_IS_ISCANNER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_ISCANNER))
#define JAG_ISCANNER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), JAG_TYPE_ISCANNER, JagIScannerInterface))


typedef struct _JagIScanner               JagIScanner;
typedef struct _JagIScannerInterface      JagIScannerInterface;

struct _JagIScannerInterface {
  GTypeInterface parent_iface;

  JagToken *(*next_token) (JagIScanner *self);
  
  JagToken *(*createToken)(JagIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);
  
};

GType jag_iscanner_get_type (void);

JagToken *jag_iscanner_next_token(JagIScanner *scanner);
JagToken *jag_iscanner_create_token(JagIScanner *scanner, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);


G_END_DECLS


#endif /* JAGISCANNER_H_ */
