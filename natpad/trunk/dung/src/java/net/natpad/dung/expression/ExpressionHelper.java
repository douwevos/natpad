package net.natpad.dung.expression;

import net.natpad.dung.expression.IExpressionValue;
import net.natpad.dung.expression.generated.runtime.LrParserContext;
import net.natpad.dung.expression.generated.runtime.LrScanner;

public abstract class ExpressionHelper extends LrParserContext implements IExpressionValue {

	public ExpressionHelper(LrScanner scanner) {
		super(scanner);
	}
	
}
