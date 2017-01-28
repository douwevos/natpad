package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.ast.NewExpression;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeMultiNewArray extends AbstractSubCode {

	public final int dimensions;
	public final DeclarationType descriptor;
	
	
	public SubCodeMultiNewArray(int mnemonicIndex, int dimensions, DeclarationType descriptor) {
		super(mnemonicIndex);
		this.dimensions = dimensions;
		this.descriptor = descriptor;
	}
	
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		IExpression dimExprs[] = new IExpression[dimensions];
		for(int idx=0; idx<dimExprs.length; idx++) {
			dimExprs[dimExprs.length-1-idx] = consumerList.get(idx).getExpression(DeclarationType.DEFAULT_INT);
		}
		Identifier plainTypeName = modelRuntime.createPlainName(descriptor, null);
		valueProducer.setExpression(new NewExpression(plainTypeName, dimExprs));
	}
	

}
