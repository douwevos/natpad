package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.InstanceOfExpression;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeInstanceOf extends AbstractSubCode {

	public final DeclarationType declDescRef;
	
	public SubCodeInstanceOf(int index, DeclarationType declDescRef) {
		super(index);
		this.declDescRef = declDescRef;
	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		DeclarationType finalDeclarationType = modelRuntime.importRepository.createFinalDeclarationType(declDescRef);
		IExpression untestedExpression = consumerList.get(0).getExpression(null);
		valueProducer.setExpression(new InstanceOfExpression(finalDeclarationType, untestedExpression));
	}
	
	
}
