package net.natpad.sht.model;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class BlockValue extends Value implements Iterable<Reference> {

	TemplateCall templateCall;
	ArrayList<Reference> list = new ArrayList<Reference>();
	
	
	public BlockValue() {
		this.templateCall = null;
	}
	
	
	public void setTemplateCall(TemplateCall templateCall) {
		this.templateCall = templateCall;
	}
	
	
	public void add(Reference ref) {
		list.add(ref);
		
	}
	
	
	@Override
	public Iterator<Reference> iterator() {
		return list.iterator();
	}


	public TemplateCall getTemplateCall() {
		return templateCall;
	}


	public Reference getFirstReferenceById(String idToFind) {
		for(Reference ref : list) {
			if (idToFind.equals(ref.getId())) {
				return ref;
			}
		}
		return null;
	}


	public List<Reference> enlistReferenceById(String idToFind) {
		ArrayList<Reference> result = null;
		for(Reference ref : list) {
			if (idToFind.equals(ref.getId())) {
				if (result == null) {
					result = new ArrayList<Reference>();
				}
				result.add(ref);
			}
		}
		return result;
	}
	
	
	@Override
	public void dump(PrintStream out, String indent) {
		out.println(indent+"Block");
		indent = indent+"   ";
		for(Reference ref : list) {
			ref.dump(out, indent);
		}
	}

	
}
