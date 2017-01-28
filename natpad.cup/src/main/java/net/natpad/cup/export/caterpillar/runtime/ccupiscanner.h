#ifndef [%PRE%]ISCANNER_H_
#define [%PRE%]ISCANNER_H_

#include <caterpillar.h>
#include "[%pre%]token.h"

G_BEGIN_DECLS

#define [%PRE%]_TYPE_ISCANNER                 ([%pre%]_iscanner_get_type ())
#define [%PRE%]_ISCANNER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), [%PRE%]_TYPE_ISCANNER, [%Pre%]IScanner))
#define [%PRE%]_IS_ISCANNER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), [%PRE%]_TYPE_ISCANNER))
#define [%PRE%]_ISCANNER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), [%PRE%]_TYPE_ISCANNER, [%Pre%]IScannerInterface))


typedef struct _[%Pre%]IScanner               [%Pre%]IScanner;
typedef struct _[%Pre%]IScannerInterface      [%Pre%]IScannerInterface;

struct _[%Pre%]IScannerInterface {
  GTypeInterface parent_iface;

  [%Pre%]Token *(*next_token) ([%Pre%]IScanner *self);
  
  [%Pre%]Token *(*createToken)([%Pre%]IScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);
  
};

GType [%pre%]_iscanner_get_type (void);

[%Pre%]Token *[%pre%]_iscanner_next_token([%Pre%]IScanner *scanner);
[%Pre%]Token *[%pre%]_iscanner_create_token([%Pre%]IScanner *scanner, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);


G_END_DECLS


#endif /* [%PRE%]ISCANNER_H_ */
