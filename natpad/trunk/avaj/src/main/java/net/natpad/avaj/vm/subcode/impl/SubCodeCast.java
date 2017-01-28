package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.CastExpression;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeCast extends AbstractSubCode {

	
	public final DeclarationType declDescRef;
	
	public SubCodeCast(int index, DeclarationType declDescRef) {
		super(index);
		this.declDescRef = declDescRef;
	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		DeclarationType createDeclarationType = declDescRef;
		DeclarationType finalDeclarationType = modelRuntime.importRepository.createFinalDeclarationType(createDeclarationType);
		IExpression uncastedExpression = consumerList.get(0).getExpression(null);
		CastExpression castExpression = new CastExpression(finalDeclarationType, uncastedExpression);
		valueProducer.setExpression(castExpression);
	}
	
	
}
