package net.natpad.avaj.vm.intermediate;

import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.block.LoopBlock;

public class IntermediateLoopGroup extends IntermediateGroup {
	
	
	public boolean consumeCondition = true;
	
	public IntermediateLoopGroup(IIntermediateNode ... codes) {
		super(codes);
	}

	private IIntermediateNode getConditionBlock() {
		return getLast();
	}

	@Override
	public IntermediateNodeScope getScope() {
		IntermediateNodeScope lcScope = super.getScope();
		lcScope.setExitIndex(getConditionBlock().getScope().getExitIndex());
		lcScope.setJumpIndex(IntermediateNodeScope.INDEX_NONE);
		return lcScope;
	}
	
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		getConditionBlock().connectStageOne(modelRuntime);
		
		IntermediateRunContext branchRuntime = modelRuntime.createBranch();
		for(IIntermediateNode subCode : groupList) {
			subCode.connectStageOne(branchRuntime);
		}
		
		if (consumeCondition) {
			Consumer consumer = new Consumer(this, "LoopGroup.block");
			LoopBlock loopBlock = new LoopBlock(consumer, branchRuntime.subCodeBlock);
			modelRuntime.add(loopBlock);
			modelRuntime.pushValueConsumer(loopBlock.condition_consumer);
		}
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		IntermediateLoopGroup result = new IntermediateLoopGroup();
		if (deep) {
			for(IIntermediateNode child : groupList) {
				result.groupList.add(child.makeCopy(true));
			}
		} else {
			result.groupList.addAll(groupList);
		}
		return result;
	}
	
}
