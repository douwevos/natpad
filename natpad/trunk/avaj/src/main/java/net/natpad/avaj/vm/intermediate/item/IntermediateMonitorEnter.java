package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateGroup;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.block.SubCodeSynchronizedBlock;

public class IntermediateMonitorEnter extends IntermediateGroup {

	
	boolean blockResolved = false;

	public IntermediateMonitorEnter() {
		super();
	}
	
	

	public void setResolved(boolean val) {
		blockResolved = val;
	}
	
	public boolean isResolved() {
		return blockResolved;
	}
	
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		Consumer consumer = new Consumer(this);

		IntermediateRunContext branchRuntime = modelRuntime.createBranch();

		SubCodeSynchronizedBlock synchronizedBlock = new SubCodeSynchronizedBlock(0, branchRuntime.subCodeBlock, consumer);
		modelRuntime.add(synchronizedBlock);
		branchRuntime.pushValueConsumer(consumer);
		
		super.connectStageOne(branchRuntime);

	}
	
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateMonitorEnter();
	}

//	@Override
//	public boolean isSame(IGroupNode otherNode) {		// TODO should this code be in or out ?
//		if (otherNode instanceof GroupMonitor) {
////			GroupMonitor that = (GroupMonitor) otherNode;
//			return true;
//		}
//		return false;
//	}

	
}
