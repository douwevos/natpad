package net.natpad.caterpillar;

public class CatFixedHashMap<K, V> {

	private final static float LOAD_FACTOR = 0.75f;
	private final static int LOAD_FACTOR_INCREASE = 61;

	
	int size;
	int modification_count;
	int bucket_count;
	CatIList<Object> buckets[];

	
	
	private CatIList<Object>[] _clone_buckets_list() {
		if (bucket_count==0) {
			return null;
		}
		@SuppressWarnings("unchecked")
		CatIList<Object> result[] = new CatIList[bucket_count];
		int index = 0;
		for(index=0; index<bucket_count; index++) {
			result[index] = buckets[index];
		}
		return result;
	}

	private CatIList<Object>[] _create_buckets_list(int count) {
		if (count==0) {
			return null;
		}
		@SuppressWarnings("unchecked")
		CatIList<Object>[] result = new CatIList[count];
		int index = 0;
		for(index=0; index<count; index++) {
			result[index] = null;
		}
		return result;
	}
	


	private CatIList<Object> _get_bucket_list(K key) {
		if (bucket_count==0) {
			return null;
		}
		int hash = key.hashCode();
		return buckets[hash % bucket_count];
	}

	@SuppressWarnings("unchecked")
	private V _get_from_bucket_list(CatIList<Object> bucket, K key, IntPointer index) {
		K skey;
		V svalue;
		V result = null;
//		GObject *skey, *svalue;
//		GObject *result = null;
		CatIIterator<?> iterator = bucket.iterator();
		int cnt = 0;
		if (index!=null) {
			index.val =  -1;
		}
		while(iterator.hasNext()) {
			skey = (K) iterator.next();
			svalue = (V) iterator.next();
//			cat_log_error("key:%p, skey=%p, %d", key, skey, equal_func(key, skey));
			if (key.equals(skey)) {
				result = svalue;
				if (index!=null) {
					index.val = cnt;
				}
				break;
			}
			cnt+=2;
		}
		return result;
	}


	@SuppressWarnings("unchecked")
	private CatFixedHashMap<K, V> _rehash(int new_bucket_count) {
		if (new_bucket_count<LOAD_FACTOR_INCREASE) {
			new_bucket_count = LOAD_FACTOR_INCREASE;
		}
//		cat_log_debug("new_bucket_count=%d", new_bucket_count);
		if (new_bucket_count==bucket_count) {
			return this;
		}

		CatIList<Object>[] bucket_list = _create_buckets_list(new_bucket_count);

		int bucket_index, hash, new_bucket_index;
		K key;
		V value;
		CatIList<Object> new_bucket;
		for(bucket_index=0; bucket_index<bucket_count; bucket_index++) {
			CatIList<Object> bucket = buckets[bucket_index];
			if (bucket!=null) {
				CatIIterator<Object> bucket_iter = bucket.iterator();
				while(bucket_iter.hasNext()) {
					key = (K) bucket_iter.next();
					value = (V) bucket_iter.next();
					hash = key.hashCode();
					new_bucket_index = hash%new_bucket_count;
					new_bucket = bucket_list[new_bucket_index];
					if (new_bucket == null) {
						new_bucket = new CatFixedArray<Object>();
					}
					new_bucket = new_bucket.add(key);
					new_bucket = new_bucket.add(value);
					bucket_list[new_bucket_index] = new_bucket;
				}
			}
		}


		CatFixedHashMap<K, V> result = new CatFixedHashMap<K, V>();
		result.buckets = bucket_list;
		result.bucket_count = new_bucket_count;
		result.size = size;
		result.modification_count = modification_count;
		return result;
	}
//
//	CatFixedHashMap *cat_fixed_hash_map_rehash(CatFixedHashMap *map) {
//		return _rehash(map, map->bucket_count);
//	}
//
//
	private CatFixedHashMap<K,V> _rehash_check(CatFixedHashMap<K,V> map) {
		float nice_bucket_count = (float) map.size * LOAD_FACTOR;
		int wanted_bckt_cnt = (int) Math.ceil(nice_bucket_count);
		wanted_bckt_cnt = (wanted_bckt_cnt-1+LOAD_FACTOR_INCREASE)/LOAD_FACTOR_INCREASE;
		wanted_bckt_cnt *= LOAD_FACTOR_INCREASE;
		if (map.bucket_count==wanted_bckt_cnt) {
			return map;
		}
		return map._rehash(wanted_bckt_cnt);
	}

	public int count() {
		return size;
	}

	public int modCount() {
		return modification_count;
	}

//	gboolean cat_fixed_hash_map_equals(CatFixedHashMap *map, CatFixedHashMap *other, GEqualFunc equal_func_for_value) {
//		if (map==other) {
//			return TRUE;
//		}
//		if ((map==null) || (other==null)) {
//			return FALSE;
//		}
//		if (map->size!=other->size) {
//			return FALSE;
//		}
//
//		gboolean result = TRUE;
//		int bucket_index;
//		GObject *value;
//		GObject *other_val;
//		GObject *key;
//		for(bucket_index=0; bucket_index<map->bucket_count && result; bucket_index++) {
//			CatIList *bucket = (CatIList *) map->buckets[bucket_index];
//			if (bucket) {
//				if (!cat_ilist_is_empty(bucket)) {
//					CatIIterator *src_iter = cat_ilist_iterator(bucket);
//					while(cat_iiterator_has_next(src_iter) && result) {
//						key = cat_iiterator_next(src_iter);
//						value = cat_iiterator_next(src_iter);
//						other_val = cat_fixed_hash_map_get(other, key);
//						if (equal_func_for_value==null) {
//							result = value==other_val;
//						} else if (!equal_func_for_value(value, other_val)) {
//							result = FALSE;
//						}
//					}
//					cat_unref_ptr(src_iter);
//				}
//			}
//		}
//		return result;
//	}
//
	public V get(K key) {
		CatIList<Object> bucket_list = _get_bucket_list(key);
		if (bucket_list!=null) {
			return _get_from_bucket_list(bucket_list, key, null);
		}
		return null;
	}

