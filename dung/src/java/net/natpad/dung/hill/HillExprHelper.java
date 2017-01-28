package net.natpad.dung.hill;

import net.natpad.dung.expression.ExpressionHelper;
import net.natpad.dung.expression.IExpressionValue;
import net.natpad.dung.expression.generated.runtime.LrScanner;

public class HillExprHelper extends ExpressionHelper {

	private final HillSession session;
	
	public HillExprHelper(LrScanner scanner, HillSession session) {
		super(scanner);
		this.session = session;
	}
	
	
	public IExpressionValue getById(Object id) {
//		if ("modules".equals(id)) {
//			return session.context.moduleMap;
//		} else if ("module".equals(id)) {
//			return session.module;
//		}
		return null;
	}
	
	
}
