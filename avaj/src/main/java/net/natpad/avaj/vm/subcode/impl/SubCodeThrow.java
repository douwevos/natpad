package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.ThrowStatement;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeThrow extends AbstractSubCode {

	public SubCodeThrow(int mnemonicIndex) {
		super(mnemonicIndex);
	}

	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		ThrowStatement throwStatement = new ThrowStatement(consumerList.get(0).getExpression(null));
		modelRuntime.appendStatement(throwStatement);
	}

}
