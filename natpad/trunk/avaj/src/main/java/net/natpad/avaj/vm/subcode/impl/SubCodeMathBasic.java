package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.InfixExpression;
import net.natpad.avaj.classfile.bytecode.Operator;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.IConsumer;

public class SubCodeMathBasic extends AbstractSubCode {


	public final Operator operator;
	
	
	public SubCodeMathBasic(int mnemonicIndex, Operator operator) {
		super(mnemonicIndex);
		this.operator = operator;
	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		IConsumer leftConsumer = consumerList.get(1);
		IConsumer rightConsumer = consumerList.get(0);

		IExpression leftCondExpr = leftConsumer.getExpression(null);
		IExpression rightCondExpr = rightConsumer.getExpression(null);
		if (leftCondExpr==null) {
			log.fail("leftCondExpr is null");
			return;
		}
		if (rightCondExpr==null) {
			log.fail("rightCondExpr is null");
			return;
		}

		log.debug("valueProducer.expressionHolder.expression set for "+valueProducer);
		valueProducer.setExpression(new InfixExpression(operator, leftCondExpr, rightCondExpr));
	}


}
