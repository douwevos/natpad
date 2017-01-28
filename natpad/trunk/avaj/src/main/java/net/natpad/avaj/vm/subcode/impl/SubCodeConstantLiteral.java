package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeConstantLiteral extends AbstractSubCode {

	
	public SubCodeConstantLiteral(int index, IExpression defaultExpression) {
		super(index);
		if (defaultExpression==null) {
			throw new RuntimeException();
		}
	}
	
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
	}

	
}
