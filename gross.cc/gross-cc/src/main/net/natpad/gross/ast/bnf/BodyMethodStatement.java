package net.natpad.gross.ast.bnf;

public class BodyMethodStatement implements BodyStatement {

	public final String methodName;
	public final String codeText;
	public final String returnType;
	
	public BodyMethodStatement(String methodName, String codeText, String returnType) {
		this.methodName = methodName;
		this.codeText = codeText;
		this.returnType = returnType;
	}

}