	public CatFixedHashMap<K,V> put(K key, V value) {
		CatIList<Object> new_bucket = null;
		int hash = key.hashCode();
//		cat_log_debug("map->bucket_count=%d, hash=%u", map->bucket_count, hash);
		if (bucket_count==0) {
			new_bucket = new CatFixedArray<Object>();
			new_bucket = new_bucket.add(key);
			new_bucket = new_bucket.add(value);
			int new_bucket_count = LOAD_FACTOR_INCREASE;
			CatIList<Object>[] bucket_list = _create_buckets_list(new_bucket_count);
			int bucket_index = hash % new_bucket_count;
			bucket_list[bucket_index] = new_bucket;
			CatFixedHashMap<K,V> result = new CatFixedHashMap<K,V>();
			result.buckets = bucket_list;
			result.bucket_count = new_bucket_count;
			result.size = size+1;
			result.modification_count = modification_count+1;
			return result;
		}
		int new_size = size;
		int bucket_index = hash % bucket_count;
//		cat_log_detail("bucket_index=%d, new_size=%d", bucket_index, new_size);
		CatIList<Object> bucket = buckets[bucket_index];
		if (bucket!=null) {
			IntPointer key_index = new IntPointer();
			V old_value = _get_from_bucket_list(bucket, key, key_index);
			if (old_value==value) {
				return this;
			}
			if (old_value!=null) {
				new_bucket = bucket.setAt(value, key_index.val+1);
			} else {
				new_size++;
				new_bucket = bucket.add(key);
				new_bucket = new_bucket.add(value);
			}
		} else {
			new_bucket = new CatFixedArray<Object>();
			new_size++;
			new_bucket = new_bucket.add(key);
			new_bucket = new_bucket.add(value);
		}
		CatFixedHashMap<K,V> result = new CatFixedHashMap<K, V>();
		CatIList<Object>[] bucket_list = _clone_buckets_list();
		bucket_list[bucket_index] = new_bucket;
		result.buckets = bucket_list;
		result.bucket_count = bucket_count;
		result.size = new_size;
		result.modification_count = modification_count+1;
		return _rehash_check(result);
	}



	public CatFixedHashMap<K,V> remove(K key, CatObjectPtr<V> value_removed) {
		if (bucket_count==0) {
			if (value_removed!=null) {
				value_removed.set(null);
			}
			return this;
		}
		int hash = key.hashCode();
		int bucket_index = hash % bucket_count;
		CatIList<Object> bucket = buckets[bucket_index];
		if (bucket==null) {
			if (value_removed!=null) {
				value_removed.set(null) ;
			}
			return this;
		}

		IntPointer key_index = new IntPointer();
		key_index.val = -1;
		V old_value = _get_from_bucket_list(bucket, key, key_index);
		if (key_index.val<0) {
			if (value_removed!=null) {
				value_removed.set(null) ;
			}
			return this;
		}

		if (value_removed!=null) {
			value_removed.set(old_value) ;
		}
		CatIList<Object> new_bucket = bucket.removeAt(key_index.val);
		new_bucket = new_bucket.removeAt(key_index.val);

		CatFixedHashMap<K,V> result = new CatFixedHashMap<K, V>();
		CatIList<Object>[] bucket_list = _clone_buckets_list();
		bucket_list[bucket_index] = new_bucket;
		result.buckets = bucket_list;
		result.bucket_count = bucket_count;
		result.size = size-1;
		result.modification_count = modification_count+1;
		return _rehash_check(result);
	}


	@SuppressWarnings("unchecked")
	public CatIList<K> createKeyList() {
		CatIList<K> result = new CatFixedArray<K>();
		int bucket_index;
		K entry;
		for(bucket_index=0; bucket_index<bucket_count; bucket_index++) {
			CatIList<Object> bucket = buckets[bucket_index];
			if (bucket!=null) {
				CatIIterator<Object> bucket_iter = bucket.iterator();
				while(bucket_iter.hasNext()) {
					entry = (K) bucket_iter.next();
					result = result.add(entry);
					bucket_iter.next();
				}
			}
		}
		return result;
	}


	@SuppressWarnings("unchecked")
	public CatArray<V> createValueList() {
		CatArray<V> result = new CatArray<V>();
		int bucket_index;
		V entry;
		for(bucket_index=0; bucket_index<bucket_count; bucket_index++) {
			CatIList<Object> bucket = buckets[bucket_index];
			if (bucket!=null) {
				CatIIterator<Object> bucket_iter = bucket.iterator();
				while(bucket_iter.hasNext()) {
					bucket_iter.next();
					entry = (V) bucket_iter.next();
					result.append(entry);
				}
			}
		}
		return result;
	}

	
	static class IntPointer {
		public int val;
	}
	
}


