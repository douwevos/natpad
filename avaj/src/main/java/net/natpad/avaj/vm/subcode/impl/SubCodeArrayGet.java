package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.ArrayAccessExpression;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.Consumer;

public class SubCodeArrayGet extends AbstractSubCode {

	public SubCodeArrayGet(int index) {
		super(index);
	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		Consumer indexConsumer = (Consumer) consumerList.get(0);
		Consumer instanceConsumer = (Consumer) consumerList.get(1);
		IExpression expression = instanceConsumer.getExpression(null);
		ArrayAccessExpression arrayAccessExpression = new ArrayAccessExpression(expression, indexConsumer.getExpression(DeclarationType.DEFAULT_INT));
		valueProducer.setExpression(arrayAccessExpression);
	}
	
	
}
