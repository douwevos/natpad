package net.natpad.dung.run;

import net.natpad.dung.expression.ExpressionHelper;
import net.natpad.dung.expression.IExpressionValue;
import net.natpad.dung.expression.generated.runtime.LrScanner;

public class DungExprHelper extends ExpressionHelper {

	private final Session session;
	
	public DungExprHelper(LrScanner scanner, Session session) {
		super(scanner);
		this.session = session;
	}
	
	
	public IExpressionValue getById(Object id) {
		if ("modules".equals(id)) {
			return session.context.moduleMap;
		} else if ("module".equals(id)) {
			return session.module;
		} else if ("workspace".equals(id)) {
			return session.getWorkspace();
		}
		
		return null;
	}
	
	
}
