package net.natpad.gross.ast.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class AstLines {

	public final AstLines link;
	public final AstLine line;
	
	public AstLines(AstLines link, AstLine line) {
		this.link = link;
		this.line = line;
	}

	public List<AstLine> enlist() {
		ArrayList<AstLine> result = new ArrayList<AstLine>();
		AstLines l = this;
		while(l!=null) {
			result.add(l.line);
			l = l.link;
		}
		Collections.reverse(result);
		return result;
	}
	
}
