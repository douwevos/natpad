package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.util.Util;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeCast;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateCast extends AbstractIntermediateItem {

	public final DeclarationType declDescRef;
	
	public IntermediateCast(int mnemonicIndex, DeclarationType declDescRef) {
		super(mnemonicIndex);
		this.declDescRef= declDescRef;
	}
	
	
	@Override
	public void connectStageOne(IntermediateRunContext groupConnectRuntime) {
		SubCodeCast subCodeCast = new SubCodeCast(mnemonicIndex, declDescRef);
		Consumer consumer = new Consumer(this);
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		valueProducer.typeSet.add(new ValueTypeDefinition(declDescRef, ValueTypeDefinitionKind.CAST));

		subCodeCast.addConsumer(consumer);
		subCodeCast.setValueProducer(valueProducer);

		groupConnectRuntime.addAndRunSubCode(subCodeCast);
	}

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateCast) {
			IntermediateCast that = (IntermediateCast) otherNode;
			return Util.nullSafeEquals(declDescRef, that.declDescRef);
		}
		return false;
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateCast(mnemonicIndex, declDescRef);
	}
}
