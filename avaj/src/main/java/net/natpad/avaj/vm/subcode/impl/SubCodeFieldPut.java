package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.Assignment;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.FieldAccessExpression;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.IConsumer;

public class SubCodeFieldPut extends AbstractSubCode {

	public final DeclarationType fieldSource;
	public final DeclarationType fieldDeclType;
	public final Identifier fieldName;
	public final boolean isNonStatic;
	
	public SubCodeFieldPut(int mnemonicIndex, DeclarationType fieldDeclType, DeclarationType fieldSource, Identifier fieldName, boolean isNonStatic) {
		super(mnemonicIndex);
		this.fieldSource = fieldSource;
		this.fieldDeclType = fieldDeclType;
		this.fieldName = fieldName;
		this.isNonStatic = isNonStatic;
	}
	
	
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		IConsumer fieldConsumer = consumerList.get(0);
		IConsumer sourceConsumer = consumerList.get(1);
		IExpression accessExpr = null;
		if (isNonStatic) {
			IExpression expression = sourceConsumer.getExpression(null);
			if (expression instanceof Identifier) {
				if ("this".equals(((Identifier) expression).text)) {
					accessExpr = fieldName;
				}
			}
			if (accessExpr==null) {
				accessExpr = new FieldAccessExpression(expression, fieldName);
			}
		} else {
			Identifier resolvedName = modelRuntime.createPlainName(fieldSource, null);
			accessExpr = new FieldAccessExpression(resolvedName, fieldName);
		}
		
		IExpression value = fieldConsumer.getExpression(null);
		Assignment assignment = new Assignment(accessExpr, value);
		modelRuntime.appendStatement(assignment);
	}
	
}
