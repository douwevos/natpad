package net.natpad.avaj.vm.intermediate;

import net.natpad.avaj.log.Logger;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.block.SubCodeBlock;
import net.natpad.avaj.vm.subcode.block.SubCodeForLoop;

public class IntermediateForLoop implements IIntermediateNode {

	
	IIntermediateNode initNode;
	IIntermediateNode conditionNode;
	IIntermediateNode incrementNode;
	IIntermediateNode loopNode;

	public IntermediateForLoop(IIntermediateNode initNode, IIntermediateNode conditionNode, IIntermediateNode incrementNode, IIntermediateNode loopNode) {
		this.initNode = initNode;
		this.conditionNode = conditionNode;
		this.incrementNode = incrementNode;
		this.loopNode = loopNode;
	}
	
	
	@Override
	public void connectStageOne(IntermediateRunContext groupConnectRuntime) {
		IntermediateRunContext branchRuntime = groupConnectRuntime.createBranch();
		initNode.connectStageOne(branchRuntime);
		SubCodeBlock initBlock = branchRuntime.subCodeBlock;
		branchRuntime.subCodeBlock = new SubCodeBlock();
		branchRuntime.pseudoStack.clear();
		loopNode.connectStageOne(branchRuntime);
		SubCodeBlock bodyBlock = branchRuntime.subCodeBlock;
		branchRuntime.subCodeBlock = new SubCodeBlock();
		branchRuntime.pseudoStack.clear();
		incrementNode.connectStageOne(branchRuntime);
		SubCodeBlock incrementBlock = branchRuntime.subCodeBlock;
		branchRuntime.subCodeBlock = new SubCodeBlock();
		branchRuntime.pseudoStack.clear();
		conditionNode.connectStageOne(branchRuntime);
		SubCodeBlock conditionBlock = branchRuntime.subCodeBlock;
		
		Consumer consumer = new Consumer(this, "ForLoopGroup.condition");
		
		SubCodeForLoop subCodeForLoop = new SubCodeForLoop(initBlock, conditionBlock, consumer, incrementBlock, bodyBlock);
		groupConnectRuntime.add(subCodeForLoop);
		branchRuntime.pushValueConsumer(consumer);
	}

	
	
	@Override
	public boolean contains(int mnemonicIndex) {
		return initNode.contains(mnemonicIndex) ||
				loopNode.contains(mnemonicIndex) ||
				incrementNode.contains(mnemonicIndex) ||
				conditionNode.contains(mnemonicIndex);
	}

	
	
	@Override
	public void dump(StringBuilder dumpBuffer, String prefix) {
		dumpBuffer.append(prefix).append(toString()).append(Logger.ln);
		prefix+="   ";
		String prefixSub = prefix+"   ";

		dumpBuffer.append(prefix).append("for-init").append(Logger.ln);
		initNode.dump(dumpBuffer, prefixSub);
		
		dumpBuffer.append(prefix).append("for-condition").append(Logger.ln);
		conditionNode.dump(dumpBuffer, prefixSub);

		dumpBuffer.append(prefix).append("for-increment").append(Logger.ln);
		incrementNode.dump(dumpBuffer, prefixSub);

		dumpBuffer.append(prefix).append("for-content").append(Logger.ln);
		loopNode.dump(dumpBuffer, prefixSub);

	}
	
	
	@Override
	public IntermediateNodeScope getScope() {
		IntermediateNodeScope lcScope = new IntermediateNodeScope(-1,-1);
		lcScope.setExitIndex(conditionNode.getScope().getExitIndex());
		lcScope.setJumpIndex(IntermediateNodeScope.INDEX_NONE);
		return lcScope;
	}

	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode==this) {
			return true;
		}
		if (otherNode instanceof IntermediateForLoop) {
			IntermediateForLoop that = (IntermediateForLoop) otherNode;
			return that.initNode.isSame(initNode) &&
					that.conditionNode.isSame(conditionNode) &&
					that.incrementNode.isSame(incrementNode) &&
					that.loopNode.isSame(loopNode);
		}
		return false;
	}
	
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateForLoop(initNode, conditionNode, incrementNode, loopNode);
	}
	
	@Override
	public void resolveBreaks(int jumpIndex) {
		initNode.resolveBreaks(jumpIndex);
		conditionNode.resolveBreaks(jumpIndex);
		incrementNode.resolveBreaks(jumpIndex);
		loopNode.resolveBreaks(jumpIndex);
	}



	@Override
	public boolean tryPrepend(IIntermediateNode node) {
		return initNode.tryPrepend(node);
	}

}
