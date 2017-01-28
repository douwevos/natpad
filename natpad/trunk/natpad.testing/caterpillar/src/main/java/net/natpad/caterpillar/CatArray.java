package net.natpad.caterpillar;

import java.util.Arrays;
import java.util.Comparator;

public class CatArray<T> implements CatIList<T>, Iterable<T> {

	protected T[] data;
	protected int data_size;
	protected int size;
	protected int modification_count;
	
	
	public CatArray() {
		data = null;
		data_size = 0;
		size = 0;
		modification_count = 0;
//		cat_log_detail("new:%p", result);
	}

	
	public CatArray(T data[]) {
		this.data = data;
		data_size = data.length;
		size = data.length;
		modification_count = 0;
	}

	
	
	@SuppressWarnings("unchecked")
	public CatArray(int initial_size) {
		if (initial_size<=0) {
			data = null;
			data_size = 0;
		} else {
			data = (T[]) new Object[initial_size];
			data_size = initial_size;
		}
		size = 0;
		modification_count = 0;
//		cat_log_detail("new:%p", result);
	}


//	CatArray *cat_array_new_from_glist(GList *list) {
//		CatArray *result = g_object_new(CAT_TYPE_ARRAY, NULL);
//		cat_ref_anounce(result);
//		result->size = g_list_length(list);
//		result->data = g_malloc(sizeof(void *) * (result->size));
//		result->data_size = result->size;
//		result->modification_count = 0;
//		int index = 0;
//		while(list) {
//			result->data[index++] = cat_ref_ptr(list->data);
//			list = list->next;
//		}
//		return result;
//	}


	public static <T> CatArray<T> convert(CatIList<T> list, boolean allow_reuse) {
		if (list instanceof CatArray) {
			return (CatArray<T>) list;
		}
//		if (allow_reuse && g_object_is_floating(list) && CAT_IS_FIXED_ARRAY(list)) {
//			/* The list is of the type Fixed Array, is still floating and the
//			 * flag allow_reuse is set true. This allows us to steal its array
//			 * saving us a full copy of the underlying array.
//			 */
//			cat_ref_sink_ptr(list);	// WARN !! do not move this to the top
//			CatFixedArray *fixed_array = CAT_FIXED_ARRAY(list);
//			CatArray *result = cat_array_new();
//			result->data_size = cat_fixed_array_size(fixed_array);
//			result->size = result->data_size;
//			result->modification_count = cat_fixed_array_modification_count(fixed_array);
//			result->data = (void **) cat_fixed_array_unsafe_set_data(fixed_array, NULL, 0);
//			cat_unref(list);
//			return result;
//		}
//		cat_ref_ptr(list);	// WARN !! do not move this to the top
		int count = list.count();
		CatArray<T> result = new CatArray<T>(count);
		result.modification_count = list.modCount();
		
		CatIIterator<T> iterator = list.iterator();
		int index = 0;
		while(iterator.hasNext()) {
			if (index>result.data_size) {
//				cat_log_error("source list changed while converting");
				result._check_size(index+16);	// something went wrong so we reserve an extra 16 items ensuring the log is not filled with the above warning
			}
			result.data[index] = iterator.next();
			index++;
		}
		result.size = index;
		return result;
	}

	public void clear() {
		size = 0;
		modification_count++;
	}


	public int size() {
		return size;
	}
//
//	void cat_array_set_size(CatArray *array, int new_size) {
//		if (new_size==array->size) {
//			return;
//		} else if (new_size>array->size) {
//			_check_size(array, new_size);
//			int set_idx;
//			for(set_idx=array->size; set_idx<new_size; set_idx++) {
//				array->data[set_idx] = NULL;
//			}
//		} else {
//			int unset_idx;
//			for(unset_idx=array->size-1; unset_idx>=new_size; unset_idx--) {
//				cat_unref_ptr(array->data[unset_idx]);
//			}
//		}
//		array->size = new_size;
//		array->modification_count++;
//	}
//
//
	public T get(int index) {
		if (index>=0 && index<size) {
			return data[index];
		}
		return null;
	}

	public T getFirst() {
		if (size>0) {
			return data[0];
		}
		return null;
	}

	public T getLast() {
		if (size>0) {
			return data[size-1];
		}
		return null;
	}


	public int findIndex(T search_item, int hint_index) {
//		cat_log_detail("array=%p", array);
		if (hint_index>=0 && hint_index<size) {
			if (data[hint_index]==search_item) {
				return hint_index;
			}
		}
		int idx=0;
		for(idx=0; idx<size; idx++) {
			if (data[idx]==search_item) {
				return idx;
			}
		}
		return -1;
	}
//
//
//
//	int cat_array_find_index_ext(CatArray *array, void *search_item, int hint_index, GEqualFunc equal_func) {
//		g_return_val_if_fail(equal_func!=NULL, -1);
//		cat_log_detail("cat_array_find_index:array=%p", array);
//		if (hint_index>=0 && hint_index<array->size) {
//			if (equal_func(array->data[hint_index], search_item)) {
//				return hint_index;
//			}
//		}
//		int idx=0;
//		for(idx=0; idx<array->size; idx++) {
//			if (equal_func(array->data[idx], search_item)) {
//				return idx;
//			}
//		}
//		return -1;
//	}
//
//
	@SuppressWarnings("unchecked")
	private void _check_size(int needed_size) {
		if (needed_size==0) {
			return;
		}
		needed_size = needed_size+(64-(needed_size%64));
		if (data_size == 0) {
			data = (T[]) new Object[needed_size];
			data_size = needed_size;
		}
		if (needed_size>data_size) {
			T[] newdata = (T[]) new Object[needed_size];
			System.arraycopy(data, 0, newdata, 0, size);
			data_size = needed_size;
			data = newdata;
		}
	}

