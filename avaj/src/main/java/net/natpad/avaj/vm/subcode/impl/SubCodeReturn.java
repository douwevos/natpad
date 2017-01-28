package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.ReturnStatement;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeReturn extends AbstractSubCode {

	public final DeclarationType expectedReturnType;
	
	public SubCodeReturn(int mnemonicIndex, DeclarationType expectedReturnType) {
		super(mnemonicIndex);
		this.expectedReturnType = expectedReturnType;
	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		IExpression expression = null;
//		log.debug("return consumer="+consumer);
		if (expectedReturnType!=null) {
			expression = consumerList.get(0).getExpression(expectedReturnType);
		}
		modelRuntime.appendStatement(new ReturnStatement(expression));
	}
	
	

	@Override
	public String toString() {
		return subCodeName()+"["+expectedReturnType+", "+expectedReturnType+"]";
	}

	
}
