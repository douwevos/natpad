package net.natpad.caterpillar;

import java.util.Arrays;


public class CatString {

	private char data[];
	private int size;
	private int dataSize;
	private int hashCode;
	
	public CatString() {
		data = new char[1];
		data[0] = 0;
		size = 0;
		dataSize = 0;
		hashCode = 0;
	}
	
	
	

	private void ensureCapicity(int requested_size) {
		requested_size++;
		requested_size = ((requested_size+15)/16)*16;

		if (dataSize<requested_size) {
			if (dataSize==0) {
				data = new char[requested_size];
			} else {
				char newData[] = new char[requested_size];
				System.arraycopy(data, 0, newData, 0, size+1);
				data = newData;
			}
			dataSize = requested_size;
		}
	}	
	
	public int length() {
		return size;
	}
	
	
	public void clear(boolean full) {
		size = 0;
		hashCode = 0;
		data[0] = 0;
		if (full) {
			data = new char[16];
			data[0] = 0;
			dataSize = 15;
		}
	}
	
	public char charAt(int index) {
		return data[index];
	}
	
	public int compare(CatString with) {
		if (this==with) {
			return 0;
		}
		if (with==null) {
			return 1;
		}

		int n = size>with.size ? with.size : size;
		for(int idx=0; idx<n; idx++) {
			char cha = data[idx];
			char chb = with.data[idx];
			if (cha<chb) {
				return -1;
			} else if (cha>chb) {
				return 1;
			}
		}
		
		return size>with.size ? 1 : (size == with.size ? 0 : -1);
	}

	public int caseCompare(CatString with) {
		if (this==with) {
			return 0;
		}
		if (with==null) {
			return 1;
		}

		int n = size>with.size ? with.size : size;
		for(int idx=0; idx<n; idx++) {
			char cha = Character.toLowerCase(data[idx]);
			char chb = Character.toLowerCase(with.data[idx]);
			if (cha<chb) {
				return -1;
			} else if (cha>chb) {
				return 1;
			}
		}
		
		return size>with.size ? 1 : (size == with.size ? 0 : -1);
	}


	public void append(CatString other) {
		int new_length = size+other.size;
		ensureCapicity(new_length);
		System.arraycopy(other.data, 0, data, size, other.size);
		data[new_length] = (char) 0;
		size = new_length;
		hashCode = 0;
	}
	
	

//	void cat_string_append_string(CatString *char_array, const CatFixedString *other) {
//		int new_length = char_array->size+cat_fixed_string_length(other);
//		_ensure_capicity(char_array, new_length);
//		gchar *new_data = char_array->data;
//		memcpy(new_data+char_array->size, cat_fixed_string_getchars(other), cat_fixed_string_length(other));
//		new_data[new_length] = (gchar) 0;
//		char_array->size = new_length;
//		char_array->hash = 0;
//	}
	
	

	public void appendChar(char ch) {
		int new_length = size+1;
		ensureCapicity(new_length);
		data[new_length-1] = ch;
		data[new_length] = (char) 0;
		size++;
		hashCode = 0;
	}
	
	
	public void appendDecimal(long val) {
		int idx=0;
		boolean sign = val<0;
		if (sign) {
			if (val==0x8000000000000000l) {
				appendCharsLen("-9223372036854775808", 20);
				return;
			}
			val = -val;
		}
		char buf[] = new char[40];
		long rest;
		while(idx<39) {
			rest = val % 10l;
			val = val/10l;
			buf[idx] = (char) ('0'+rest);
			idx++;
			if (val==0) {
				break;
			}
		}
		if (sign) {
			buf[idx++] = '-';
		}

		int new_length = size+idx;
		ensureCapicity(new_length);
		
		int out_idx = size;
		for(idx--; idx>=0; idx--) {
			data[out_idx++] = buf[idx];
		}
		data[new_length] = (char) 0;
		size = new_length;
		hashCode = 0;
	}

	
	
