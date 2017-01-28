/*
   File:    catbitarraywo.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    May 30, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "catbitarraywo.h"
#include "../catistringable.h"
#include "catstringwo.h"
#include "catwoprivate.h"
#include <string.h>

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatBitArrayWo"
#include "../logging/catlog.h"

struct _CatBitArrayWoPrivate {
	char *bit_array;
	int bit_count;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatBitArrayWo, cat_bit_array_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(CatBitArrayWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static CatWo *l_wo_anchor(CatWo *wo, int version);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);


static void cat_bit_array_wo_class_init(CatBitArrayWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchor = l_wo_anchor;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_bit_array_wo_init(CatBitArrayWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatBitArrayWo *instance = CAT_BIT_ARRAY_WO(object);
	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(instance);

	CatBitArrayWo *original = (CatBitArrayWo *) cat_wo_get_original((CatWo *) instance);
	if (original) {
		CatBitArrayWoPrivate *opriv = cat_bit_array_wo_get_instance_private(original);
		cat_log_detail("opriv->bit_array:%p", opriv->bit_array);
		if (priv->bit_array == opriv->bit_array) {
			priv->bit_array = NULL;
			priv->bit_count = 0;
		}
	}

	cat_log_detail("bit_array:%p", priv->bit_array);
	cat_free_ptr(priv->bit_array);
	G_OBJECT_CLASS(cat_bit_array_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_bit_array_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatBitArrayWo *cat_bit_array_wo_new() {
	CatBitArrayWo *result = g_object_new(CAT_TYPE_BIT_ARRAY_WO, NULL);
	cat_ref_anounce(result);
	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->bit_array = NULL;
	priv->bit_count = 0;
	return result;
}

int cat_bit_array_wo_length(CatBitArrayWo *bit_array) {
	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(bit_array);
	return priv->bit_count;
}

gboolean cat_bit_array_wo_get(CatBitArrayWo *bit_array, int index) {
	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(bit_array);
	if (index<0 || index>=priv->bit_count) {
		return FALSE;
	}
	int bidx = index>>3;
	int mask = 0x80 >> (index & 0x7);
	return (priv->bit_array[bidx] & mask)!=0;
}


#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_bit_array)) { \
			cat_log_error("Object is read only:%o", e_bit_array); \
			return rval; \
		} \

#define MDE \
	CatBitArrayWo *original = (CatBitArrayWo *) cat_wo_get_original((CatWo *) e_bit_array); \
	if (original) { \
		CatBitArrayWoPrivate *opriv = cat_bit_array_wo_get_instance_private(original); \
		if (priv->bit_array==opriv->bit_array) { \
			int bcnt = (priv->bit_count+7)>>3; \
			cat_log_detail("bcnt=%d", bcnt); \
			priv->bit_array = g_malloc(bcnt); \
			memcpy(priv->bit_array, opriv->bit_array, bcnt); \
		} \
	} \


gboolean cat_bit_array_wo_set(CatBitArrayWo *e_bit_array, int index) {
	cat_log_debug("set:index=%d",index);
	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(e_bit_array);
	CHECK_IF_WRITABLE(FALSE);
	if (index<0 || index>=priv->bit_count) {
		return FALSE;
	}
	MDE
	int bidx = index>>3;
	int mask = 0x80 >> (index & 0x7);
	if ((priv->bit_array[bidx] & mask)==0) {
		priv->bit_array[bidx] |= mask;
		return TRUE;
	}
	return FALSE;
}

void cat_bit_array_wo_set_all(CatBitArrayWo *e_bit_array) {
	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(e_bit_array);
	cat_log_debug("set: all");
	CHECK_IF_WRITABLE();
	MDE
	int idx=0;
	int byte_cnt = priv->bit_count>>3;
	for(idx=0; idx<byte_cnt; idx++) {
		priv->bit_array[idx] = 0xFF;
	}
	if (byte_cnt<<3 != priv->bit_count) {
		int mask = 0xFF00 >> (priv->bit_count & 7);
		mask = 0xFF & mask;
		priv->bit_array[byte_cnt] = (char) mask;
	}
}

void cat_bit_array_wo_invert(CatBitArrayWo *e_bit_array) {
	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(e_bit_array);
	cat_log_debug("set: invert");
	CHECK_IF_WRITABLE();
	MDE
	int idx=0;
	int byte_cnt = priv->bit_count>>3;
	for(idx=0; idx<byte_cnt; idx++) {
		priv->bit_array[idx] = 0xFF ^ priv->bit_array[idx];
	}
	if (byte_cnt<<3 != priv->bit_count) {
		int mask = 0xFF00 >> (priv->bit_count & 7);
		mask = 0xFF & mask;
		priv->bit_array[byte_cnt] = (char) (priv->bit_array[byte_cnt] ^ mask);
	}
}

gboolean cat_bit_array_wo_unset(CatBitArrayWo *e_bit_array, int index) {
	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(e_bit_array);
	cat_log_debug("unset:index=%d",index);
	CHECK_IF_WRITABLE(FALSE);
	if (index<0 || index>=priv->bit_count) {
		return FALSE;
	}
	MDE
	int bidx = index>>3;
	int mask = 0x80 >> (index & 0x7);
	if ((priv->bit_array[bidx] & mask)!=0) {
		priv->bit_array[bidx] &= (~mask);
		return TRUE;
	}
	return FALSE;
}

void cat_bit_array_wo_toggle(CatBitArrayWo *e_bit_array, int index) {
	cat_log_debug("toggle:index=%d",index);
	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(e_bit_array);
	CHECK_IF_WRITABLE();
	if (index<0 || index>=priv->bit_count) {
		return;
	}
	MDE

	int bidx = index>>3;
	int mask = 0x80 >> (index & 0x7);
	cat_log_debug("pre=%d", priv->bit_array[bidx]);
	priv->bit_array[bidx] ^= mask;
	cat_log_debug("pst=%d", priv->bit_array[bidx]);
}

void cat_bit_array_wo_remove_range(CatBitArrayWo *e_bit_array, int offset, int length) {
	cat_log_debug("remove-range:offset=%d, length=%d", offset, length);
	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(e_bit_array);
	CHECK_IF_WRITABLE();
	if (offset<0) {
		offset = 0;
	} else if (offset>=priv->bit_count) {
		return;
	}

	if (offset+length>priv->bit_count) {
		length = priv->bit_count-offset;

		CatBitArrayWo *original = (CatBitArrayWo *) cat_wo_get_original((CatWo *) e_bit_array);
		if (original) {
			CatBitArrayWoPrivate *opriv = cat_bit_array_wo_get_instance_private(original);
			if (priv->bit_array==opriv->bit_array) {
				int bcnt = (offset+7)>>3;
				priv->bit_array = g_malloc(bcnt);
				memcpy(priv->bit_array, opriv->bit_array, bcnt);
			}
		}

		priv->bit_count = offset;
		return;
	}
	if (length<=0) {
		return;
	}

	CatBitArrayWo *original = (CatBitArrayWo *) cat_wo_get_original((CatWo *) e_bit_array);
	if (original) {
		CatBitArrayWoPrivate *opriv = cat_bit_array_wo_get_instance_private(original);
		if (priv->bit_array==opriv->bit_array) {
			int bcnt = (priv->bit_count+7)>>3;
			priv->bit_array = g_malloc(bcnt);
			memcpy(priv->bit_array, opriv->bit_array, bcnt);
		}
	}

	int bits_in = 0;
	int bits_in_count = 0;
	int out_idx = offset>>3;
	int s = offset&7;
	if (s>0) {
		bits_in = priv->bit_array[out_idx];
		bits_in = bits_in>>(8-s);
		bits_in_count+=s;
	}

	gboolean last_data_read = FALSE;
	int in_end_idx = (priv->bit_count+7)>>3;
	int in_idx =  offset+length;
	int shift_cnt = in_idx&7;
	in_idx = in_idx>>3;
	int shift_data = priv->bit_array[in_idx++];
	int mask = (0x100 >> shift_cnt)-1;
	shift_data &= mask;
	shift_cnt = (8-shift_cnt);
	if (in_idx >= in_end_idx) {
		int rest = 8-(priv->bit_count&7);
		shift_cnt-=rest;
		mask>>=rest;
		shift_data>>=rest;
		last_data_read = TRUE;
	}
	if (shift_cnt>0) {
		bits_in = (bits_in<<shift_cnt) | shift_data;
		bits_in_count += shift_cnt;
	}

	while(TRUE) {
		if (bits_in_count>=8) {
			int exp = bits_in>>(bits_in_count-8);
			priv->bit_array[out_idx++] = (char) exp;
			bits_in_count-=8;
		} else if (last_data_read) {
			if (bits_in_count>0) {
				bits_in = bits_in<<(8-bits_in_count);
				priv->bit_array[out_idx++] = (char) bits_in;
			}
			break;
		} else {
			shift_data = priv->bit_array[in_idx++];
			mask = 0xFF;
			shift_cnt = 8;
			shift_data &= mask;
			if (in_idx >= in_end_idx) {
				int rest = 8-(priv->bit_count&7);
				shift_cnt-=rest;
				mask>>=rest;
				shift_data>>=rest;
				last_data_read = TRUE;
			}
			if (shift_cnt>0) {
				bits_in = (bits_in<<shift_cnt) | shift_data;
				bits_in_count += shift_cnt;
			}
		}
	}

	priv->bit_count = priv->bit_count-length;
}

void cat_bit_array_wo_insert(CatBitArrayWo *e_bit_array, int offset, int length) {
	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(e_bit_array);
	CHECK_IF_WRITABLE();
	if (offset<0) {
		offset = 0;
	} else if (offset>=priv->bit_count) {
		offset = priv->bit_count;
	}
	if (length <= 0) {
		return;
	}

	int new_length = priv->bit_count+length;
	int new_bc = (new_length+7)>>3;
	char *new_data = g_malloc0(new_bc);

	int offset_in = offset;
	int byte_in = (offset_in+7)>>3;
	memcpy(new_data, priv->bit_array, byte_in);
	int shift_cnt = offset_in & 0x7;
	int mask;
	if (shift_cnt!=0) {
		byte_in--;
		mask = (0xFF00 >> shift_cnt) & 0xFF;
		new_data[byte_in] = new_data[byte_in] & mask;
	}


	int bits_out = offset+length;
	int next_bits_out = (bits_out +8) & (~7);
	if (next_bits_out>new_length) {
		next_bits_out = new_length;
	}
	int bits_in = offset;
	int next_bits_in = (bits_in +8) & (~7);
	if (next_bits_in>priv->bit_count) {
		next_bits_in=priv->bit_count;
	}

	int bits_cached = 0;
	int bits_data = 0;
	gboolean last_bits_read = FALSE;
	while(TRUE) {
		cat_log_trace("bits_in=%d, next_in=%d, bits_out=%d, next_out=%d, bits_cached=%d, last_bits_read=%d", bits_in, next_bits_in, bits_out, next_bits_out, bits_cached, last_bits_read);
		if (bits_cached>8 || last_bits_read) {
			/* store bits_cached */
			int bits_to_store = next_bits_out-bits_out;
			cat_log_trace("bits_to_store=%d", bits_to_store);
			if ((bits_to_store==8) && ((bits_out&7)==0)) {
				/* store 8 bits in byte mode */
				bits_cached-=8;
				int s = bits_data >> bits_cached;
				cat_log_trace("store byte mode: bits_cached=%d, s=%d", bits_cached, s);
				new_data[bits_out>>3] = (char) s;
				bits_out = next_bits_out;
				next_bits_out+=8;
			} else if (bits_to_store>0) {
				/* shift data */
				bits_cached-=bits_to_store;
				int s = bits_data >> bits_cached;
				cat_log_trace("store shift mode: bits_cached=%d, s=%d", bits_cached, s);
				int mask = (0x100 >> (8-bits_to_store))-1;
				int nn_bits_out = (next_bits_out) & (~7);
				cat_log_trace("mask=%d, nn_bits_out=%d", mask, nn_bits_out);
				if (nn_bits_out!=next_bits_out) {
					nn_bits_out += 8;
					int shift_back = nn_bits_out-next_bits_out;
					mask = mask<<shift_back;
					s = s<<shift_back;
				} else {
					nn_bits_out += 8;
				}
				int byte_out = bits_out>>3;
				int r = new_data[byte_out];
				cat_log_trace("byte_out=%d, r=%d", byte_out, r);
				r = (r&(~mask)) | (s&mask);
				new_data[byte_out] = r;
				bits_out = next_bits_out;
				next_bits_out = nn_bits_out;
			}
			if (next_bits_out>new_length) {
				next_bits_out = new_length;
			}
			if (last_bits_read && bits_cached==0) {
				break;
			}
		} else {
			int bits_to_load = next_bits_in-bits_in;
			if ((bits_to_load==8) && ((bits_in&0x7)==0)) {
				/* load 8 bits in byte mode */
				int r = 0xFF & (priv->bit_array[bits_in>>3]);
				bits_cached+=8;
				bits_data = (bits_data<<8) | r;
				bits_in+=8;
			} else if (bits_to_load>0) {
				/* load shifted data */
				int fh = (bits_in & 7);
				int mask = ((0x100) >> (fh))-1;
				fh = 8-fh;
				int r = mask & (priv->bit_array[bits_in>>3]);
				if (fh>bits_to_load) {
					fh = fh-bits_to_load;
					r = r>>fh;
				}
				bits_data = (bits_data<<bits_to_load) | r;
				bits_cached+=bits_to_load;
				bits_in = next_bits_in;
				next_bits_in = (next_bits_in+8) & (~7);
			}
			if (bits_in==priv->bit_count) {
				last_bits_read = TRUE;
			}
			if (next_bits_in>priv->bit_count) {
				next_bits_in=priv->bit_count;
			}
		}
	}


	CatBitArrayWo *original = (CatBitArrayWo *) cat_wo_get_original((CatWo *) e_bit_array);
	if (original) {
		CatBitArrayWoPrivate *opriv = cat_bit_array_wo_get_instance_private(original);
		if (priv->bit_array!=opriv->bit_array) {
			cat_free_ptr(priv->bit_array);
		}
	} else {
		cat_free_ptr(priv->bit_array);
	}
	priv->bit_array = new_data;
	priv->bit_count = new_length;
}

