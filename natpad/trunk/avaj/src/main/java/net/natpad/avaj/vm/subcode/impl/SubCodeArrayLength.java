package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.ArrayLengthExpression;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.Consumer;

public class SubCodeArrayLength extends AbstractSubCode {
	
	public SubCodeArrayLength(int index) {
		super(index);
	}
	
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		Consumer arrayConsumer = (Consumer) consumerList.get(0);
		IExpression arrayExpression = arrayConsumer.getExpression(null);
		ArrayLengthExpression arrayLengthExpression = new ArrayLengthExpression(arrayExpression);
		valueProducer.setExpression(arrayLengthExpression);
	}
	
	
}