	private final static String l_txt_hex = "0123456789ABCDEF";
	
	public void appendHexadecimal(long val, int digits) {
		int idx=0;
		char buf[] = new char[40];
		if (digits<1) {
			digits = 1;
		}
		while((val>0) || (digits>0)) {
			buf[idx] = l_txt_hex.charAt((int) (val & 0xf));
			val = val>>4;
			idx++;
			digits--;
		}
	
		int new_length = size+idx;
		ensureCapicity(new_length);
		
		int out_idx = size;
		for(idx--; idx>=0; idx--) {
			data[out_idx++] = buf[idx];
		}
		data[new_length] = (char) 0;
		size = new_length;
		hashCode = 0;
	}
	

	public void appendChars(String other) {
		int other_length = other.length();
		int new_length = size+other_length;
		ensureCapicity(new_length);
		System.arraycopy(other.toCharArray(), 0, data, size, other_length);
		data[new_length] = (char) 0;
		size = new_length;
		hashCode = 0;
	}	

	

	public void appendCharsLen(String other, int other_length) {
		int new_length = size+other_length;
		ensureCapicity(new_length);
		System.arraycopy(other.toCharArray(), 0, data, size, other_length);
		data[new_length] = (char) 0;
		size = new_length;
		hashCode = 0;
	}
	
	


	
	public boolean equal(CatString with) {
		if (with==this) {
			return true;
		}
		if (with==null) {
			return false;
		}
		return Arrays.equals(data, with.data);
	}
	
	public int hash() {
		if (hashCode == 0) {
			int code = 0;
			int n = size>255 ? 255 : size;
			for(; n>=0; n--) {
				code = (code * 71) + data[n];
			}
			if (code==0) {
				code++;
			}
			hashCode = code;
		}
		return hashCode;
	}
//	
//	
//	GString *cat_string_to_gstring(const CatString *char_array) {
//		return g_string_new_len(char_array->data, char_array->size);
//	}
//	
//	gchar *cat_string_to_gchars(const CatString *char_array) {
//		gchar *result = g_malloc(char_array->size+1);
//		if (char_array->size>0) {
//			memcpy(result, char_array->data, char_array->size);
//		}
//		result[char_array->size] = '\0';
//		return result;
//	}
//	
	public final char[] getchars() {
		if (data==null) {
			char[] result = new char[1];
			result[0] = 0;
			return result;
		}
		return data;
	}
	
	
	public int lastIndexOf(char ch) {
		int idx;
		for(idx=size-1; idx>=0; idx--) {
			if (data[idx]==ch) {
				return idx;
			}
		}
		return -1;
	}
	
	
	public int indexOf(char ch) {
		int idx;
		for(idx=0; idx<size; idx++) {
			if (data[idx]==ch) {
				return idx;
			}
		}
		return -1;
	}
	
	
	
	public void trim() {
		if (dataSize==0) {
			return;
		}
		int start_idx=0;
		while(start_idx<size) {
			char ch = data[start_idx];
			if (ch==' ' || ch=='\t') {
				start_idx++;
			} else {
				break;
			}
		}
	
		int end_idx = size;
		while(end_idx>0) {
			char ch = data[end_idx-1];
			if (ch==' ' || ch=='\t') {
				end_idx--;
			} else {
				break;
			}
		}
	
		int size = end_idx-start_idx;
		if (start_idx>0) {
			System.arraycopy(data, start_idx, data, 0, size);
		}
		this.size = size;
		this.data[size] = 0;
		this.hashCode = 0;
	}
	
//	void cat_string_set_length(CatString *char_array, int length) {
//		if (length<0) {
//			cat_log_warn("length smaller then 0: %d", length);
//			length = 0;
//		}
//		if (char_array->size<=length) {
//			return;
//		}
//		char_array->size = length;
//		char_array->hash = 0;
//		char_array->data[length] = 0;
//	}
//	
//	
//	
	public void toLowercase() {
		int idx;
		for(idx=0; idx<size; idx++) {
			data[idx] = Character.toLowerCase(data[idx]);
		}
		hashCode = 0;
	}
	
	
	
		
	private final static int FM_NONE = 0;
	private final static int FM_START = 1; // '%'
	private final static int FM_ESCAPE = 2; // '\'
	private final static int FM_START_ZERO = 3; // '%0'

