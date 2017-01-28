package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.FieldAccessExpression;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeFieldGet extends AbstractSubCode {

	public final DeclarationType fieldSource;
	public final Identifier fieldName;
	public final boolean isNonStatic;
	
	
	public SubCodeFieldGet(int index, DeclarationType fieldSource, Identifier fieldName, boolean isNonStatic) {
		super(index);
		this.fieldSource = fieldSource;
		this.fieldName = fieldName;
		this.isNonStatic = isNonStatic;
		
	}

	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		
		IExpression accessExpr = null;
		if (isNonStatic) {
			IExpression expression = consumerList.get(0).getExpression(null);
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
		
		valueProducer.setExpression(accessExpr);
	}
	
	@Override
	public String toString() {
		return subCodeName()+"[src="+fieldSource+", "+fieldName+", "+valueProducer+"]";
	}

}
