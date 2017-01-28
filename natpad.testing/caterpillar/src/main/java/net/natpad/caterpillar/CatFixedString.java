package net.natpad.caterpillar;

import java.util.Arrays;

public class CatFixedString {

	
	private static final CatFixedString emptyString = new CatFixedString("");
	
	protected char data[];
	protected int size;
	protected int hash;
	
	
	public CatFixedString(String txt) {
		char[] charArray = txt.toCharArray();
		data = new char[charArray.length+1];
		System.arraycopy(charArray, 0, data, 0, charArray.length);
		data[charArray.length] = 0;
		size = charArray.length;
		hash = 0;
	}
	
	private CatFixedString() {
	}
	
	
	public static CatFixedString cat_fixed_string_new(String txt) {
		return new CatFixedString(txt);
	}
	
//	
//
//CatFixedString *cat_fixed_string_convert(gpointer string, gboolean allow_reuse) {
//	if (CAT_IS_STRING(string)) {
//		if (allow_reuse && g_object_is_floating(string)) {
//			CatString *charArr = CAT_STRING(string);
//			CatFixedString *result = cat_fixed_string_new_internal(charArr->data, charArr->size);
//			charArr->data = NULL;
//			cat_ref_sink_ptr(charArr);
//			cat_unref_ptr(charArr);
//			return result;
//		}
//		CatString *array = CAT_STRING(string);
//		return cat_fixed_string_new_len(array->data, array->size);
//	} else if (CAT_IS_UNICHAR_ARRAY(string)) {
//		CatUnicharArray *unichar_array = CAT_UNICHAR_ARRAY(string);
//		CatFixedString *result = cat_unichar_array_to_string(unichar_array);
//		return result;
//	} else if (CAT_IS_FIXED_STRING(string)) {
//		CatFixedString *cstring = CAT_FIXED_STRING(string);
//		cat_ref(cstring);
//		return cstring;
//	} else {
//		cat_log_warn("Can not convert object to string: %s", g_type_name_from_instance((GTypeInstance *) string));
//	}
//	return NULL;
//}
//
//
//CatFixedString *cat_fixed_string_convert_gstring(GString *string) {
//	CatFixedString result = new CatFixedString();
//	cat_ref_anounce(result);
//	result->data = string->str;
//	result->size = string->len;
//	result->hash = 0;
//	return result;
//
//
//}
//
//
//
//static void _dispose(GObject *object) {
////	printf("ben in de dispose van %lx\n", (unsigned long) object);
//	CatFixedString *nstring = CAT_FIXED_STRING(object);
//	if (nstring->data) {
//		g_free(nstring->data);
//		nstring->data = NULL;
//	}
//	nstring->size = 0;
//	nstring->hash = 0;
//}
//
	public int length() {
		return size;
	}
	
	public char charAt(int idx) {
		return data[idx];
	}

//int cat_fixed_string_compare(CatFixedString *string, CatFixedString *with) {
//	if (string==with) {
//		return 0;
//	}
//	if (string==NULL) {
//		return -1;
//	} else if (with==NULL) {
//		return 1;
//	}
//	int n = string->size>with->size ? with->size : string->size;
//	int res = memcmp(string->data, with->data, n);
//	if (res == 0) {
//		res = string->size>with->size ? 1 : (string->size == with->size ? 0 : -1);
//	}
//	return res;
//}
//
//int cat_fixed_string_case_compare(CatFixedString *string, CatFixedString *with) {
//	if (string==with) {
//		return 0;
//	}
//	if (string==NULL) {
//		return -1;
//	} else if (with==NULL) {
//		return 1;
//	}
//	int n = string->size>with->size ? with->size : string->size;
//	int res = strncasecmp(string->data, with->data, n);
//	if (res == 0) {
//		res = string->size>with->size ? 1 : (string->size == with->size ? 0 : -1);
//	}
//	return res;
//}
//
//
	public CatFixedString substring(int offset, int length) {
		if (length<0) {
			length = this.size-offset;
		}
		char new_data[] = new char[length+1];
		System.arraycopy(data, offset, new_data, 0, length);
		new_data[length] = (char) 0;
		CatFixedString result = new CatFixedString();
		result.data = new_data;
		result.size = length;
		result.hash = 0;
		return result;
	}
	
