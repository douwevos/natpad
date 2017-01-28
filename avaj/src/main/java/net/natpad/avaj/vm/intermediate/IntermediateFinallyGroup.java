package net.natpad.avaj.vm.intermediate;

import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.ExpressionHolder;
import net.natpad.avaj.vm.subcode.impl.SubCodeFinally;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

public class IntermediateFinallyGroup extends IntermediateGroup {

	
	public SubCodeFinally subCodeFinally;
	
	public IntermediateFinallyGroup() {
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		IntermediateRunContext finallycontext = modelRuntime.createBranch();
//		GroupConnectContext finallycontext = modelRuntime;
		subCodeFinally = new SubCodeFinally(0,finallycontext.subCodeBlock);
		
		DefaultValueProducer valueProducer = new DefaultValueProducer(this, ValueTypeDefinitionList.HIDDEN_EXCEPTION_TYPE_SET, new ExpressionHolder());
		valueProducer.setExpression(new Identifier("/* throw-expression */"));
		subCodeFinally.setValueProducer(valueProducer);
		finallycontext.pushValueProvider(valueProducer);
		
		super.connectStageOne(finallycontext);
	}

	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		IntermediateFinallyGroup result = new IntermediateFinallyGroup();
		for(IIntermediateNode sub : groupList) {
			if (deep) {
				result.add(sub.makeCopy(true));
			} else {
				result.add(sub);
			}
		}
		return result;
	}
	
}
