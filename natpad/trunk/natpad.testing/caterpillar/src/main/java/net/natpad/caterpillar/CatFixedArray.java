package net.natpad.caterpillar;

import java.util.Arrays;
import java.util.Comparator;

public class CatFixedArray<T> implements CatIList<T>, Iterable<T> {

	private T data[];
	private int size;
	private int modificationCount;
	

	public CatFixedArray() {
		data = null;
		size = 0;
		modificationCount = 0;
	}
	
	@SuppressWarnings("unchecked")
	public static <T> CatFixedArray<T> convert(CatIList<T> list) {
		CatFixedArray<T> result = new CatFixedArray<T>();
		int size = list.count();
		result.data = (T[]) new Object[size];
		result.size = size;
		result.modificationCount = list.modCount();
		for(size--; size>=0; size--) {
			T at = list.getAt(size);
			result.data[size] = at;
		}
		return result;
	}
	
	
	public int size() {
		return size;
	}
	
	public int modificationCount() {
		return modificationCount;
	}

	public T get(int index) {
		if (index<0 || index>=size) {
			return null;
		}
		return data[index];
	}

	public T getFirst() {
		if (size<=0) {
			return null;
		}
		return data[0];
	}
	
	public T getLast() {
		if (size<=0) {
			return null;
		}
		return data[size-1];
	}

	public int findIndex(T searchItem, int hintIndex) {
		if (hintIndex>=0 && hintIndex<size) {
			if (data[hintIndex] == searchItem) {
				return hintIndex;
			}
		}
		for(int idx=0; idx<size; idx++) {
			if (data[idx] == searchItem) {
				return idx;
			}
		}
		return -1;
	}

	public int findIndex(T searchItem, int hintIndex, Comparator<T> c) {
		if (hintIndex>=0 && hintIndex<size) {
			if (c.compare(data[hintIndex],searchItem)==0) {
				return hintIndex;
			}
		}
		for(int idx=0; idx<size; idx++) {
			if (c.compare(data[idx], searchItem)==0) {
				return idx;
			}
		}
		return -1;
	}
	
	

	@SuppressWarnings("unchecked")
	public CatFixedArray<T> append(T toAppend) {
		CatFixedArray<T> result = new CatFixedArray<T>();
		if (data==null || size==0) {
			result.data = (T[]) new Object[1];
			result.data[0] = toAppend;
			result.size = 1;
		} else {
			result.data = (T[]) new Object[size+1];
			System.arraycopy(data, 0, result.data,0, size);
			result.data[size] = toAppend;
			result.size = size+1;
		}
		result.modificationCount = modificationCount+1;
		return result;
	}

//CatFixedArray *cat_fixed_array_append_all(CatFixedArray *fixed_array, CatIList *other);
//
//
	@SuppressWarnings("unchecked")
	public CatFixedArray<T> setAt(T toSet, int index) {
		if (index<0 || index>=size) {
			return this;
		}
		if (data[index]==toSet) {
			return this;
		}
		
		CatFixedArray<T> result = new CatFixedArray<T>();
		result.data = (T[]) new Object[size+1];
		System.arraycopy(data, 0, result.data,0, size);
		result.data[index] = toSet;
		result.size = size;
		result.modificationCount = modificationCount+1;
		return result;
		
	}

	@SuppressWarnings("unchecked")
	public CatFixedArray<T> insert(int index, T toInsert) {
		if (index>=size) {
			return append(toInsert);
		}
		if (index<0) {
			index = 0;
		}
		CatFixedArray<T> result = new CatFixedArray<T>();
		result.data = (T[]) new Object[size+1];
		int idx, idx_in=0;

		for(idx=0; idx<size+1; idx++) {
			if (idx==index) {
				result.data[idx] = toInsert;
				idx_in--;
			} else {
				result.data[idx] = data[idx_in];
			}
			idx_in++;
		}
		result.data[index] = toInsert;		
		result.size = size;
		result.modificationCount = modificationCount+1;
		return result;
		
	}

