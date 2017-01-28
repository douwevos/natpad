package net.natpad.viper;

import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicReference;

import net.natpad.caterpillar.CatFixedString;
import net.natpad.caterpillar.concurrent.CatAtomicLong;
import net.natpad.caterpillar.versionedtree.CatTree;
import net.natpad.caterpillar.versionedtree.CatReadableTreeNode;
import net.natpad.caterpillar.versionedtree.CatWritableTreeNode;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.VipSnapshot;
import net.natpad.viper.model.access.VipPath;
import net.natpad.viper.model.file.VipFS;
import net.natpad.viper.model.file.VipFSMap;
import net.natpad.viper.model.file.VipSimpleFileMapper;
import net.natpad.viper.model.scanner.VipMapperRegistry;

public class ViperService implements VipISequence {

		
	ArrayList<IViperListener> listeners = new ArrayList<IViperListener>();
	
	CatAtomicLong viperIdGenerator = new CatAtomicLong();
	
	VipMapperRegistry mapperRegistry;
	
	AtomicReference<VipSnapshot> snapshot_ref = new AtomicReference<VipSnapshot>();
	
	VipQueue queue = new VipQueue();
	
	
	CatTree<VipNode> tree;
	
	public ViperService() {
		tree = new CatTree<VipNode>();
		VipSimpleFileMapper simpleFileMapper = new VipSimpleFileMapper(this);
		mapperRegistry = new VipMapperRegistry(simpleFileMapper);
	}

	
	public void start() {
		VipNode vipNode = new VipNode(viperIdGenerator.increment(), new VipFSMap(new VipFS(new VipPath(new CatFixedString("/")))));
		CatWritableTreeNode<VipNode> writable_root_node = tree.cat_tree_get_writable_root_node();
		writable_root_node.cat_writable_tree_node_set_content(vipNode);
		tree.cat_tree_submit(false);
		CatReadableTreeNode<VipNode> root_node = tree.cat_tree_get_readable_root_node_ref();
		VipSnapshot snapshot = new VipSnapshot(root_node);
		snapshot_ref.set(snapshot);
		
		VipScanRequest scan_request = new VipScanRequest(this);
		queue.post(scan_request);
	}
	
	public VipMapperRegistry getMapperRegistry() {
		return mapperRegistry;
	}
	
	public VipSnapshot getSnapshot() {
		return snapshot_ref.get();
	}

	
	public void addListener(IViperListener viperListener) {
		VipSnapshot snapshot = null;
		synchronized (this) {
			listeners.add(viperListener);
			snapshot = snapshot_ref.get();
		}
		
		viperListener.snapshotSet(snapshot);
	}
	
	public void post(VipIRequest request) {
		queue.post(request);
	}
	
	
	public void scan() {
		VipScanRequest vipScanRequest = new VipScanRequest(this);
		queue.post(vipScanRequest);
	}
	
	public VipCreatePathRequest createPath(VipPath full_path) {
		VipCreatePathRequest request = new VipCreatePathRequest(this, full_path);
		queue.post(request);
		return request;
	}
	
	public boolean setSnapshot(VipSnapshot newSnapshot) {
		IViperListener listeners[] = null;
		
		boolean result = false;
		
		while(true) {
			VipSnapshot vipSnapshot = snapshot_ref.get();
			if (vipSnapshot==newSnapshot) {
				return true;
			}
			if (vipSnapshot.version<newSnapshot.version) {
				if (snapshot_ref.compareAndSet(vipSnapshot, newSnapshot)) {
					result = true;
					break;
				} else {
					// TODO add retry_count
				}
			} else {
				break;
			}
		}
		
		if (result) {
			synchronized (this) {
				if (!this.listeners.isEmpty()) {
					listeners = new IViperListener[this.listeners.size()];
					this.listeners.toArray(listeners);
				}
			}
			if (listeners!=null) {
				for(IViperListener listener : listeners) {
					listener.snapshotSet(newSnapshot);
				}
			}
		}
		return result;
	}
	
	@Override
	public long next() {
		return viperIdGenerator.increment();
	}


	
	
	
}
