package net.natpad.gross.definition.bnf;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class IdList {

	public final IdList idList;
	public final String id;
	
	public IdList(IdList idList, String id) {
		this.idList = idList;
		this.id = id;
	}

	public List<String> enlist() {
		ArrayList<String> enlisted = new ArrayList<String>();
		IdList tdl = this;
		while(tdl!=null) {
			enlisted.add(tdl.id);
			tdl = tdl.idList;
		}
		Collections.reverse(enlisted);
		return enlisted;
	}

}
