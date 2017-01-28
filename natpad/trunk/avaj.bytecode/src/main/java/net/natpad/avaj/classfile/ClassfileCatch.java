/**
 * 
 */
package net.natpad.avaj.classfile;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.util.TextUtil;

public class ClassfileCatch implements ICatchOrFinally {
	
	public final DeclarationType exceptionType;
	public final int handlePc;
	
	public ClassfileCatch(DeclarationType exceptionType, int handlePc) {
		this.exceptionType = exceptionType;
		this.handlePc = handlePc;
	}
	
	public int getHandlePc() {
	    return handlePc;
	}
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder("Catch[");
		buf.append("handlePc=").append(TextUtil.hex(handlePc, 2));
		buf.append(", exceptionType="+exceptionType);
		return buf.append("]").toString();
	}
}