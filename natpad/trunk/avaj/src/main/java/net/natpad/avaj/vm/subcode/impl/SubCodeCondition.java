package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IConditionalExpression;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.RelationalExpression;
import net.natpad.avaj.ast.WrappedConditionalExpression;
import net.natpad.avaj.classfile.bytecode.ConditionType;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.Consumer;

public class SubCodeCondition extends AbstractSubCode {

	
	public final ConditionType type;
	public final boolean withZero;
	
	
	public SubCodeCondition(int index, boolean withZero, ConditionType type) {
		super(index);
		this.withZero = withZero;
		this.type = type;
	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		Consumer leftConsumer = (Consumer) consumerList.get(1);
		Consumer rightConsumer = (Consumer) consumerList.get(0);

		IExpression rightExpression = rightConsumer.getExpression(null);
		IConditionalExpression outputExpression = null;
		if (withZero) {
			DeclarationType bestType = rightConsumer.getTypeSet().getBestType();
			if (bestType!=null && bestType.equals(DeclarationType.DEFAULT_BOOLEAN)) {
				IConditionalExpression condExpr = null;
				if (rightExpression instanceof IConditionalExpression) {
					condExpr = (IConditionalExpression) rightExpression;
				} else {
					condExpr = new WrappedConditionalExpression(rightExpression);
				}
				
				if (type==ConditionType.EQUAL) {
					outputExpression = condExpr.invert();
				} else if (type==ConditionType.NOT_EQUAL) {
					outputExpression = condExpr;
				}
				
				if (outputExpression!=null) {
					valueProducer.setExpression(outputExpression);
					return;
				}
			}
		}
		valueProducer.setExpression(new RelationalExpression(type, leftConsumer.getExpression(null), rightExpression));
	}

	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"[index="+mnemonic_index+", outputExpression="+valueProducer.getExpression(null)+"]";
	}
	
	


}
