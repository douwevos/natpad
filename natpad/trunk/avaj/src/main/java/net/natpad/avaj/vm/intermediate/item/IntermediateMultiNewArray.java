package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeMultiNewArray;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateMultiNewArray extends AbstractIntermediateItem {

	private final int dimensions;
	private final DeclarationType descriptor;
	
	public IntermediateMultiNewArray(int mnemonicIndex, int dimensions, DeclarationType descriptor) {
		super(mnemonicIndex);
		if (descriptor==null) {
			throw new RuntimeException();
		}
		this.dimensions = dimensions;
		this.descriptor = descriptor;
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext groupConnectRuntime) {
		SubCodeMultiNewArray subCode = new SubCodeMultiNewArray(mnemonicIndex, dimensions, descriptor);
		for(int idx=0; idx<dimensions; idx++) {
			Consumer dimConsumer = new Consumer(this);
			subCode.addConsumer(dimConsumer);
			groupConnectRuntime.pushValueConsumer(dimConsumer);
		}
		
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		valueProducer.typeSet.add(new ValueTypeDefinition(descriptor.setDimCount(dimensions), ValueTypeDefinitionKind.NEW));
		subCode.setValueProducer(valueProducer);
		
		groupConnectRuntime.pushValueProvider(valueProducer);
		groupConnectRuntime.add(subCode);
		
	}

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateMultiNewArray) {
			return true;
		}
		return false;
	}

	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateMultiNewArray(mnemonicIndex, dimensions, descriptor);
	}
	
	
	@Override
	public String toString() {
		String result = TextUtil.getName(getClass());
		if (scope!=null) {
			result+="[scope="+scope+", descriptor="+descriptor+"]";
		}
		return result;
	}

	
}
