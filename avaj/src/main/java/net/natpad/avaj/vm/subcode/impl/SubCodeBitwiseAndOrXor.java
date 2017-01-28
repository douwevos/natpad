package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.IConditionalExpression;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.InfixExpression;
import net.natpad.avaj.ast.LogicalExpression;
import net.natpad.avaj.ast.WrappedConditionalExpression;
import net.natpad.avaj.classfile.bytecode.Operator;
import net.natpad.avaj.classfile.bytecode.Type;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.Consumer;

public class SubCodeBitwiseAndOrXor extends AbstractSubCode {
	
	public final Operator bitwiseOperator;
	
	public SubCodeBitwiseAndOrXor(int mnemonicIndex, Operator bitwiseOperator) {
		super(mnemonicIndex);
		this.bitwiseOperator = bitwiseOperator;
		
	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		Consumer leftConsumer = (Consumer) consumerList.get(1);
		Consumer rightConsumer = (Consumer) consumerList.get(0);
		
		IExpression leftExpr = (IExpression) leftConsumer.getExpression(null);
		IExpression rightExpr = (IExpression) rightConsumer.getExpression(null);
		if (leftExpr==null) {
			log.fail("leftExpr is null");
			return;
		}
		if (rightExpr==null) {
			log.fail("rightExpr is null");
			return;
		}
		
		if (((leftConsumer.getTypeSet().getBestType().type==Type.BOOLEAN) && (rightConsumer.getTypeSet().getBestType().type==Type.BOOLEAN)) ||
				valueProducer.getTypeSet().getBestType().type==Type.BOOLEAN)
		{
			IConditionalExpression leftCondExpr = WrappedConditionalExpression.ensureIsConditional(leftExpr);
			IConditionalExpression rightCondExpr = WrappedConditionalExpression.ensureIsConditional(rightExpr);
			
			valueProducer.setExpression(new LogicalExpression(bitwiseOperator, leftCondExpr, rightCondExpr));
		} else {
			valueProducer.setExpression(new InfixExpression(bitwiseOperator, leftExpr, rightExpr));
		}
	}


}
