package net.natpad.viper;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import java.io.IOException;
import java.util.ArrayList;

import net.natpad.caterpillar.CatArray;
import net.natpad.caterpillar.versionedtree.CatReadableTreeNode;
import net.natpad.caterpillar.versionedtree.CatTreeNode;
import net.natpad.caterpillar.versionedtree.CatWritableTreeNode;
import net.natpad.viper.model.VipIMap;
import net.natpad.viper.model.VipIResource;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.VipVirtualResource;
import net.natpad.viper.model.access.VipInvalidPath;
import net.natpad.viper.model.access.VipNodeNameMatcher;
import net.natpad.viper.model.access.VipNodePath;
import net.natpad.viper.model.access.VipPath;

public class VipCreatePathRequest implements VipIRequest {

	private final ViperService vip_service;
	private final VipPath full_path;
	
	private VipNodePath node_path;
	
	private boolean did_run = false;
	
	
	public VipCreatePathRequest(ViperService vip_service, VipPath full_path) {
		this.vip_service = vip_service;
		this.full_path = full_path;
	}
	
	@Override
	public void run() {
		try {
			node_path = open();
		} catch (VipInvalidPath e) {
			e.printStackTrace();
		}
		synchronized(this) {
			did_run = true;
			this.notifyAll();
		}
	}

	
	public synchronized VipNodePath wait_for_path() {
		while(!did_run) {
			try {
				wait();
			} catch (InterruptedException ignore) {
			}
		}
		return node_path;
	}
	
	
	public VipNodePath open() throws VipInvalidPath {
		CatWritableTreeNode<VipNode> root_node = vip_service.tree.cat_tree_get_writable_root_node();
		
		CatWritableTreeNode<VipNode> cdNode = root_node;
		
		ArrayList<VipNode> listed = new ArrayList<VipNode>(); 
		
		ArrayList<CatTreeNode<?, VipNode>> node_list = new ArrayList<CatTreeNode<?, VipNode>>();
		
		
		for(int idx=0; idx<full_path.count(); idx++) {
			
			final String entry = full_path.get(idx);
			cat_log_debug("entry="+entry);
			if (entry.isEmpty()) {
				continue;
			}
			
			int foundNode = -1;
			if (cdNode.cat_tree_node_child_count()>0) {
				foundNode = cdNode.cat_tree_node_find_child_index(new VipNodeNameMatcher(entry), -1);
			}
			
			
			CatWritableTreeNode<VipNode> childNode = null;
			if (foundNode>=0) {
				childNode = cdNode.cat_tree_node_child_at(foundNode);
			} else {
				VipNode vip_node = cdNode.cat_tree_node_get_content();
				VipIResource content = vip_node.getResource();
				VipIResource childContent = null;
				if (content instanceof VipIMap) {
					VipIMap map = (VipIMap) content;
					CatArray<VipIResource> enlisted = null;
					try {
						enlisted = map.enlist(false);
					} catch (IOException e) {
						e.printStackTrace();
					}
					if (enlisted!=null) {
						for (VipIResource vipIResource : enlisted) {
							
							String lname = vipIResource.getName();
							cat_log_debug("lname="+lname+", vipIResource="+vipIResource);
							if (entry.equals(lname)) {
								childContent = vipIResource;
								break;
							}
						}
					}
					if (childContent == null) {
						childContent = new VipVirtualResource(entry);
					}
				} else {
					// TODO invalid path if content is not VipVirtualResource
					childContent = new VipVirtualResource(entry);
				}
				
				vip_node = new VipNode(vip_service.next(), childContent);
				childNode = cdNode.cat_writable_tree_node_append_child();
				childNode.cat_writable_tree_node_set_content(vip_node);
			}
			listed.add(childNode.cat_tree_node_get_content());
			node_list.add(childNode);
			cdNode = childNode;
		}
		
		CatReadableTreeNode<VipNode> readableTreeNode = vip_service.tree.cat_tree_submit(false);
		
		if (readableTreeNode==null) {
			readableTreeNode=vip_service.tree.cat_tree_get_readable_root_node_ref();
		}
		
		
		ArrayList<CatReadableTreeNode<VipNode>> sl = new ArrayList<CatReadableTreeNode<VipNode>>(node_list.size());
		for(CatTreeNode<?, VipNode> n : node_list) {
			CatReadableTreeNode<VipNode> readable_node = readableTreeNode.cat_tree_node_get_node_for_location(n.cat_tree_node_get_location());
			sl.add(readable_node);
		}
		
		
		VipNode nodes[] = new VipNode[listed.size()];
		listed.toArray(nodes);
		return new VipNodePath(nodes, sl);
	}
	
	
	
}
