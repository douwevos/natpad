package net.natpad.gross.ast.bnf;

public class AstTypeDescription extends TypeDescription {

	public final String optExtends;
	public final String optAsName;
	public final BodyStatements body;
	
	public AstTypeDescription(TypeDef typeDef, String optExtends, String optAsName, BodyStatements body) {
		super(typeDef);
		this.optExtends = optExtends;
		this.optAsName = optAsName;
		this.body = body;
	}
	
}
