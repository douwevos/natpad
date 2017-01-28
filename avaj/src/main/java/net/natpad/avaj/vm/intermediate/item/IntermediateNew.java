package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeNew;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateNew extends AbstractIntermediateItem {

	public final DeclarationType declarationType;
	
	public IntermediateNew(int index, DeclarationType declarationType) {
		super(index);
		this.declarationType = declarationType.setDimCount(0);;
	}
	
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateNew(mnemonicIndex, declarationType);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		SubCodeNew subCodeNew = new SubCodeNew(mnemonicIndex, declarationType);
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		valueProducer.typeSet.add(new ValueTypeDefinition(this.declarationType, ValueTypeDefinitionKind.NEW));
		
		subCodeNew.setValueProducer(valueProducer);
		modelRuntime.pushValueProvider(valueProducer);
		modelRuntime.add(subCodeNew);
	}

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateNew) {
			IntermediateNew that = (IntermediateNew) otherNode;
			return that.declarationType.equals(declarationType);
		}
		return false;
	}

	
}
