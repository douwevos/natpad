package net.natpad.avaj.classfile;

import net.natpad.avaj.util.TextUtil;


public class ClassfileFinally implements ICatchOrFinally {
	
	public final int handlePc;
	
	public ClassfileFinally(int handlePc) {
		this.handlePc = handlePc;
	}
	
	
	public int getHandlePc() {
	    return handlePc;
	}
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder("Finally[");
		buf.append("handlePc=").append(TextUtil.hex(handlePc, 2));
		return buf.append("]").toString();
	}
}