	public CatFixedString append(CatFixedString other) {
		int new_length = size+other.size;
		if (new_length==0) {
			return emptyString;
		}
	
		char new_data[] = new char[new_length+1];
		System.arraycopy(data, 0, new_data, 0, size);
		System.arraycopy(other.data, 0, new_data, size, other.size);
		new_data[new_length] = (char) 0;
		CatFixedString result = new CatFixedString();
		result.data = new_data;
		result.size = new_length;
		result.hash = 0;
		return result;
	}
////
////CatFixedString *cat_fixed_string_append_decimal(CatFixedString *this, long val) {
////	cat_ref_sink_ptr(this);
////	int idx=0;
////	gboolean sign = val<0;
////	if (sign) {
////		if (val==0x8000000000000000) {
////			return cat_fixed_string_append_chars_len(this, "-9223372036854775808", 20);
////		}
////		val = -val;
////	}
////	char buf[40];
////	long rest;
////	while(idx<39) {
////		rest = val % 10l;
////		val = val/10l;
////		buf[idx] = '0'+rest;
////		idx++;
////		if (val==0) {
////			break;
////		}
////	}
////	if (sign) {
////		buf[idx++] = '-';
////	}
////
////	int new_length = this->size+idx;
////	gchar *new_data = (gchar *) g_malloc(new_length+1);
////	memcpy(new_data, this->data, this->size);
////	int out_idx = this->size;
////	for(idx--; idx>=0; idx--) {
////		new_data[out_idx++] = buf[idx];
////	}
////	new_data[new_length] = (char) 0;
////	CatFixedString result = new CatFixedString();
////	cat_ref_anounce(result);
////	result->data = new_data;
////	result->size = new_length;
////	result->hash = 0;
////	cat_unref(this);
////	return result;
////}
//
//CatFixedString *cat_fixed_string_append_unichar(CatFixedString *this, gunichar unichar) {
//	char tmp[7];
//	int out_length = 0;
//	if (unichar<0x80) {
//		tmp[0] = unichar;
//		out_length = 1;
//	} else if (unichar< (1<<11)) {
//		tmp[0] = 0xC0 || (unichar>>6);
//		tmp[1] = 0x80 || (unichar & 0x3f);
//		out_length = 2;
//	} else if (unichar< (1<<16)) {
//		tmp[0] = 0xE0 || (unichar>>12);
//		tmp[1] = 0x80 || ((unichar>>6) && 0x3f);
//		tmp[2] = 0x80 || (unichar & 0x3f);
//		out_length = 3;
//	} else if (unichar< (1<<21)) {
//		tmp[0] = 0xF0 || (unichar>>18);
//		tmp[1] = 0x80 || ((unichar>>12) && 0x3f);
//		tmp[2] = 0x80 || ((unichar>>6) && 0x3f);
//		tmp[3] = 0x80 || (unichar & 0x3f);
//		out_length = 4;
//	} else if (unichar< (1<<26)) {
//		tmp[0] = 0xF8 || (unichar>>24);
//		tmp[1] = 0x80 || ((unichar>>18) && 0x3f);
//		tmp[2] = 0x80 || ((unichar>>12) && 0x3f);
//		tmp[3] = 0x80 || ((unichar>>6) && 0x3f);
//		tmp[4] = 0x80 || (unichar & 0x3f);
//		out_length = 5;
//	} else if (unichar< (1<<31)) {
//		tmp[0] = 0xFC || (unichar>>30);
//		tmp[1] = 0x80 || ((unichar>>24) && 0x3f);
//		tmp[2] = 0x80 || ((unichar>>18) && 0x3f);
//		tmp[3] = 0x80 || ((unichar>>12) && 0x3f);
//		tmp[4] = 0x80 || ((unichar>>6) && 0x3f);
//		tmp[5] = 0x80 || (unichar & 0x3f);
//		out_length = 6;
//	}
//	if (out_length==0) {
//		return this;
//	}
//	tmp[out_length] = 0;
//	return cat_fixed_string_append_chars(this, tmp);
//}
//
//CatFixedString *cat_fixed_string_format(CatFixedString *fmt_txt, ...) {
////	for(int idx=0; idx<fmt_txt->size; idx++) {
////		char ch = fmt_txt->data[idx];
////
////	}
//	return NULL;
//}
//
	public CatFixedString appendChars(char other[]) {
		int other_length = other.length;
		int new_length = this.size+other_length;
		if (new_length==0) {
			return emptyString;
		}
	
		char new_data[] = new char[new_length+1];

		System.arraycopy(data, 0, new_data, 0, size);
		System.arraycopy(other, 0, new_data, size, other.length);
		new_data[new_length] = (char) 0;
		CatFixedString result = new CatFixedString();
		result.data = new_data;
		result.size = new_length;
		result.hash = 0;
		return result;
	}
//
//CatFixedString *cat_fixed_string_append_chars_len(CatFixedString *this, const char *other, int length) {
//	g_object_ref_sink(this);
//	int new_length = this->size+length;
//	if (new_length==0) {
//		cat_unref(this);
//		return CAT_STATIC_STRING(cat_s_empty_string);
//	}
//
//	gchar *new_data = (gchar *) g_malloc(new_length+1);
//	memcpy(new_data, this->data, this->size);
//	memcpy(new_data+this->size, other, length);
//	new_data[new_length] = (char) 0;
//	CatFixedString result = new CatFixedString();
//	cat_ref_anounce(result);
//	result->data = new_data;
//	result->size = new_length;
//	result->hash = 0;
//	cat_unref(this);
//	return result;
//}
//
	public boolean equal(CatFixedString with) {
		if (this==with) {
			return true;
		}
		if (with==null) {
			return false;
		}
		if (size!=with.size) {
			return false;
		}
	
		return Arrays.equals(data, with.data);
	}
//
//gboolean cat_fixed_string_endswith(const CatFixedString *string, const CatFixedString *with) {
//	gboolean result = FALSE;
//	if (string->size>=with->size) {
//		result = memcmp(string->data+string->size-with->size, with->data, with->size)==0;
//	}
//	return result;
//}
//
//gboolean cat_fixed_string_beginswith(const CatFixedString *string, const CatFixedString *with) {
//	gboolean result = FALSE;
//	if (string->size>=with->size) {
//		result = memcmp(string->data, with->data, with->size)==0;
//	}
//	return result;
//}
//
//
	public int hash() {
		if (hash == 0) {
			int code = 0;
			int n = size>255 ? 255 : size;
	
			for(; n>=0; n--) {
				code = (code * 71) + data[n];
			}
			if (code==0) {
				code++;
			}
			hash = code;
		}
		return hash;
	}
//
//
//gchar *cat_fixed_string_to_gchars(const CatFixedString *string) {
//	gchar *result = g_malloc(string->size+1);
//	if (string->size>0) {
//		memcpy(result, string->data, string->size);
//	}
//	result[string->size] = '\0';
//	return result;
//}
//
//
//const gchar *cat_fixed_string_getchars(const CatFixedString *string) {
//	if (string==NULL) {
//		return "";
//	}
//	if (string->data==NULL) {
//		return "";
//	}
//	return (const gchar *) string->data;
//}
//
//const gchar *cat_fixed_string_safechars(const CatFixedString *string) {
//	if (string==NULL) {
//		return "";
//	}
//	if (string->data==NULL) {
//		return "";
//	}
//	return (const gchar *) string->data;
//
//}
//
//
//
//gint cat_fixed_string_last_index_of(CatFixedString *txt, gchar ch) {
//	int idx;
//	for(idx=txt->size-1; idx>=0; idx--) {
//		if (txt->data[idx]==ch) {
//			return idx;
//		}
//	}
//	return -1;
//}
//
//
//gint cat_fixed_string_index_of(CatFixedString *txt, gchar ch) {
//	int idx;
//	for(idx=0; idx<txt->size; idx++) {
//		if (txt->data[idx]==ch) {
//			return idx;
//		}
//	}
//	return -1;
//}
//
//gint cat_fixed_string_count_occurrence(CatFixedString *txt, gchar ch) {
//	int idx;
//	int count=0;
//	for(idx=0; idx<txt->size; idx++) {
//		if (txt->data[idx]==ch) {
//			count++;
//		}
//	}
//	return count;
//}
//
//gint cat_fixed_string_index_of_from(CatFixedString *txt, gchar ch, int idx) {
//	if (idx<0) {
//		idx = 0;
//	}
//	for(; idx<txt->size; idx++) {
//		if (txt->data[idx]==ch) {
//			return idx;
//		}
//	}
//	return -1;
//}
//
//gint cat_fixed_string_str_index_of(CatFixedString *txt, CatFixedString *str) {
//	int idx;
//	int subIdx;
//	int subEndIdx;
//	char ch = cat_fixed_string_char_at(str, 0);
//	int endIdx = txt->size-cat_fixed_string_length(str);
//	for(idx=0; idx<=endIdx; idx++) {
//		if (txt->data[idx]==ch) {
//			subEndIdx = cat_fixed_string_length(str);
//			for(subIdx=1; subIdx<subEndIdx; subIdx++) {
//				if (txt->data[idx+subIdx]!=cat_fixed_string_char_at(str, subIdx)) {
//					subIdx=-1;
//					break;
//				}
//			}
//			if (subIdx!=-1) {
//				return idx;
//			}
//		}
//	}
//	return -1;
//}
//
//gint cat_fixed_string_str_index_of_from(CatFixedString *txt, CatFixedString *str, int idx) {
//	if (idx<0) {
//		return -1;
//	}
//	int subIdx;
//	int subEndIdx;
//	char ch = cat_fixed_string_char_at(str, 0);
//	int endIdx = txt->size-cat_fixed_string_length(str);
//	for(; idx<=endIdx; idx++) {
//		if (txt->data[idx]==ch) {
//			subEndIdx = cat_fixed_string_length(str);
//			for(subIdx=1; subIdx<subEndIdx; subIdx++) {
//				if (txt->data[idx+subIdx]!=cat_fixed_string_char_at(str, subIdx)) {
//					subIdx=-1;
//					break;
//				}
//			}
//			if (subIdx!=-1) {
//				return idx;
//			}
//		}
//	}
//	return -1;
//}
//
//
//
//
//CatFixedString *cat_fixed_string_trim(CatFixedString *this) {
//	if (this->data==0) {
//		return this;
//	}
//	g_object_ref_sink(this);
//	int start_idx=0;
//	while(start_idx<this->size) {
//		gchar ch = this->data[start_idx];
//		if (ch==' ' || ch=='\t') {
//			start_idx++;
//		} else {
//			break;
//		}
//	}
//
//	int end_idx = this->size;
//	while(end_idx>0) {
//		gchar ch = this->data[end_idx-1];
//		if (ch==' ' || ch=='\t') {
//			end_idx--;
//		} else {
//			break;
//		}
//	}
//
//	int size = end_idx-start_idx;
//	if (size==0) {
//		cat_unref(this);
//		return CAT_STATIC_STRING(cat_s_empty_string);
//	}
//
//
//	CatFixedString result = new CatFixedString();
//	cat_ref_anounce(result);
//	result->size = size;
//	result->data = g_malloc(1+size);
//	memcpy(result->data, this->data+start_idx, size);
//	result->data[size] = 0;
//	result->hash = 0;
//	cat_unref(this);
//	return result;
//}
//
//
//CatFixedString *cat_fixed_string_to_lowercase(CatFixedString *txt) {
//	if (txt==NULL) {
//		return NULL;
//	}
//	cat_ref_sink_ptr(txt);
//	if (txt->size==0) {
//		cat_unref(txt);
//		return CAT_STATIC_STRING(cat_s_empty_string);
//	}
//
//	gchar *data = g_malloc(sizeof(gchar)*(txt->size+1));
//	int idx;
//	for(idx=0; idx<txt->size; idx++) {
//		data[idx] = cat_char_to_lower(txt->data[idx]);
//	}
//
//	CatFixedString *result = g_object_new(((GTypeInstance *) txt)->g_class->g_type, NULL);
//	cat_ref_anounce(result);
//	result->size = txt->size;
//	result->data = data;
//	result->data[txt->size] = 0;
//	result->hash = 0;
//	cat_unref(txt);
//	return result;
//}
//
//CatFixedString *cat_fixed_string_replace_all_char(CatFixedString *str, char replace, char with) {
//	if (str==NULL) {
//		return NULL;
//	}
//	int idx = str->size-1;
//	char *newdata = NULL;
//	while(idx>=0) {
//		if (str->data[idx]==replace) {
//			if (newdata==NULL) {
//				newdata = g_malloc(str->size+1);
//				memcpy(newdata, str->data, str->size);
//			}
//			newdata[idx] = with;
//		}
//		idx--;
//	}
//
//	CatFixedString *result;
//	if (newdata) {
//		cat_ref_sink_ptr(str);
//		result = g_object_new(((GTypeInstance *) str)->g_class->g_type, NULL);
//		cat_ref_anounce(result);
//		result->size = str->size;
//		result->data = newdata;
//		result->data[str->size] = 0;
//		result->hash = 0;
//		cat_unref_ptr(str);
//	} else {
//		result = str;
//	}
//	return result;
//}


public CatArray<CatFixedString> split(char ch, boolean include_empty) {
	CatArray<CatFixedString> result = new CatArray<CatFixedString>();
	if (size==0) {
		return result;
	}
	int idxa=0;
	int idxb=0;
	while(idxb<=size) {
		char crch = idxb==size ? ch : data[idxb];
		if (crch==ch) {
			if (idxa!=idxb || include_empty) {
				CatFixedString txt = substring(idxa, idxb-idxa);
				result.append(txt);
			}
			idxa=idxb+1;
		}
		idxb++;
	}
	return result;
}



//gchar cat_char_to_lower(gchar ch) {
//	if (ch>='A' && ch<='Z') {
//		return ch-'A'+'a';
//	}
//	return ch;
//}
//
//
//
//
///********************* start CatIStringable implementation *********************/
//
//static void l_stringable_print(CatIStringable *self, struct _CatString *append_to) {
//	CatFixedString *str = CAT_FIXED_STRING(self);
//	cat_string_append_string(append_to, str);
//}
//
//static void l_stringable_iface_init(CatIStringableInterface *iface) {
//	iface->print = l_stringable_print;
//}
//
///********************* end CatIStringable implementation *********************/

	

	@Override
	public String toString() {
		return new String(this.data,0,size);
	}
}
