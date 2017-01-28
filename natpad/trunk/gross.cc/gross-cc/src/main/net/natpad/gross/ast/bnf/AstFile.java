package net.natpad.gross.ast.bnf;

public class AstFile {

	public final TypeDefs typeDefs;
	public final AstLines astLines;

	public AstFile(TypeDefs typeDefs, AstLines astLines) {
		this.typeDefs = typeDefs;
		this.astLines = astLines;
	}

}
