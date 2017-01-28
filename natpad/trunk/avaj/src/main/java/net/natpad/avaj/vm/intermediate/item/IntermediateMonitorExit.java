package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;

public class IntermediateMonitorExit extends AbstractIntermediateItem {

	
	public IntermediateMonitorExit() {
		super(1);
	}
	
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		modelRuntime.pushValueConsumer(new Consumer(this));
	}
	
	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateMonitorExit) {
			return true;
		}
		return false;
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateMonitorExit();
	}
	
	
	
	
	
}
