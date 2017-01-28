package net.natpad.monkey.model.access;

import java.util.concurrent.atomic.AtomicReference;

import net.natpad.monkey.model.MooEditableNode;
import net.natpad.monkey.model.MooINode;

public class MooIdPath {

	private final long[] ids;
	private AtomicReference<int[]> indexCache = new AtomicReference<int[]>();
	
	
	public MooIdPath(long[] ids) {
		this.ids = ids;
		int cache[] = new int[ids.length];
		indexCache.set(cache);
	}
	
	
	
	public MooEditableNode createEditableNode(MooEditableNode rootNode) {
		int[] cache = indexCache.get();
		boolean cacheUpdated = false;
		for(int idx=0; idx<ids.length; idx++) {
			long nextId = ids[idx];
			MooINode childAt = rootNode.childAt(cache[idx]);
			if (childAt!=null && childAt.getUniqueId()==nextId) {
				rootNode = rootNode.getEditableChild(cache[idx]);
			} else {
				MooEditableNode nextNode = null;
				for(int cidx=rootNode.count()-1; cidx>=0; cidx--) {
					childAt = rootNode.childAt(cidx);
					if (childAt!=null && childAt.getUniqueId()==nextId) {
						nextNode = rootNode.getEditableChild(cidx);
						if (!cacheUpdated) {
							cacheUpdated = true;
							int newcache[] = new int[cache.length];
							System.arraycopy(cache, 0, newcache, 0, cache.length);
							cache = newcache;
						}
						cache[idx] = cidx;
						cidx = -1;
					}
				}
				if (nextNode==null) {
					return null;
				}
				rootNode = nextNode;
			}
		}
		if (cacheUpdated) {
			indexCache.set(cache);
		}
		return rootNode;
	}



	public MooIdPath addId(long uniqueId, int idx) {
		long nids[] = new long[this.ids.length+1];
		System.arraycopy(ids, 0, nids, 0, ids.length);
		nids[ids.length] = uniqueId;
		MooIdPath result = new MooIdPath(nids);
		int[] ocache = indexCache.get();
		int[] ncache = result.indexCache.get();
		System.arraycopy(ocache, 0, ncache, 0, ocache.length);
		ncache[ocache.length] = idx;
		return result;
	}



	public MooIdPath addMultipleIds(long[] srcMainJavaPath) {
		if (srcMainJavaPath==null || srcMainJavaPath.length==0) {
			return this;
		}
		long nids[] = new long[this.ids.length+srcMainJavaPath.length];
		System.arraycopy(ids, 0, nids, 0, ids.length);
		System.arraycopy(srcMainJavaPath, 0, nids, ids.length, srcMainJavaPath.length);
		return new MooIdPath(nids);
	}
	
	
}
