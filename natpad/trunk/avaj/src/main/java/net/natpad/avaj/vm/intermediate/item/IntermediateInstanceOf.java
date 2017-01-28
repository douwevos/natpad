package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeInstanceOf;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateInstanceOf extends AbstractIntermediateItem {

	public final DeclarationType declDescRef;
	
	public IntermediateInstanceOf(int mnemonicIndex, DeclarationType declDescRef) {
		super(mnemonicIndex);
		this.declDescRef= declDescRef;
	}
	
	
	@Override
	public void connectStageOne(IntermediateRunContext groupConnectRuntime) {
		SubCodeInstanceOf subCodeCast = new SubCodeInstanceOf(mnemonicIndex, declDescRef);
		Consumer consumer = new Consumer(this);
		subCodeCast.addConsumer(consumer);
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		valueProducer.getTypeSet().add(new ValueTypeDefinition(DeclarationType.DEFAULT_BOOLEAN, ValueTypeDefinitionKind.INSTANCEOF));
		subCodeCast.setValueProducer(valueProducer);
		groupConnectRuntime.addAndRunSubCode(subCodeCast);
	}

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateInstanceOf) {
			IntermediateInstanceOf that = (IntermediateInstanceOf) otherNode;
			return (declDescRef.equals(that.declDescRef));
		}
		return false;
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateInstanceOf(mnemonicIndex, declDescRef);
	}
}
