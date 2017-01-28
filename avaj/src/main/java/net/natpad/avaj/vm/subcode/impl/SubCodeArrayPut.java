package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.ArrayAccessExpression;
import net.natpad.avaj.ast.Assignment;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeArrayPut extends AbstractSubCode {

	private final int CIDX_VALUE	= 0;
	private final int CIDX_INDEX	= 1;
	private final int CIDX_MAIN		= 2;
	
	ArrayAccessExpression arrayAccessExpr;
	
	public SubCodeArrayPut(int index) {
		super(index);
	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {

		IExpression mainExpression = consumerList.get(CIDX_MAIN).getExpression(null);
		
		IExpression indexExpression = consumerList.get(CIDX_INDEX).getExpression(DeclarationType.DEFAULT_INT);
		
		arrayAccessExpr = new ArrayAccessExpression(mainExpression, indexExpression);
		
		IExpression value = consumerList.get(CIDX_VALUE).getExpression(null);
		Assignment assignment = new Assignment(arrayAccessExpr, value);
		
		modelRuntime.appendStatement(assignment);
	}
	
}
