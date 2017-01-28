package net.natpad.monkey.model;

import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import net.natpad.monkey.model.access.MooINodeMatcher;

public final class MooNode implements MooINode {

	public long uniqueId;
	public String name;
	public MooContentMap contentMap;
	public int zorder;
	
	public NodeRecursiveCounter recursiveErrorCount;
	
	
	
	protected MooNode children[];
	
	public MooNode(long uniqueId, String name, MooNode[] children, MooContentMap contentMap, int zorder) {
		if (name ==null) {
			throw new RuntimeException();
		}
		this.uniqueId = uniqueId;
		this.name = name;
		this.children = children;
		this.contentMap = contentMap;
		this.zorder = zorder;
		this.recursiveErrorCount = new NodeRecursiveCounter();
		if (contentMap==null) {
			new Exception().printStackTrace();
			System.exit(0);
		}
	}
	
	protected MooNode() {
		name = "<not-set>";
	}

	
	protected void cloneValues(MooNode source) {
		uniqueId = source.uniqueId;
		name = source.name;
		children = source.children;
		contentMap = source.contentMap;
		zorder = source.zorder;
		recursiveErrorCount = source.recursiveErrorCount;
	}
	
	public final MooNode cloneNode(MooNode source) {
		MooNode result = this;
		try {
			Constructor<? extends MooNode> constructor = getClass().getDeclaredConstructor();
			constructor.setAccessible(true);
			result = constructor.newInstance();
			result.cloneValues(this);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

	public MooIContentMap getContentMap() {
		return contentMap;
	}

	
	@Override
	public <T extends MooIContent> T getContent(String key) {
		return (T) contentMap.get(key);
	}

	
	@Override
	public long getUniqueId() {
		return uniqueId;
	}

	@Override
	public String getName() {
		return name;
	}

	@Override
	public MooNode getOriginal() {
		return this;
	}
	


	public int count() {
		if (children!=null) {
			return children.length;
		}
		return 0;
	}

	public int findIndex(MooINodeMatcher nodeMatcher, int try_index) {
		int result = -1;
		if (children!=null) {
			if (try_index>=0 && try_index<children.length) {
				if (nodeMatcher.matches(children[try_index])) {
					result = try_index;
					return result;
				}
			}

			for(int idx=children.length-1; idx>=0; idx--) {
				if (nodeMatcher.matches(children[idx])) {
					result = idx;
					break;
				}
			}
		}
		return result;
	}

	
	public MooINode childAt(int idx) {
		if (children==null || idx<0 || idx>=children.length) {
			return null;
		}
		return children[idx];
	}

	@SuppressWarnings("unchecked")
	@Override
	public ArrayList<MooINode> enlistChildren(ArrayList<MooINode> children) {
		if (children==null) {
			children = new ArrayList<MooINode>();
		}
		if (this.children!=null) {
			List<?> asList = Arrays.asList(this.children);
			children.addAll((List<MooINode>) asList); 
		}
		return children;
	}
	
	@Override
	public MooEditableNode ensureEditable() {
		return new MooEditableNode(this, false);
	}
	
	
	public MooNode[] getChildren() {
		return children;
	}

	public MooNode setChildren(MooNode[] newChildren) {
		if (newChildren==children) {
			return this;
		}
		MooNode result = cloneNode(this);
		result.children = newChildren;
		return result;
	}

	public MooNode addChild(MooNode toAdd) {
		MooNode newChildren[];
		if (children==null) {
			newChildren = new MooNode[1];
			newChildren[0] = toAdd;
		} else {
			newChildren = new MooNode[children.length+1];
			System.arraycopy(children, 0, newChildren, 0, children.length);
			newChildren[children.length] = toAdd;
		}
		MooNode result = cloneNode(this);
		result.children = newChildren;
		return result;
	}


	public NodeRecursiveCounter getRecursiveErrorCount() {
		return recursiveErrorCount;
	}
	
	public MooNode setRecursiveErrorCount(NodeRecursiveCounter recursiveErrorCount) {
		if (this.recursiveErrorCount.equals(recursiveErrorCount)) {
			return this;
		}
		MooNode result = cloneNode(this);
		result.recursiveErrorCount = recursiveErrorCount;
		return result;
	}

	public int getErrorCount() {
		return recursiveErrorCount.foreignCount+recursiveErrorCount.localCount;
	}
	
	
	
	@Override
	public String toString() {
		String id = super.toString();
		int indexOf = id.indexOf('@');
		if (indexOf>0) {
			id = id.substring(indexOf);
		}
		return "MooNode["+id+",uniqueId="+uniqueId+", children="+(children==null ? -1 : children.length)+"]";
	}

	
}
