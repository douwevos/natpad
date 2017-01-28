package net.natpad.avaj.ast;

import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.classfile.bytecode.ConditionType;

public class RelationalExpression implements IConditionalExpression {

	public final ConditionType conditionType;
	public final IExpression left;
	public final IExpression right;
	
	public RelationalExpression(ConditionType conditionType, IExpression left, IExpression right) {
		this.left = left;
		this.right = right;
		this.conditionType = conditionType;
	}
	
	@Override
	public IConditionalExpression invert() {
		return new RelationalExpression(conditionType.opposite(), left, right);
	}
	
	@Override
	public void write(Writer out) {
		left.write(out);
		out.print(conditionType.getText());
		right.write(out);
	}
	
}
