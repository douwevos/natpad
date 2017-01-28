package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.ast.NewExpression;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeNew extends AbstractSubCode {
	
	public final DeclarationType declarationType;
	

	public SubCodeNew(int mnemonicIndex, DeclarationType declarationType) {
		super(mnemonicIndex);
		this.declarationType = declarationType;
	}

	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		DeclarationType finalDeclarationType = modelRuntime.importRepository.createFinalDeclarationType(this.declarationType);
		Identifier identifier = new Identifier(finalDeclarationType.referenceTypeName);
		valueProducer.setExpression(new NewExpression(identifier));
	}

	
	@Override
	public String toString() {
		return subCodeName()+"["+declarationType+", "+valueProducer+"]";
	}

}
