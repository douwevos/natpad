package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeNewArray;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateNewArray extends AbstractIntermediateItem {

	DeclarationType declarationType;
	
	public IntermediateNewArray(int mnemonicIndex, DeclarationType declarationType) {
		super(mnemonicIndex);
		this.declarationType = declarationType.setDimCount(1);
	}
	
	
	@Override
	public void connectStageOne(IntermediateRunContext groupConnectRuntime) {
		SubCodeNewArray subCode = new SubCodeNewArray(mnemonicIndex, declarationType);
		Consumer dimConsumer = new Consumer(this);
		subCode.addConsumer(dimConsumer);
		groupConnectRuntime.pushValueConsumer(dimConsumer);
		
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		valueProducer.typeSet.add(new ValueTypeDefinition(declarationType, ValueTypeDefinitionKind.NEW));
		subCode.setValueProducer(valueProducer);
		groupConnectRuntime.pushValueProvider(valueProducer);
		groupConnectRuntime.add(subCode);
	}

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateNewArray) {
			return true;
		}
		return false;
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateNewArray(mnemonicIndex, declarationType);
	}
	
	

	@Override
	public String toString() {
		String result = TextUtil.getName(getClass());
		if (scope!=null) {
			result+="[scope="+scope+", declarationType="+declarationType+"]";
		}
		return result;
	}
}