gboolean cat_bit_array_wo_has_at_least_one_set(const CatBitArrayWo *bit_array) {
	const CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private((CatBitArrayWo *) bit_array);
	if (priv->bit_count==0) {
		return FALSE;
	}
	int byte_count = ((priv->bit_count+7)>>3)-1;
	int byte_idx;
	for(byte_idx=0; byte_idx<byte_count; byte_idx++) {
		if (priv->bit_array[byte_idx]!=0) {
			return TRUE;
		}
	}
	int shift = priv->bit_count & 0x7;
	int mask = 0x100>>shift;
	mask--;
	mask = ~mask;
	return (priv->bit_array[byte_count] & mask)!=0;
}


static CatWo *l_wo_anchor(CatWo *wo, int version) {
	cat_ref_ptr(wo);
	CatWo *result = CAT_WO_CLASS(cat_bit_array_wo_parent_class)->anchor(wo, version);
	if (result!=wo) {
		CatBitArrayWoPrivate *opriv = cat_bit_array_wo_get_instance_private((CatBitArrayWo *) result);
		CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private((CatBitArrayWo *) wo);
		if (priv->bit_array==opriv->bit_array) {
			priv->bit_array = NULL;
			priv->bit_count = 0;
		}
	}
	cat_unref_ptr(wo);
	return result;
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CAT_TYPE_BIT_ARRAY_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private((CatBitArrayWo *) e_uninitialized);
	if (original) {
		CatBitArrayWoPrivate *rpriv = cat_bit_array_wo_get_instance_private((CatBitArrayWo *) original);
		priv->bit_count = rpriv->bit_count;
		priv->bit_array = rpriv->bit_array;
	} else {
		priv->bit_count = 0;
		priv->bit_array = NULL;
	}
	return CAT_WO_CLASS(cat_bit_array_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}



//
//static gboolean l_check_for_modifications(CatWo *wo) {
//	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private((CatBitArrayWo *) wo);
//	gboolean was_modified = TRUE;
////	CatBitArrayWoInfo *info_name = (CatBitArrayWoInfo *) cat_wo_get_info(wo);
//	CatBitArrayWo *original = (CatBitArrayWo *) cat_wo_get_original(wo);
//	cat_log_debug("check for mods:orginal=%p", original);
//	if (original) {
//		CatBitArrayWoPrivate *opriv = cat_bit_array_wo_get_instance_private(original);
//		if (priv->bit_count==opriv->bit_count) {
//			cat_log_debug("priv->bit_count=%d", priv->bit_count);
//			if (priv->bit_count==0 || priv->bit_array==opriv->bit_array) {
//				was_modified = FALSE;
//				cat_log_debug("same array or 0");
//			} else {
//				int byte_count = (priv->bit_count+7)>>3;
//				cat_log_debug("byte_cnt=%d", byte_count);
//				if (byte_count>1) {
//					if (memcmp(priv->bit_array,opriv->bit_array, byte_count-1) == 0) {
//						was_modified = FALSE;
//					}
//				} else {
//					was_modified = FALSE;
//				}
//				if (!was_modified) {
//					byte_count--;
//					char cha = priv->bit_array[byte_count];
//					char chb = opriv->bit_array[byte_count];
//					int shift = priv->bit_count & 0x7;
//					int mask = 0x100>>shift;
//					mask--;
//					mask = ~mask;
//					cat_log_debug("last_idx=%d, mask=%d, cha=%d, chb=%d", byte_count, mask, cha, chb);
//					was_modified = (cha & mask)!=(chb & mask);
//				}
//				if (!was_modified && opriv->bit_array!=NULL) {
//					cat_free_ptr(priv->bit_array);
//					priv->bit_array = opriv->bit_array;
//				}
//			}
//		}
//	}
//
//	return was_modified;
//}

static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const CatBitArrayWoPrivate *priv_a = cat_bit_array_wo_get_instance_private((CatBitArrayWo *) wo_a);
	const CatBitArrayWoPrivate *priv_b = cat_bit_array_wo_get_instance_private((CatBitArrayWo *) wo_b);
	if (priv_a->bit_count==priv_b->bit_count) {
		if (priv_a->bit_count==0 || priv_a->bit_array==priv_b->bit_array) {
			return TRUE;
		} else {
			int byte_count = (priv_a->bit_count+7)>>3;
			if (byte_count>1) {
				if (memcmp(priv_a->bit_array, priv_b->bit_array, byte_count-1) != 0) {
					return FALSE;
				}
			}
			byte_count--;
			char cha = priv_a->bit_array[byte_count];
			char chb = priv_b->bit_array[byte_count];
			int shift = priv_a->bit_count & 0x7;
			int mask = 0x100>>shift;
			mask--;
			mask = ~mask;
			return (cha & mask)==(chb & mask);
		}
	}
	return FALSE;
}

