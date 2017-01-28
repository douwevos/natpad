package net.natpad.monkey.model;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import java.util.ArrayList;

import net.natpad.monkey.model.access.MooINodeMatcher;

public final class MooEditableNode implements MooINode {

	
	MooNode originalNode;
	MooNode resultNode;
	
	ArrayList<MooINode> editedChildren;
	NodeRecursiveCounter editedRecursiveCount;

	MooIContentMap contentMap;
	
	
	public MooEditableNode(MooNode node, boolean initial) {
		originalNode = initial ? null : node;
		resultNode = node;
		if (node!=null) {
			contentMap = node.getContentMap();
		} else {
			System.err.println("no node-type-class defined");
			contentMap = new MooEditableContentMap(null);
			
		}
		if (originalNode!=null) {
			editedChildren = originalNode.enlistChildren(editedChildren);
		}
	}

	
	
	@Override
	public long getUniqueId() {
		return (resultNode!=null) ? resultNode.getUniqueId() : -1l;
	}
	
	@Override
	public String getName() {
		return resultNode==null ? null : resultNode.getName();
	}
	
	@Override
	public MooNode getOriginal() {
		return originalNode;
	}

	@Override
	public <T extends MooIContent> T getContent(String key) {
		return (T) contentMap.get(key);
	}
	
	public void setContent(MooIContent newContent) {
		newContent.markFixed();
		MooEditableContentMap editableContentMap = null;
		if (contentMap instanceof MooEditableContentMap) {
			editableContentMap = (MooEditableContentMap) contentMap;
		} else {
			editableContentMap = new MooEditableContentMap(((MooContentMap) contentMap).map);
			contentMap = editableContentMap;
		}
		editableContentMap.set(newContent);
	}

	
	
	public void markedForDeletion() {
		resultNode = null;
	}
	
	
	
	@Override
	public int count() {
		if (editedChildren!=null) {
			return editedChildren.size();
		} else if (originalNode!=null) {
			return originalNode.count();
		}
		return 0;
	}
	
	@Override
	public MooINode childAt(int idx) {
		if (editedChildren!=null) {
			return editedChildren.get(idx);
		} else if (originalNode!=null) {
			return originalNode.childAt(idx);
		}
		return null;
	}
	
	
	public int findIndex(MooINodeMatcher nodeMatcher, int try_index) {
		int result = -1;
		if (editedChildren==null) {
			if (originalNode!=null) {
				result = originalNode.findIndex(nodeMatcher, try_index);
			}
		} else {
			if (try_index>=0 && try_index<editedChildren.size()) {
				if (nodeMatcher.matches(editedChildren.get(try_index))) {
					result = try_index;
					return result;
				}
			}
			for(int idx=editedChildren.size()-1; idx>=0; idx--) {
				if (nodeMatcher.matches(editedChildren.get(idx))) {
					result = idx;
					idx = -1;
				}
			}
		}
		return result;
	}
	
	
	@Override
	public ArrayList<MooINode> enlistChildren(ArrayList<MooINode> children) {
		if (children==null) {
			children = new ArrayList<MooINode>();
		}
		if (editedChildren!=null) {
			children.addAll(editedChildren);
		} else if (originalNode!=null) {
			enlistChildren(children);
		}
		return children;
	}

	
	@Override
	public MooEditableNode ensureEditable() {
		return this;
	}
	
	public MooEditableNode getEditableChild(int childIdx) {
		if (editedChildren==null) {
			if (originalNode==null) {
				return null;
			}
			editedChildren = new ArrayList<MooINode>();
			originalNode.enlistChildren(editedChildren);
		}
		MooEditableNode result = editedChildren.get(childIdx).ensureEditable();
		editedChildren.set(childIdx, result);
		return result;
	}


	public MooEditableNode addNewChild(MooINode child) {
		if (editedChildren==null) {
			editedChildren = new ArrayList<MooINode>();
			if (originalNode!=null) {
				originalNode.enlistChildren(editedChildren);
			}
		}
		MooEditableNode realnode = child.ensureEditable();
		editedChildren.add(realnode);
		return realnode;
	}

	
	
	
	public MooNode finalizeResult() {
		if (resultNode!=null) {
			boolean contentDidChange = false;

			NodeRecursiveCounter errRecCounter = editedRecursiveCount!=null ? editedRecursiveCount : resultNode.getRecursiveErrorCount();
			
			
			
			MooContentMap contentMap = null;
			if (this.contentMap instanceof MooContentMap) {
				contentMap = (MooContentMap) this.contentMap;
			} else {
				contentMap = ((MooEditableContentMap) this.contentMap).finalizeResult();
			}
			
			if (originalNode!=null) {
				contentDidChange = !originalNode.getContentMap().equals(contentMap);
			} else {
				contentDidChange = true;
			}
			
			MooNode newChildren[] = originalNode!=null ? originalNode.getChildren() : null;
			

			if (editedChildren!=null) {
				boolean childrenDidChange = false;
				cat_log_debug("merging work");
	
				
				int errCount = 0;
				ArrayList<MooNode> resultChildren = new ArrayList<MooNode>(editedChildren.size());
				
//					for(int childIdx=editedChildren.size()-1; childIdx>=0; childIdx--) {
				for(int childIdx=0; childIdx<editedChildren.size(); childIdx++) {
					MooNode resolvedChild = null;
					MooINode editedChild = editedChildren.get(childIdx);
					if (editedChild instanceof MooEditableNode) {
						MooEditableNode ren = (MooEditableNode) editedChild;
						resolvedChild = ren.finalizeResult();
						if (resolvedChild==null) {
							continue;
						}
						childrenDidChange = childrenDidChange || (!resolvedChild.equals(ren.getOriginal()));
					} else {
						resolvedChild = (MooNode) editedChild;
					}
					
					resultChildren.add(resolvedChild);
					errCount += resolvedChild.getErrorCount();
				}
				
				/* check new count with old count */
				if (!childrenDidChange) {
					int oldCount = newChildren!=null ? newChildren.length : 0;
					childrenDidChange = resultChildren.size() != oldCount;
				}
				if (childrenDidChange) {
					newChildren = new MooNode[resultChildren.size()];
					newChildren = resultChildren.toArray(newChildren);
					contentDidChange = true;
				}
				
				errRecCounter = errRecCounter.setForeignCount(errCount);
			}
			
			if (originalNode!=null) {
				contentDidChange = contentDidChange || !errRecCounter.equals(originalNode.recursiveErrorCount);
			}
			
			
			if (contentDidChange) {
				resultNode = new MooNode(resultNode.uniqueId, resultNode.name, newChildren, contentMap, resultNode.zorder);
				resultNode = resultNode.setRecursiveErrorCount(errRecCounter);
			}
		}
		
		return (MooNode) resultNode;
	}







	
}
