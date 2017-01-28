package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateGroup;
import net.natpad.avaj.vm.intermediate.IntermediateNodeScope;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeConditionSet;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateConditionSet implements IIntermediateNode {

	final IIntermediateNode groupList[];
	
	private final IntermediateNodeScope scope;
	private int out;
	private int parentOutIndex, finalOutIndex;
	
	

	public IntermediateConditionSet(int outIndex, IIntermediateNode ... groupNodes) {
		groupList = groupNodes;
		IIntermediateNode lastGroup = groupNodes[groupNodes.length-1];
		IntermediateNodeScope lcScope = lastGroup.getScope();
		int exitIndex = lcScope.getExitIndex();
		scope = new IntermediateNodeScope(exitIndex, lcScope.getJumpIndex());
		out = outIndex;
		setConditionType(exitIndex, exitIndex);
	}
	
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		IntermediateConditionSet result = null;
		if (deep) {
			IIntermediateNode copiedGroupList[] = new IIntermediateNode[groupList.length];
			for(int idx=0; idx<groupList.length; idx++) {
				copiedGroupList[idx] = groupList[idx].makeCopy(true);
			}
			result = new IntermediateConditionSet(out, copiedGroupList);
		} else {
			result = new IntermediateConditionSet(out, groupList);
		}
		result.setConditionType(parentOutIndex, finalOutIndex);
		return result;
	}
	
	public void setConditionType(int parentOutIndex, int finalOutIndex) {
		for(IIntermediateNode node : groupList) {
			if (node instanceof IntermediateConditionSet) {
				IntermediateConditionSet child = (IntermediateConditionSet) node;
				child.setConditionType(out, finalOutIndex);
			}
		}
		this.parentOutIndex = parentOutIndex;
		this.finalOutIndex = finalOutIndex;
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
	
	public boolean tryPrepend(IIntermediateNode node) {
		if (groupList.length!=0) {
			return groupList[0].tryPrepend(node);
		}
		return false;
	}
	
	@Override
	public IntermediateNodeScope getScope() {
		return scope;
	}
	
	public int getGroupOutIndex() {
		return out;
	}

//	final IGroupNode groupList[];
//	public final int jumpIndex, exitIndex;
//	public final int out;

	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateConditionSet) {
			IntermediateConditionSet that = (IntermediateConditionSet) otherNode;
			if (groupList.length!=that.groupList.length) {
				return false;
			}
			for(int idx=0; idx<groupList.length; idx++) {
				if (!groupList[idx].isSame(that.groupList[idx])) {
					return false;
				}
			}
			return true;
		}
		return false;
	}
	
	private boolean isLastAndInverted() {
		return parentOutIndex!=finalOutIndex;
	}
	
	
//	private ConditionalConsumer conditionalConsumer;

	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		SubCodeConditionSet subCode = new SubCodeConditionSet(-1, out, parentOutIndex, finalOutIndex);

		for(IIntermediateNode groupNode : groupList) {
			groupNode.connectStageOne(modelRuntime);
			boolean shouldInvert = false;
			if (groupNode instanceof IntermediateGroup) {
				shouldInvert = !(groupNode.getScope().getJumpIndex()!=parentOutIndex^isLastAndInverted());
			}
			ConditionalItemConsumer singleConsumer = new ConditionalItemConsumer(this, shouldInvert);
			
			subCode.addConsumer(singleConsumer);
			
		}
		
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		valueProducer.typeSet.add(new ValueTypeDefinition(DeclarationType.DEFAULT_BOOLEAN, ValueTypeDefinitionKind.CONDITION));
		
		
		subCode.setValueProducer(valueProducer);
		modelRuntime.addAndRunSubCode(subCode);
	}
	
	@Override
	public void resolveBreaks(int jumpIndex) {
		for(IIntermediateNode groupNode : groupList) {
			groupNode.resolveBreaks(jumpIndex);
		}
	}
	


	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"[scope="+scope+", out="+out+", parent-out="+parentOutIndex+", isLastAndInverted()="+isLastAndInverted()+"]";
	}
	
	
	@Override
	public void dump(StringBuilder dumpBuffer, String prefix) {
		dumpBuffer.append(prefix).append(toString()).append(Logger.ln);
		prefix+="   ";
		for(IIntermediateNode node : groupList) {
			node.dump(dumpBuffer, prefix);
		}
	}
	
}
