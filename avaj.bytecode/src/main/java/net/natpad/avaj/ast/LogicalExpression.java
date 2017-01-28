package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.classfile.bytecode.Operator;
import net.natpad.avaj.log.Logger;

public class LogicalExpression implements IConditionalExpression {

	Logger log = Logger.getInstance("ast");
	
	
	public final Operator operator;
	private final IExpression expressionList[];
	
	public LogicalExpression(Operator operator, IExpression ... expressions) {
		this.expressionList = expressions==null ? new IExpression[0] : expressions;
		this.operator = operator;
//		for(IConditionalExpression expr : expression) {
//			if (expr==null) {
//				throw new RuntimeException();
//			}
//		}
	}
	
	public int count() {
		return expressionList.length;
	}
	
	public IExpression get(int index) {
		if (index>=0 && index<expressionList.length) {
			return expressionList[index];
		}
		return null;
	}
	
	@Override
	public IConditionalExpression invert() {
		switch(operator) {
			case LOGICAL_AND :
			case LOGICAL_OR : {
				IExpression newSet[] = new IExpression[expressionList.length];
				for(int idx=newSet.length-1; idx>=0; idx--) {
					log.trace("expression[idx]="+expressionList[idx]);
					IConditionalExpression conditionalExpression = WrappedConditionalExpression.ensureIsConditional(expressionList[idx]);
					newSet[idx] = conditionalExpression.invert();
				}
				Operator resultType = null;
				if (operator==Operator.LOGICAL_AND) {
					resultType = Operator.LOGICAL_OR;
				} else if (operator==Operator.LOGICAL_OR) {
					resultType = Operator.LOGICAL_AND;
				}
				
				return new LogicalExpression(resultType, newSet);
			}
		}

		return UnaryExpression.create(this);
	}

	@Override
	public void write(Writer out) {
//		out.print("(");
		boolean isFirst = true;
		for(IExpression expr : expressionList) {
			if (isFirst) {
				isFirst = false;
			} else {
				out.print(" "+operator.getText()+" ");
			}
			expr.write(out);
		}
//		out.print(")");
	}
	
}
