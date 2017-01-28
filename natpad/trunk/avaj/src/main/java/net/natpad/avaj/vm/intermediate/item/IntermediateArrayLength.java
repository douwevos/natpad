package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeArrayLength;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateArrayLength extends AbstractIntermediateItem {

	
	public IntermediateArrayLength(int index) {
		super(index);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		SubCodeArrayLength subCode = new SubCodeArrayLength(mnemonicIndex);
		Consumer arrayConsumer = new Consumer(this);
		subCode.addConsumer(arrayConsumer);
		
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		valueProducer.getTypeSet().add(new ValueTypeDefinition(DeclarationType.DEFAULT_INT, ValueTypeDefinitionKind.CONSTANT));
		subCode.setValueProducer(valueProducer);

		modelRuntime.addAndRunSubCode(subCode);
		
	}
	
	@Override
	public boolean isSame(IIntermediateNode other) {
		if (other==this) {
			return true;
		}
		if (other instanceof IntermediateArrayLength) {
			return true;
		}
		return false;
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateArrayLength(mnemonicIndex);
	}
	
	
}
