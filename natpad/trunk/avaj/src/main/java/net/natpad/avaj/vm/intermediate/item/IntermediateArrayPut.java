package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.classfile.bytecode.Type;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.impl.SubCodeArrayPut;

public class IntermediateArrayPut extends AbstractIntermediateItem {

	protected final Type storeType;
	
	public IntermediateArrayPut(int index, Type storeType) {
		super(index);
		this.storeType = storeType;
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		SubCodeArrayPut subCode = new SubCodeArrayPut(mnemonicIndex);
		Consumer valueConsumer = new Consumer(this);
		Consumer indexConsumer = new Consumer(this);
		Consumer mainConsumer = new Consumer(this);
		subCode.addConsumer(valueConsumer);
		subCode.addConsumer(indexConsumer);
		subCode.addConsumer(mainConsumer);
		modelRuntime.pushValueConsumer(valueConsumer);
		modelRuntime.pushValueConsumer(indexConsumer);
		modelRuntime.pushValueConsumer(mainConsumer);
//		modelRuntime.addCrossTypeMatcher("put-field", subCode.consumer.getTypeSet(), typeSet);
		modelRuntime.add(subCode);
	}
	
	@Override
	public boolean isSame(IIntermediateNode other) {
		if (other == this) {
			return true;
		}
		if (other instanceof IntermediateArrayPut) {
			return true;
		}
		return false;
	}
	
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateArrayPut(mnemonicIndex, storeType);
	}
}
