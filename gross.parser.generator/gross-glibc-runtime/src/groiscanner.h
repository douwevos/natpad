#ifndef GROISCANNER_H_
#define GROISCANNER_H_

#include "grotoken.h"

G_BEGIN_DECLS

#define GRO_TYPE_ISCANNER                 (gro_iscanner_get_type ())
#define GRO_ISCANNER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRO_TYPE_ISCANNER, GroIScanner))
#define GRO_IS_ISCANNER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_ISCANNER))
#define GRO_ISCANNER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GRO_TYPE_ISCANNER, GroIScannerInterface))


typedef struct _GroIScanner               GroIScanner;
typedef struct _GroIScannerInterface      GroIScannerInterface;

struct _GroIScannerInterface {
  GTypeInterface parent_iface;

  GroToken *(*next_token) (GroIScanner *self);
  
  GroToken *(*createToken)(GroIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);
  
};

GType gro_iscanner_get_type (void);

GroToken *gro_iscanner_next_token(GroIScanner *scanner);
GroToken *gro_iscanner_create_token(GroIScanner *scanner, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);


G_END_DECLS


#endif /* GROISCANNER_H_ */
