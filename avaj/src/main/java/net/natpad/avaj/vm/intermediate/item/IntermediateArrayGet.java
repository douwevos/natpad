package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.classfile.bytecode.Type;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeArrayGet;

public class IntermediateArrayGet extends AbstractIntermediateItem {


	/* TODO: what should we do with the loadType ?*/
	private final Type loadType;
	
	public IntermediateArrayGet(int index, Type loadType) {
		super(index);
		this.loadType = loadType;
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		SubCodeArrayGet subCode = new SubCodeArrayGet(mnemonicIndex);
		Consumer indexConsumer = new Consumer(this);
		subCode.addConsumer(indexConsumer);
		DefaultValueProducer producer = new DefaultValueProducer(this);
		Consumer instanceConsumer = new ArrayGetInstanceConsumer(this, producer);
		subCode.addConsumer(instanceConsumer);
		subCode.setValueProducer(producer);

		modelRuntime.addAndRunSubCode(subCode);
//		modelRuntime.pushValueConsumer(indexConsumer);
//		modelRuntime.pushValueConsumer(instanceConsumer);
//		modelRuntime.pushValueProvider(producer);
		modelRuntime.addValueTypeDefinitionList(producer.getTypeSet());
//		modelRuntime.add(subCode);
	}
	
	
	@Override
	public boolean isSame(IIntermediateNode other) {
		if (other==this) {
			return true;
		}
		if (other instanceof IntermediateArrayGet) {
			return true;
		}
		return false;
	}
	
	
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateArrayGet(mnemonicIndex, loadType);
	}
}
