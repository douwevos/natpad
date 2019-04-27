/*
   File:    leakey.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    May 13, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "leakey.h"
#include <caterpillar.h>
#include <string.h>
#include <gdk/gdkkeysyms.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaKey"
#include <logging/catlog.h>

CatS lea_s_key_left_alt = CAT_S_DEF("LeftAlt");
CatS lea_s_key_right_alt = CAT_S_DEF("RightAlt");

CatS lea_s_key_modifier_up = CAT_S_DEF("up");
CatS lea_s_key_modifier_down = CAT_S_DEF("down");


CatS lea_s_key_modifier_shift = CAT_S_DEF("Shift");
CatS lea_s_key_modifier_control = CAT_S_DEF("Ctrl");
CatS lea_s_key_modifier_alt = CAT_S_DEF("Alt");
CatS lea_s_key_modifier_super = CAT_S_DEF("Super");

CatS lea_s_key_delete = CAT_S_DEF("Delete");
CatS lea_s_key_insert = CAT_S_DEF("Insert");
CatS lea_s_key_enter = CAT_S_DEF("Enter");
CatS lea_s_key_space = CAT_S_DEF("Space");
CatS lea_s_key_tab = CAT_S_DEF("Tab");
CatS lea_s_key_left_tab = CAT_S_DEF("LeftTab");
CatS lea_s_key_escape = CAT_S_DEF("Esc");

CatS lea_s_key_home = CAT_S_DEF("Home");
CatS lea_s_key_end = CAT_S_DEF("End");

CatS lea_s_key_page_up = CAT_S_DEF("PgUp");
CatS lea_s_key_page_down = CAT_S_DEF("PgDown");

CatS lea_s_key_left = CAT_S_DEF("Left");
CatS lea_s_key_right = CAT_S_DEF("Right");
CatS lea_s_key_up = CAT_S_DEF("Up");
CatS lea_s_key_down = CAT_S_DEF("Down");


CatS lea_s_key_f1 = CAT_S_DEF("F1");
CatS lea_s_key_f2 = CAT_S_DEF("F2");
CatS lea_s_key_f3 = CAT_S_DEF("F3");
CatS lea_s_key_f4 = CAT_S_DEF("F4");
CatS lea_s_key_f5 = CAT_S_DEF("F5");
CatS lea_s_key_f6 = CAT_S_DEF("F6");
CatS lea_s_key_f7 = CAT_S_DEF("F7");
CatS lea_s_key_f8 = CAT_S_DEF("F8");
CatS lea_s_key_f9 = CAT_S_DEF("F9");
CatS lea_s_key_f10 = CAT_S_DEF("F10");
CatS lea_s_key_f11 = CAT_S_DEF("F11");
CatS lea_s_key_f12 = CAT_S_DEF("F12");
CatS lea_s_key_f13 = CAT_S_DEF("F13");
CatS lea_s_key_f14 = CAT_S_DEF("F14");
CatS lea_s_key_f15 = CAT_S_DEF("F15");
CatS lea_s_key_f16 = CAT_S_DEF("F16");


struct _LeaKeyPrivate {
	int hash_code;
	int key;
	int modifiers;
	CatStringWo *a_text;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaKey, lea_key, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(LeaKey)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);

static void lea_key_class_init(LeaKeyClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void lea_key_init(LeaKey *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaKey *instance = LEA_KEY(object);
	LeaKeyPrivate *priv = lea_key_get_instance_private(instance);
	cat_unref_ptr(priv->a_text);
	cat_log_detail("disposed:%p", object);
}

LeaKey *lea_key_new(int key, int modifiers) {
	LeaKey *result = g_object_new(LEA_TYPE_KEY, NULL);
	if (key>='a' && key<='z') {
		key = key - 'a' + 'A';
	}
	cat_log_debug("key=%d, modifiers=%d", key, modifiers);
	LeaKeyPrivate *priv = lea_key_get_instance_private(result);
	priv->key = key;
	priv->modifiers = modifiers;
	priv->a_text = NULL;
	return result;
}


LeaKey *lea_key_from_gkd_event(GdkEventKey *event, gboolean is_pressed) {
	int key = event->keyval;
	int modifiers = 0;
	const int state = event->state;
	if (state & GDK_SHIFT_MASK) {
		modifiers += LEA_KM_SHIFT;
	}
	if (state & GDK_CONTROL_MASK) {
		modifiers += LEA_KM_CONTROL;
	}
	if (state & GDK_MOD1_MASK) {
		modifiers += LEA_KM_ALT;
	}
	if (state & GDK_SUPER_MASK) {
		modifiers += LEA_KM_SUPER;
	}
	return lea_key_new(key, modifiers);
}


static int l_modifier_from_string(CatStringWo *a_maybe_modifier) {
	CatStringWo *e_lt = cat_string_wo_clone(a_maybe_modifier, CAT_CLONE_DEPTH_MAIN);
	cat_string_wo_to_lowercase(e_lt);
	const char *t = cat_string_wo_getchars(e_lt);
	cat_log_detail("t=%s", t);

	int result = 0;
	if (strcmp(t, "shift")==0) {
		result = LEA_KM_SHIFT;
	} else if (strcmp(t, "shift:down")==0) {
		result = LEA_KM_SHIFT_DOWN;
	} else if (strcmp(t, "shift:up")==0) {
		result = LEA_KM_SHIFT_UP;
	} else if (strcmp(t, "ctrl")==0) {
		result = LEA_KM_CONTROL;
	} else if (strcmp(t, "ctrl:down")==0) {
		result = LEA_KM_CONTROL_DOWN;
	} else if (strcmp(t, "ctrl:up")==0) {
		result = LEA_KM_CONTROL_UP;
	} else if (strcmp(t, "alt")==0) {
		result = LEA_KM_ALT;
	} else if (strcmp(t, "alt:down")==0) {
		result = LEA_KM_ALT_DOWN;
	} else if (strcmp(t, "alt:up")==0) {
		result = LEA_KM_ALT_UP;
	} else if (strcmp(t, "super")==0) {
		result = LEA_KM_SUPER;
	} else if (strcmp(t, "super:down")==0) {
		result = LEA_KM_SUPER_DOWN;
	} else if (strcmp(t, "super:up")==0) {
		result = LEA_KM_SUPER_UP;
	}
	cat_unref_ptr(e_lt);
	return result;
}

static int l_key_from_string(CatStringWo *a_maybe_modifier) {

	if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_left_alt))==0) {
		return GDK_KEY_Alt_L;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_right_alt))==0) {
		return GDK_KEY_Alt_R;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_delete))==0) {
		return GDK_KEY_Delete;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_insert))==0) {
		return GDK_KEY_Insert;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_enter))==0) {
		return GDK_KEY_Return;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_space))==0) {
		return GDK_KEY_space;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_tab))==0) {
		return GDK_KEY_Tab;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_left_tab))==0) {
		return GDK_KEY_ISO_Left_Tab;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_escape))==0) {
		return GDK_KEY_Escape;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_f1))==0) {
		return GDK_KEY_F1;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_f2))==0) {
		return GDK_KEY_F2;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_f3))==0) {
		return GDK_KEY_F3;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_f4))==0) {
		return GDK_KEY_F4;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_f5))==0) {
		return GDK_KEY_F5;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_f6))==0) {
		return GDK_KEY_F6;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_f7))==0) {
		return GDK_KEY_F7;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_f8))==0) {
		return GDK_KEY_F8;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_f9))==0) {
		return GDK_KEY_F9;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_home))==0) {
		return GDK_KEY_Home;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_end))==0) {
		return GDK_KEY_End;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_page_up))==0) {
		return GDK_KEY_Page_Up;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_page_down))==0) {
		return GDK_KEY_Page_Down;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_left))==0) {
		return GDK_KEY_Left;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_right))==0) {
		return GDK_KEY_Right;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_up))==0) {
		return GDK_KEY_Up;
	} else if (cat_string_wo_case_compare(a_maybe_modifier, CAT_S(lea_s_key_down))==0) {
		return GDK_KEY_Down;
	}
	CatStringWo *e_lt = cat_string_wo_clone(a_maybe_modifier, CAT_CLONE_DEPTH_MAIN);
	cat_string_wo_to_uppercase(e_lt);

	int result = -1;
	if (cat_string_wo_length(e_lt)==1) {
		result = cat_string_wo_char_at(e_lt, 0);
	}
	cat_unref_ptr(e_lt);
	return result;
}

LeaKey *lea_key_from_string(CatStringWo *a_txt) {
	LeaKey *result = NULL;
	int key = 0;
	int modifiers = 0;
	// TODO can't handle Ctrl++ or Ctrl+'+'
	CatArrayWo *e_splitted = cat_string_wo_split(a_txt, '+', FALSE);
	if (e_splitted) {
		CatIIterator *iter = cat_array_wo_iterator(e_splitted);
		while(cat_iiterator_has_next(iter)) {
			CatStringWo *a_single_txt = (CatStringWo *) cat_iiterator_next(iter);
			cat_log_debug("single_txt=%o", a_single_txt);
			int m = l_modifier_from_string(a_single_txt);
			cat_log_detail("modifier=%d", m);
			if (m!=0) {
				modifiers |= m;
			} else {
				m = l_key_from_string(a_single_txt);
				cat_log_detail("key=%d", m);
				if (m>=0) {
					if (key>0) {
						cat_log_warn("double char-code defined key:%o part:%o", a_txt, a_single_txt);
					}
					key = m;
				} else {
					cat_log_error("cannot parse key:%o part:%o", a_txt, a_single_txt);
				}
			}
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(e_splitted);
		if (key!=0 || modifiers!=0) {
			result = lea_key_new(key, modifiers);
		}
	}
	return result;
}


CatStringWo *lea_key_get_key_text(LeaKey *key) {
	LeaKeyPrivate *priv = lea_key_get_instance_private(key);
	if (priv->a_text==NULL) {
		switch(priv->key) {
			case GDK_KEY_Alt_L : {
				priv->a_text = CAT_S(lea_s_key_left_alt);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Alt_R : {
				priv->a_text = CAT_S(lea_s_key_right_alt);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Delete : {
				priv->a_text = CAT_S(lea_s_key_delete);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Insert : {
				priv->a_text = CAT_S(lea_s_key_insert);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Return : {
				priv->a_text = CAT_S(lea_s_key_enter);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_space : {
				priv->a_text = CAT_S(lea_s_key_space);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Tab : {
				priv->a_text = CAT_S(lea_s_key_tab);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_ISO_Left_Tab : {
				priv->a_text = CAT_S(lea_s_key_left_tab);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Escape : {
				priv->a_text = CAT_S(lea_s_key_escape);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_F1 : {
				priv->a_text = CAT_S(lea_s_key_f1);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_F2 : {
				priv->a_text = CAT_S(lea_s_key_f2);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_F3 : {
				priv->a_text = CAT_S(lea_s_key_f3);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_F4 : {
				priv->a_text = CAT_S(lea_s_key_f4);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_F5 : {
				priv->a_text = CAT_S(lea_s_key_f5);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_F6 : {
				priv->a_text = CAT_S(lea_s_key_f6);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_F7 : {
				priv->a_text = CAT_S(lea_s_key_f7);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_F8 : {
				priv->a_text = CAT_S(lea_s_key_f8);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_F9 : {
				priv->a_text = CAT_S(lea_s_key_f9);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Home : {
				priv->a_text = CAT_S(lea_s_key_home);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_End : {
				priv->a_text = CAT_S(lea_s_key_end);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Page_Up : {
				priv->a_text = CAT_S(lea_s_key_page_up);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Page_Down : {
				priv->a_text = CAT_S(lea_s_key_page_down);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Left : {
				priv->a_text = CAT_S(lea_s_key_left);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Right : {
				priv->a_text = CAT_S(lea_s_key_right);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Up : {
				priv->a_text = CAT_S(lea_s_key_up);
				cat_ref_ptr(priv->a_text);
			} break;
			case GDK_KEY_Down : {
				priv->a_text = CAT_S(lea_s_key_down);
				cat_ref_ptr(priv->a_text);
			} break;
			default : {
				CatStringWo *t = cat_string_wo_new_with("");
				gunichar tr = gdk_unicode_to_keyval(priv->key);
				if (tr>='a' && tr<='z') {
					tr=tr-'a'+'A';
				}
				cat_string_wo_append_unichar(t, tr);
				priv->a_text = cat_string_wo_anchor(t, 0);
			} break;
		}
	}
	return priv->a_text;
}


static void l_append_key_string(CatStringWo *e_text, CatStringWo *a_to_append) {
	if (cat_string_wo_length(e_text)!=0) {
		cat_string_wo_append_char(e_text, '+');
	}
	cat_string_wo_append(e_text,a_to_append);
}

CatStringWo *lea_key_to_string(LeaKey *key) {
	LeaKeyPrivate *priv = lea_key_get_instance_private(key);
	int modifiers = priv->modifiers;
	CatStringWo *e_result = cat_string_wo_new();
	if (modifiers & (LEA_KM_SHIFT|LEA_KM_SHIFT_DOWN|LEA_KM_SHIFT_UP)) {
		l_append_key_string(e_result, CAT_S(lea_s_key_modifier_shift));
		if (modifiers & LEA_KM_SHIFT_DOWN) {
			l_append_key_string(e_result, CAT_S(lea_s_key_modifier_down));
		}
		if (modifiers & LEA_KM_SHIFT_UP) {
			l_append_key_string(e_result, CAT_S(lea_s_key_modifier_up));
		}
	}
	if (modifiers & (LEA_KM_CONTROL|LEA_KM_CONTROL_DOWN|LEA_KM_CONTROL_UP)) {
		l_append_key_string(e_result, CAT_S(lea_s_key_modifier_control));
		if (modifiers & LEA_KM_CONTROL_DOWN) {
			l_append_key_string(e_result, CAT_S(lea_s_key_modifier_down));
		}
		if (modifiers & LEA_KM_CONTROL_UP) {
			l_append_key_string(e_result, CAT_S(lea_s_key_modifier_up));
		}
	}
	if (modifiers & (LEA_KM_ALT|LEA_KM_ALT_DOWN|LEA_KM_ALT_UP)) {
		l_append_key_string(e_result, CAT_S(lea_s_key_modifier_alt));
		if (modifiers & LEA_KM_ALT_DOWN) {
			l_append_key_string(e_result, CAT_S(lea_s_key_modifier_down));
		}
		if (modifiers & LEA_KM_ALT_UP) {
			l_append_key_string(e_result, CAT_S(lea_s_key_modifier_up));
		}
	}
	if (modifiers & (LEA_KM_SUPER|LEA_KM_SUPER_DOWN|LEA_KM_SUPER_UP)) {
		l_append_key_string(e_result, CAT_S(lea_s_key_modifier_super));
		if (modifiers & LEA_KM_SUPER_DOWN) {
			l_append_key_string(e_result, CAT_S(lea_s_key_modifier_down));
		}
		if (modifiers & LEA_KM_SUPER_UP) {
			l_append_key_string(e_result, CAT_S(lea_s_key_modifier_up));
		}
	}
	l_append_key_string(e_result, lea_key_get_key_text(key));
	return cat_string_wo_anchor(e_result, 0);
}

gboolean lea_key_equal(LeaKey *key_a, LeaKey *key_b) {

	if (key_a==key_b) {
		return TRUE;
	}
	if (key_a==NULL || key_b==NULL) {
		return FALSE;
	}
	LeaKeyPrivate *priv_a = lea_key_get_instance_private(key_a);
	LeaKeyPrivate *priv_b = lea_key_get_instance_private(key_b);
	return (priv_a->key==priv_b->key) && (priv_a->modifiers==priv_b->modifiers);
}

int lea_key_hashcode(LeaKey *key) {
	LeaKeyPrivate *priv = lea_key_get_instance_private(key);
	cat_log_detail("key=%d, modifiers=%d", priv->key, priv->modifiers);
	if (priv->hash_code==0) {
		priv->hash_code = priv->key + priv->modifiers*3;
		if (priv->hash_code==0) {
			priv->hash_code++;
		}
	}
	return priv->hash_code;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaKey *instance = LEA_KEY(self);
	LeaKeyPrivate *priv = lea_key_get_instance_private(instance);
	CatStringWo *e_rs = cat_string_wo_new();
	if (priv->modifiers & LEA_KM_SHIFT) {
		l_append_key_string(e_rs, CAT_S(lea_s_key_modifier_shift));
	}
	if (priv->modifiers & LEA_KM_CONTROL) {
		l_append_key_string(e_rs, CAT_S(lea_s_key_modifier_control));
	}
	if (priv->modifiers & LEA_KM_ALT) {
		l_append_key_string(e_rs, CAT_S(lea_s_key_modifier_alt));
	}
	if (priv->modifiers & LEA_KM_SUPER) {
		l_append_key_string(e_rs, CAT_S(lea_s_key_modifier_super));
	}
	l_append_key_string(e_rs, lea_key_get_key_text(instance));
	cat_string_wo_append(append_to, e_rs);
	cat_unref_ptr(e_rs);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
