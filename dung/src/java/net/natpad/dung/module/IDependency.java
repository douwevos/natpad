package net.natpad.dung.module;

import net.natpad.dung.expression.IExpressionValue;

public interface IDependency extends IExpressionValue {

	IDependency[] children();
	
	String description();
	
}
