package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.LogicalExpression;
import net.natpad.avaj.classfile.bytecode.Operator;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.IConsumer;

public class SubCodeConditionSet extends AbstractSubCode {


	private final int out;
	private int parentOutIndex, finalOutIndex;
	
	public SubCodeConditionSet(int index, int out, int parentOutIndex, int finalOutIndex) {
		super(index);
		this.out = out;
		this.parentOutIndex = parentOutIndex;
		this.finalOutIndex = finalOutIndex;

	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		IExpression exprs[] = new IExpression[consumerList.size()];
		int idx = 0;
		for(IConsumer consumer : consumerList) {
			log.detail("exprs["+idx+"]="+consumer.getExpression(DeclarationType.DEFAULT_BOOLEAN)+", consumer="+consumer);
			exprs[idx++] = consumer.getExpression(DeclarationType.DEFAULT_BOOLEAN);
		}
		LogicalExpression result = new LogicalExpression(calculateType(), exprs);
		valueProducer.setExpression(result);
	}

	private boolean isLastAndInverted() {
		return parentOutIndex!=finalOutIndex;
	}

	
	private Operator calculateType() {
		return out==parentOutIndex^isLastAndInverted() ? Operator.LOGICAL_OR : Operator.LOGICAL_AND;
	}
		

	
	
}
