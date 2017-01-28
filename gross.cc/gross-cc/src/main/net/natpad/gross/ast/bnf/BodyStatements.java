package net.natpad.gross.ast.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class BodyStatements {

	public final BodyStatements link;
	public final BodyStatement bodyStatement;
	
	public BodyStatements(BodyStatements link, BodyStatement bodyStatement) {
		this.link = link;
		this.bodyStatement = bodyStatement;
	}

	public List<BodyStatement> enlist() {
		ArrayList<BodyStatement> result = new ArrayList<BodyStatement>();
		BodyStatements bstmts = this;
		while(bstmts!=null) {
			result.add(bstmts.bodyStatement);
			bstmts = bstmts.link;
		}
		Collections.reverse(result);
		return result;
	}
	
}
