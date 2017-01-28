package net.natpad.gross.ast.bnf;

public class BodyArgumentStatement implements BodyStatement {

	public final String typeName;
	public final String label;
	public final String value;
	
	public BodyArgumentStatement(String typeName, String label, String value) {
		this.typeName = typeName;
		this.label = label;
		this.value = value;
	}

}