	@SuppressWarnings("unchecked")
	public CatFixedArray<T> remove(int index, CatObjectPtr<T> removed) {
		if (index<0 || index>=size) {
			if (removed!=null) {
				removed.set(null);
			}
			return this;
		}
		if (removed!=null) {
			removed.set(data[index]);
		}


		CatFixedArray<T> result = new CatFixedArray<T>();
		result.modificationCount = modificationCount+1;
		result.size = size-1;
		if (result.size==0) {
			result.data = null;
			return result;
		}
		result.data = (T[]) new Object[size-1];
		int idx, idx_in = 0;
		for(idx=0; idx<size-1; idx++) {
			if (idx_in==index) {
				idx--;
			} else {
				result.data[idx] = data[idx_in];
			}
			idx_in++;
		}
		return result;		
	}
	

	@SuppressWarnings("unchecked")
	public CatFixedArray<T> removeLast(CatObjectPtr<T> removed) {
		if (size<=0) {
			if (removed!=null) {
				removed.set(null);
			}
			return null;
		}
		CatFixedArray<T> result = new CatFixedArray<T>();
		result.modificationCount = modificationCount+1;
		result.size = size-1;
		if (removed!=null) {
			removed.set(data[result.size]);
		}
		if (result.size==0) {
			result.data = null;
			return result;
		}
		result.data = (T[]) new Object[size-1];
		int idx;
		for(idx=size-2; idx>=0; idx--) {
			result.data[idx] = data[idx];
		}
		return result;		
	}
	
	@SuppressWarnings("unchecked")
	public CatFixedArray<T> removeFirst(CatObjectPtr<T> removed) {
		if (size<=0) {
			if (removed!=null) {
				removed.set(null);
			}
			return null;
		}
		CatFixedArray<T> result = new CatFixedArray<T>();
		result.modificationCount = modificationCount+1;
		result.size = size-1;
		if (removed!=null) {
			removed.set(data[0]);
		}
		if (result.size==0) {
			result.data = null;
			return result;
		}
		result.data = (T[]) new Object[size-1];
		int idx;
		for(idx=size-2; idx>=0; idx--) {
			result.data[idx] = data[idx+1];
		}
		return result;		
	}

	@SuppressWarnings("unchecked")
	public CatFixedArray<T> sub(int start, int length) {
		if (length>size) {
			length = size;
		}
		if (size==length && start==0) {
			return this;
		}
		CatFixedArray<T> result = new CatFixedArray<T>();
		result.modificationCount = modificationCount+1;
		result.size = length;
		if (length==0) {
			result.data = null;
			return result;
		}
		result.data = (T[]) new Object[length];
		int idx;
		for(idx=0; idx<length; idx++) {
			result.data[idx] = data[start+idx];
		}

		return result;
	}

//void cat_fixed_array_dump_contents(CatFixedArray *fixed_array);
//
//gboolean cat_fixed_array_equal(CatFixedArray *fixed_array, CatFixedArray *other, GEqualFunc equal_func_for_value);
//
	public CatIIterator<T> iterator() {
		return new FixedArrayIterator<T>(this);
	}
//
	@SuppressWarnings("unchecked")
	public CatFixedArray<T> sort(Comparator<T> comparator) {
		if (size==0) {
			return this;
		}
		
		CatFixedArray<T> result = new CatFixedArray<T>();
		result.modificationCount = modificationCount+1;
		result.size = size;
		result.data = (T[]) new Object[size];
		System.arraycopy(data, 0, result.data, 0, size);
		Arrays.sort(result.data, comparator);
		return result;
		
	}

	
	static class FixedArrayIterator<T> implements CatIIterator<T> {
		
		private final CatFixedArray<T> origin;
		int offset;
		
		public FixedArrayIterator(CatFixedArray<T> origin) {
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

	
	public CatIList<T> add(T object) {
		return append(object);
	};
	
	@Override
	public int count() {
		return size();
	}
	
	@Override
	public T getAt(int index) {
		return get(index);
	}
	
	public CatIList<T> insertAt(T object, int index) {
		return insert(index, object);
	};

	@Override
	public CatIList<T> removeAt(int index) {
		return remove(index, null);
	}
	
//	@Override
//	public CatIList<T> sort(Comparator<T> cmpFunc) {
//		
//		Arrays.sort(data, cmpFunc);
//		return null;
//	}
	
	@Override
	public boolean isEmpty() {
		return size()==0;
	}
	
	@Override
	public int modCount() {
		return modificationCount();
	}
	
	
}