//static CatWo *l_clone(const CatWo *wo_source, CatCloneDepth clone_depth) {
//	if (wo_source == NULL) {
//		return NULL;
//	} else if (clone_depth==CAT_CLONE_DEPTH_NONE && cat_wo_is_anchored(wo_source)) {
//		return cat_ref_ptr((CatWo *) wo_source);
//	}
//	if (clone_depth==CAT_CLONE_DEPTH_MAIN) {
//		clone_depth = CAT_CLONE_DEPTH_NONE;
//	}
//	const CatBitArrayWoPrivate *spriv = cat_bit_array_wo_get_instance_private((CatBitArrayWo *) wo_source);
//	CatBitArrayWo *result = cat_bit_array_wo_new();
//	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(result);
//	priv->bit_array = spriv->bit_array;
//	priv->bit_count = spriv->bit_count;
//	return (CatWo *) result;
//}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CAT_TYPE_BIT_ARRAY_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(CAT_BIT_ARRAY_WO(e_uninitialized));
	if (wo_source) {
		CatBitArrayWoPrivate *priv_src = cat_bit_array_wo_get_instance_private(CAT_BIT_ARRAY_WO(wo_source));
		struct CatWoInfo *info = (struct CatWoInfo *) cat_wo_get_info((CatWo *) wo_source);
		if ((info!=NULL) && (info->original==wo_source)) {
			priv->bit_array = priv_src->bit_array;
		} else if (priv_src->bit_count>0) {
			int bcnt = (priv_src->bit_count+7)>>3;
			priv->bit_array = g_malloc(bcnt);
			memcpy(priv->bit_array, priv_src->bit_array, bcnt);
		} else {
			priv->bit_array = NULL;
		}
		priv->bit_count = priv_src->bit_count;
	} else {
		priv->bit_array = NULL;
		priv->bit_count = 0;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cat_bit_array_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


CAT_WO_BASE_C(CatBitArrayWo,cat_bit_array_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatBitArrayWo *instance = CAT_BIT_ARRAY_WO(self);
	CatBitArrayWoPrivate *priv = cat_bit_array_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: %s org=%p, count=%d,data=%p ", iname, self, cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", cat_wo_get_original((CatWo *) instance), priv->bit_count, priv->bit_array);
	if (priv->bit_count>0) {
		int s = priv->bit_array[0];
		cat_string_wo_append_hexadecimal(append_to, s, 2);
	}
	cat_string_wo_append_char(append_to, ']');
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