	public void append(T to_append) {
		_check_size(size+1);
		data[size] = to_append;
		size++;
		modification_count++;
	}


	public void appendAll(CatIList<T> other) {
		int size = other.count();
		_check_size(size+size);
		int out_idx = size;
		int in_idx;
		for(in_idx=0; in_idx<size; in_idx++) {
			data[out_idx] = other.getAt(in_idx);
			out_idx++;
		}
		size += size;
		modification_count++;
	}


	public boolean setAt(T to_insert, int index, CatObjectPtr<T> old_value) {
		if (index<0 || index>=size) {
			if (old_value!=null) {
				old_value.set(null);
			}
			return false;
		}
//		cat_log_debug("array:%p, index=%d, array->size=%d", array, index, array->size);

		if (old_value!=null) {
			old_value.set(data[index]);
		}

		data[index] = to_insert;
		modification_count++;
		return true;
	}

	public void insert(int index, T to_insert) {
		if (index>=size) {
			append(to_insert);
			return;
		}
		if (index<0) {
			index = 0;
		}

		_check_size(size+1);
		int idx;
		for(idx=size-1; idx>=index; idx--) {
			data[idx+1] = data[idx];
		}

		data[index] = to_insert;
		size++;
		modification_count++;
	}


	public boolean remove(int index, CatObjectPtr<T> removed) {
		if (index<0 || index>=size) {
			if (removed!=null) {
				removed.set(null);
			}
			return false;
		}
		if (removed!=null) {
			removed.set(data[index]);
		}

		modification_count++;
		if (size==1) {
			size = 0;
			return true;
		}
		int idx;
		for(idx=index+1; idx<size; idx++) {
			data[idx-1] = data[idx];
		}
		size--;
		return true;
	}



	public boolean removeLast(CatObjectPtr<T> removed) {
		if (size<=0) {
			if (removed!=null) {
				removed.set(null);
			}
			return false;
		}
		if (removed!=null) {
			removed.set(data[size-1]);
		}

		modification_count++;
		size--;
		return true;
	}


	public boolean removeFirst(CatObjectPtr<T> removed) {
		if (size<=0) {
			if (removed!=null) {
				removed.set(null);
			}
			return false;
		}
		if (removed!=null) {
			removed.set(data[0]);
		}

		modification_count++;
		if (size==1) {
			size = 0;
			return true;
		}
		int idx;
		for(idx=1; idx<size; idx++) {
			data[idx-1] = data[idx];
		}
		size--;
		return true;
	}
//
//
//	boolean cat_array_equal(CatArray *array, CatArray *other, GEqualFunc equal_func_for_value) {
//		if (array==other) {
//			return true;
//		}
//		if (other==null || array==null) {
//			return false;
//		}
//
//		if (array->size!=other->size) {
//			return false;
//		}
//		boolean result = true;
//		if (equal_func_for_value==null) {
//			result = memcmp(array->data, other->data, sizeof(void *)*other->size)==0;
//		} else {
//			int idx;
//			for(idx=0; idx<array->size; idx++) {
//				void *array_data = array->data[idx];
//				void *other_data = other->data[idx];
//				if (array_data!=other_data) {
//					if (equal_func_for_value) {
//						if (!equal_func_for_value(array_data, other_data)) {
//							result = false;
//							idx = array->size;
//						}
//					} else {
//						result = false;
//						idx = array->size;
//					}
//				}
//			}
//		}
//		return result;
//	}
//
//
	public void reverse() {
		if (size>1) {
			int start =0;
			int end = size-1;
			T swap;
			while(start<end) {
				swap = data[start];
				data[start] = data[end];
				data[end] = swap;
				start++;
				end--;
			}
			modification_count++;
		}
	}


	public CatIIterator<T> iterator() {
		return new ArrayIterator<T>(this);
	}
//
//	void cat_array_dump_contents(CatArray *array) {
//		cat_log_detail("dumping array:array=%p :: size=%d", array, array->size);
//		int idx;
//		for(idx=0; idx<array->size; idx++) {
//			printf("  [%d] = %p\n", idx, array->data[idx]);
//		}
//	}
//


	static class ArrayIterator<T> implements CatIIterator<T> {
		
		private final CatArray<T> origin;
		int offset;
		
		public ArrayIterator(CatArray<T> origin) {
			this.origin = origin;
			offset = 0;
		}
		
		@Override
		public boolean hasNext() {
			return offset<origin.size;
		}
		
		@Override
		public boolean isLast() {
			return offset+1==origin.size;
		}

		@Override
		public T next() {
			return origin.get(offset++);
		}
		
		@Override
		public void remove() {
			throw new UnsupportedOperationException();
		}
		
	}	
	
	
	

	
	@Override
	public int modCount() {
		return modification_count;
	}

	@Override
	public int count() {
		return size;
	}
	
	@Override
	public boolean isEmpty() {
		return size==0;
	}
	
	@Override
	public T getAt(int index) {
		return get(index);
	}

	public CatIList<T> setAt(T object, int index) {
		setAt(object, index, null);
		return this;
	};
	
	
	public CatIList<T> insertAt(T object, int index) {
		insert(index, object);
		return this;
	};
	
	public CatIList<T> add(T object) {
		append(object);
		return this;
	};
	

	@Override
	public CatIList<T> removeAt(int index) {
		remove(index, null);
		return this;
	}
	
	@Override
	public CatIList<T> sort(Comparator<T> cmpFunc) {
		Arrays.sort(data, cmpFunc);
		return this;
	}
	
	
	
	
	
	
	
	
}
