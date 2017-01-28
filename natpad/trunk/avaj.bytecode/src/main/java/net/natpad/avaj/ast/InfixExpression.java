package net.natpad.avaj.ast;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.classfile.bytecode.Operator;

public class InfixExpression implements IExpression {


	IExpression expressions[];
	public final Operator infixOperation;
	
	public InfixExpression(Operator infixOperation, IExpression ... expressions) {
		this.infixOperation = infixOperation;
		this.expressions = expressions==null ? new IExpression[0] : expressions;
	}

	
	public List<IExpression> getExpressions() {
		ArrayList<IExpression> result = new ArrayList<IExpression>();
		result.addAll(Arrays.asList(expressions));
		return result;
	}
	
	
	@Override
	public void write(Writer out) {
//		out.print("(");
		boolean isFirst = true;
		for(IExpression expr : expressions) {
			if (isFirst) {
				isFirst = false;
			} else {
				switch(infixOperation) {
					case MULTIPLY : 
					case DIVIDE : 
					case ADD : 
					case SUB : 
					case REMAINDER : 
					case SHIFT_LEFT : 
					case SHIFT_RIGHT : 
					case UNSIGNED_SHIFT_RIGHT : 
					case LESS_THAN : 
					case GREATER_THAN : 
					case LESS_OR_EQUAL : 
					case GREATER_OR_EQUAL : 
					case INSTANCE_OF : 
					case EQUAL : 
					case NOT_EQUAL : 
					case BITWISE_AND : 
					case BITWISE_XOR : 
					case BITWISE_OR : 
					case LOGICAL_AND : 
					case LOGICAL_OR : {
						out.print(infixOperation.getText());
					} break;
					
				}
			}
			expr.write(out);
		}
//		out.print(")");
	}
}
