package net.natpad.monkey.model;

import net.natpad.caterpillar.versionedtree.CatReadableTreeNode;
import net.natpad.viper.model.VipNode;

public class MooResourceContent implements MooIContent {

	public final static String KEY = "MooResourceContent";
	
	private volatile boolean isFixed = false;

	CatReadableTreeNode<VipNode> node;
	
	protected MooResourceContent() {
		super();
	}
	
	public MooResourceContent(CatReadableTreeNode<VipNode> node) {
//		this.vip_node = vip_node;
		this.node = node;
	}
	

	private MooResourceContent ensureEditable() {
		if (isFixed) {
			MooResourceContent copy = new MooResourceContent(node);
			return copy;
		}
		return this;
	}

	public void markFixed() {
		isFixed = true;
	}
	
	@Override
	public String getKey() {
		return KEY;
	}

	
	
	
	public VipNode getViperNode() {
		return node.cat_tree_node_get_content();
	}
	
	public MooResourceContent setNode(CatReadableTreeNode<VipNode> node) {
		if (this.node==node) {
			return this;
		}
		MooResourceContent result = ensureEditable();
		result.node = node;
		return result;
		
	}

	public CatReadableTreeNode<VipNode> get_node() {
		return node;
	}
	
}
