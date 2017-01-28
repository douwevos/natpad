package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateNodeScope;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.impl.SubCodeThrow;

public class IntermediateThrow extends AbstractIntermediateItem {

	
	public IntermediateThrow(int index) {
		super(index);
		getScope().setExitIndex(IntermediateNodeScope.INDEX_EXIT);
	}
	
	public IIntermediateNode makeCopy(boolean deep) {	
		return new IntermediateThrow(mnemonicIndex);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		SubCodeThrow subCode = new SubCodeThrow(mnemonicIndex);
		Consumer consumer = new Consumer(this);
		subCode.addConsumer(consumer);
		modelRuntime.pushValueConsumer(consumer);
		modelRuntime.add(subCode);
	}

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		return (otherNode instanceof IntermediateThrow);
	}

	
}
