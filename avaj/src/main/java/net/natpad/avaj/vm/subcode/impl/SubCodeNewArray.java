package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.ast.NewExpression;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeNewArray extends AbstractSubCode {

	DeclarationType declarationType;

	public SubCodeNewArray(int mnemonicIndex, DeclarationType declarationType) {
		super(mnemonicIndex);
		this.declarationType = declarationType;
		
	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		IExpression dimExprs[] = new IExpression[1];
		dimExprs[0] = consumerList.get(0).getExpression(DeclarationType.DEFAULT_INT);
		Identifier plainTypeName = modelRuntime.createPlainName(declarationType, null);
		valueProducer.setExpression(new NewExpression(plainTypeName, dimExprs));
	}
	
	
}
