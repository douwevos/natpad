package net.natpad.avaj.vm.intermediate;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

import net.natpad.avaj.log.LogLevel;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;

public class IntermediateGroup implements Iterable<IIntermediateNode>, IIntermediateNode {

	static Logger log = Logger.getInstance(IIntermediateNode.class);
	
	public final List<IIntermediateNode> groupList = new ArrayList<IIntermediateNode>(); 


	private boolean scopeValid = false;
	private IntermediateNodeScope scope = new IntermediateNodeScope(-1, -1);
	
	public IntermediateGroup() {
	}

	public IntermediateGroup(IIntermediateNode ... newnodes) {
		groupList.addAll(Arrays.asList(newnodes));
	}

	public IntermediateGroup(List<IIntermediateNode> newnodes) {
		groupList.addAll(newnodes);
	}


	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		if (deep) {
			IntermediateGroup result = new IntermediateGroup();
			for(IIntermediateNode sub : groupList) {
				result.add(sub.makeCopy(deep));
			}
			return result;
		}
		return new IntermediateGroup(groupList);
	}
	
	public boolean add(IIntermediateNode groupNode) {
		if (groupNode==null) {
			throw new RuntimeException();
		}
		if (groupList.contains(groupNode)) {
			return false;
		}
		scopeValid = false;
		return groupList.add(groupNode);
	}

	public void add(int startIndex, IIntermediateNode group) {
		if (group==null) {
			throw new RuntimeException();
		}
		scopeValid = false;
		groupList.add(startIndex, group);
	}

	
	public void replace(int index, IIntermediateNode group) {
		groupList.set(index, group);
		scopeValid = false;
	}

	
	public IIntermediateNode get(int index) {
		return groupList.get(index);
	}
	
	public IIntermediateNode getLast() {
		if (groupList.isEmpty()) {
			return null;
		}
		return groupList.get(groupList.size()-1);
	}

	public void remove(IIntermediateNode toRemove) {
		scopeValid = false;
		groupList.remove(toRemove);
	}

	
	public IIntermediateNode removeTail() {
		IIntermediateNode result = null;
		while(result == null && !groupList.isEmpty()) {
			IIntermediateNode child = groupList.get(groupList.size()-1);
			if (child instanceof IntermediateGroup) {
				IntermediateGroup sub = (IntermediateGroup) child;
				result = sub.removeTail();
				if (result!=null) {
					if (sub.count()==0) {
						groupList.remove(groupList.size()-1);						
					}
					return result;
				}
			} else {
				result = child;
			}
			groupList.remove(groupList.size()-1);
		}
		return result;
	}
	
	public IIntermediateNode remove(int index) {
		scopeValid = false;
		return groupList.remove(index);
	}

	public int findIndex(IIntermediateNode node) {
		return groupList.indexOf(node);
	}
	
	public IIntermediateNode findNode(int jumpIndex) {
		for(IIntermediateNode node : groupList) {
			if (node.contains(jumpIndex)) {
				return node;
			}
		}
		return null;
	}

	

	
	public int count() {
		return groupList.size();
	}

	
//	public void setJumpIndex(int jumpIndex) {
//		scope.setJumpIndex(jumpIndex);
//	}

	@Override
	public IntermediateNodeScope getScope() {
		if (!scopeValid) {
			scopeValid = true;
			scope.jumpIndex = IntermediateNodeScope.INDEX_NONE;
			scope.exitIndex = IntermediateNodeScope.INDEX_NONE;
			if (!groupList.isEmpty()) {
				IIntermediateNode lastNode = groupList.get(groupList.size()-1);
				IntermediateNodeScope lastNodeScope = lastNode.getScope();
				scope.jumpIndex = lastNodeScope.jumpIndex;
				scope.exitIndex = lastNodeScope.exitIndex;
			}
		}
		return scope;
	}
	
	@Override
	public Iterator<IIntermediateNode> iterator() {
		return groupList.iterator();
	}

	@Override
	public boolean contains(int mnemonicIndex) {
		for(IIntermediateNode node : groupList) {
			if (node.contains(mnemonicIndex)) {
				return true;
			}
		}
		return false;
	}

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode == this) {
			return true;
		}
		if (otherNode instanceof IntermediateGroup) {
			IntermediateGroup that = (IntermediateGroup) otherNode;
			if (that.count()!=count()) {
				return false;
			}
			for(int idx=0; idx<count(); idx++) {
				IIntermediateNode t = that.get(idx);
				IIntermediateNode t2 = get(idx);
				if (!t2.isSame(t)) {
					return false;
				}
			}
			return true;
		}
		return false;
	}
	
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		modelRuntime.pseudoStack.clear();
		log.indent();
		for(IIntermediateNode intermediateNode : groupList) {
			log.debug(intermediateNode.toString());
			log.indent();
			try {
				intermediateNode.connectStageOne(modelRuntime);
			} catch(Throwable t) {
				t.printStackTrace();
			}
			modelRuntime.dumpStacks();
			log.dedent();
		}
		log.dedent();
	}
	

	@Override
	public boolean tryPrepend(IIntermediateNode node) {
		if (node==null) {
			throw new RuntimeException();
		}
		scopeValid = false;
		if (!groupList.isEmpty()) {
			IIntermediateNode firstNode = groupList.get(0);
			if (firstNode.tryPrepend(node)) {
				return true;
			}
		}
		
		groupList.add(0, node);
		return true;
	}
	
	@Override
	public void resolveBreaks(int jumpIndex) {
		for(IIntermediateNode groupNode : groupList) {
			groupNode.resolveBreaks(jumpIndex);
		}
	}

	
	public void dump(String prefix) {
		if (log.logEnabbledFor(LogLevel.DEBUG)) {
			StringBuilder dumpBuffer = new StringBuilder();
			dump(dumpBuffer, prefix);
			log.debug(dumpBuffer.toString());
		}
	}
	
	@Override
	public void dump(StringBuilder dumpBuffer, String prefix) {
		dumpBuffer.append(prefix).append(toString()).append(Logger.ln);
		prefix = prefix+"   ";
		for(IIntermediateNode node : groupList) {
			node.dump(dumpBuffer, prefix);
		}
	}
	
	@Override
	public String toString() {
		
		return TextUtil.getName(getClass())+"["+hashCode()+", scope="+getScope()+", size="+groupList.size()+"]";
	}


}
