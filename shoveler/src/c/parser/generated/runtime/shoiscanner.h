#ifndef SHOISCANNER_H_
#define SHOISCANNER_H_

#include <caterpillar.h>
#include "shotoken.h"

G_BEGIN_DECLS

#define SHO_TYPE_ISCANNER                 (sho_iscanner_get_type ())
#define SHO_ISCANNER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SHO_TYPE_ISCANNER, ShoIScanner))
#define SHO_IS_ISCANNER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_ISCANNER))
#define SHO_ISCANNER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), SHO_TYPE_ISCANNER, ShoIScannerInterface))


typedef struct _ShoIScanner               ShoIScanner;
typedef struct _ShoIScannerInterface      ShoIScannerInterface;

struct _ShoIScannerInterface {
  GTypeInterface parent_iface;

  ShoToken *(*next_token) (ShoIScanner *self);
  
  ShoToken *(*createToken)(ShoIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);
  
};

GType sho_iscanner_get_type (void);

ShoToken *sho_iscanner_next_token(ShoIScanner *scanner);
ShoToken *sho_iscanner_create_token(ShoIScanner *scanner, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);


G_END_DECLS


#endif /* SHOISCANNER_H_ */
