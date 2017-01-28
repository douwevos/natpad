package net.natpad.avaj.vm.intermediate;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeCatch;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class ItermediateCatchGroup extends IntermediateGroup {

	DeclarationType exceptionType;
	
	public SubCodeCatch subCodeCatch;
	
	public ItermediateCatchGroup(DeclarationType exceptionType) {
		this.exceptionType = exceptionType;
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		IntermediateRunContext catchcontext = modelRuntime.createBranch();
		subCodeCatch = new SubCodeCatch(0, exceptionType,catchcontext.subCodeBlock);
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		valueProducer.typeSet.add(new ValueTypeDefinition(exceptionType, ValueTypeDefinitionKind.CATCHED_EXCEPTION));
		valueProducer.setExpression(new Identifier("exception"));
		subCodeCatch.setValueProducer(valueProducer);
		catchcontext.pushValueProvider(valueProducer);
		super.connectStageOne(catchcontext);
		
	}
	
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		ItermediateCatchGroup result = new ItermediateCatchGroup(exceptionType);
		for(IIntermediateNode child : this) {
			if (deep) {
				add(child.makeCopy(true));
			} else {
				add(child);
			}
		}
		return result;
	}

}
