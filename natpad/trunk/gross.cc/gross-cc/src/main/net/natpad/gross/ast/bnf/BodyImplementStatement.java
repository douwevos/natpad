package net.natpad.gross.ast.bnf;

public class BodyImplementStatement implements BodyStatement {

	public final String ifaceTypeName;
	public final String ifaceInitText;
	
	public BodyImplementStatement(String ifaceTypeName, String ifaceInitText) {
		this.ifaceTypeName = ifaceTypeName;
		this.ifaceInitText = ifaceInitText;
	}

	
	
}