	public static CatString format(String text, Object... args) {
		CatString result = new CatString();
		int argsIdx = 0;

		boolean keep_running = true;
		int mode = FM_NONE;
		int cidx = 0;
		while (keep_running) {
			char ch = cidx>=text.length() ? 0 : text.charAt(cidx);
			switch (mode) {
				case FM_NONE: {
					if (ch == 0) {
						keep_running = false;
					} else if (ch == '%') {
						mode = FM_START;
					} else if (ch == '\\') {
						mode = FM_ESCAPE;
					} else {
						result.appendChar(ch);
					}
				}
					break;
				case FM_START: {
					if (ch == 0) {
						result.appendChar('%');
						keep_running = false;
					} else if (ch == '%') {
						result.appendChar('%');
					} else if (ch == '0') {
						mode = FM_START_ZERO;
					} else if (ch == 'd') {
						Object val = args[argsIdx++];
						long lval = 0l;
						if (val instanceof Number) {
							lval = ((Number) val).longValue();
						} else {
							lval = val.hashCode();
						}
						result.appendDecimal(lval);
						mode = FM_NONE;
					} else if (ch == 's') {
						Object val = args[argsIdx++];
						result.appendChars(val.toString());
						mode = FM_NONE;
					} else if (ch == 'o') {
						Object val = args[argsIdx++];
						if (val == null) {
							result.appendCharsLen("<nil>", 5);
							// } else if (CAT_IS_ISTRINGABLE(val)) {
							// CAT_ISTRINGABLE_GET_INTERFACE((CatIStringable *)
							// val)->print((CatIStringable *) val, result);
						} else {
							result.appendChars(val.getClass().getName());
							// cat_string_append_chars_len(result, "[0x", 3);
							// cat_string_append_hexadecimal(result, (unsigned
							// long long) val, -1);
							// cat_string_append_chars_len(result, "]", 1);
						}
						// cat_string_append_chars(result, val);
						mode = FM_NONE;
					} else if (ch == 'p') {
						Object val = args[argsIdx++];
						long lval = 0l;
						if (val instanceof Number) {
							lval = ((Number) val).longValue();
						} else {
							lval = val.hashCode();
						}
						result.appendCharsLen("0x", 2);
						result.appendHexadecimal(lval, -1);
						mode = FM_NONE;
					} else if (ch == 'x') {
						Object val = args[argsIdx++];
						long lval = 0l;
						if (val instanceof Number) {
							lval = ((Number) val).longValue();
						} else {
							lval = val.hashCode();
						}
						result.appendHexadecimal(lval, -1);
						mode = FM_NONE;
					} else {
						result.appendChar('%');
						result.appendChar(ch);
						mode = FM_NONE;
					}
				}
					break;

				case FM_ESCAPE: {
					if (ch == 0) {
						result.appendChar('\\');
						keep_running = false;
					} else if (ch == '%') {
						result.appendChar('%');
						mode = FM_NONE;
					} else if (ch == 'n') {
						result.appendChar('\n');
						mode = FM_NONE;
					} else if (ch == 't') {
						result.appendChar('\t');
						mode = FM_NONE;
					} else {
						result.appendChar('\\');
						result.appendChar(ch);
						mode = FM_NONE;
					}
				}
					break;

				case FM_START_ZERO: {
					if (ch == 0) {
						result.appendCharsLen("%0", 2);
						keep_running = false;
					} else {
						result.appendCharsLen("%0", 2);
						mode = FM_NONE;
					}
				}
					break;

			}
			cidx++;
		}

		return result;
	}


	
	@Override
	public String toString() {
		return new String(data,0,size);
	}
}
