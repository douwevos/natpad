package net.natpad.gross.definition.bnf;

public class AstDescriptionList {

	public final AstDescriptionList link;
	public final AstDescription astDescription;
	
	public AstDescriptionList(AstDescriptionList link, AstDescription astDescription) {
		this.link = link;
		this.astDescription = astDescription;
	}
	
}
