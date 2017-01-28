package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.Assignment;
import net.natpad.avaj.ast.ExpressionStatement;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.InfixExpression;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeIncrement extends AbstractSubCode {

	
	MethodFrameValue methodFrameValue;
	public final int value;
	
	
	public SubCodeIncrement(int index, MethodFrameValue methodFrameValue, int value) {
		super(index);
		this.methodFrameValue = methodFrameValue;
		this.value = value;

	}

	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
	
//		if (value==1) {
//			modelRuntime.appendStatement(new PostfixStatement(PostOrPrefixType.INCREMENT, expression));
//		} else if (value==-1) {
//			modelRuntime.appendStatement(new PostfixStatement(PostOrPrefixType.DECREASE, expression));
//		} else 
		
		System.out.println("SubCodeIncrement="+this+", valueProducer="+valueProducer);
		if ((valueProducer!=null) && !valueProducer.isConsumed()) {
			IExpression expression = valueProducer.getExpression(null);
			
			System.out.println("expression="+expression);
			// TODO describe the effect of the instanceof below
			if (expression instanceof InfixExpression) {
				Assignment assignment = new Assignment(methodFrameValue.getExpression(null), expression);
				modelRuntime.appendStatement(assignment);
			} else {
				modelRuntime.appendStatement(new ExpressionStatement(expression));
			}
		}
		
	}

}
