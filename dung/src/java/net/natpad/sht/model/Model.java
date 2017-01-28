package net.natpad.sht.model;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

public class Model implements MapValue {

	HashMap<String, Template> map = new HashMap<String, Template>();
	
	private ArrayList<Reference> references = new ArrayList<Reference>();
	
	public void add(Template template) {
		map.put(template.getName(), template);
	}

	public void add(Reference ref) {
		references.add(ref);
	}
	
	
	public List<Reference> getByRefId(String refId) {
		ArrayList<Reference> result = new ArrayList<Reference>();
		for(Reference ref : references) {
			if (ref.getId().equals(refId)) {
				result.add(ref);
			}
		}
		return result;
	}
	
	public BlockValue resolve(BlockValue postValue) {
		TemplateCall call = postValue.getTemplateCall();
		if (call!=null) {
			Template template = map.get(call.getTemplateName());
			if (template!=null) {
				BlockValue result = new BlockValue();
				int idx = 0;
				for(TemplateArgument argument : template) {
					Value callValue = call.getValueAt(idx);
					if (callValue==null) {
						callValue = argument.getDefaultValue();
					}
					idx++;
					String refid = argument.getRefId();
					Reference ref = new Reference(refid);
					ref.setValue(callValue);
					result.add(ref);
				}
				for(Reference ref : postValue) {
					result.add(ref);
				}
				return result;
			}
		}
		return postValue;
	}
	
	
	@Override
	public Iterator<Reference> iterator() {
		return references.iterator();
	}
	
	public Reference getFirstReferenceById(String id) {
		for(Reference ref : references) {
			if (id.equals(ref.getId())) {
				return ref;
			}
		}
		return null;
	}
	
	public List<Reference> enlistReferenceById(String idToFind) {
		ArrayList<Reference> result = null;
		for(Reference ref : references) {
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
	public String toString() {
		StringBuilder buf = new StringBuilder();
		
		for(Reference ref : references) {
			buf.append(ref).append("\n");
		}
		return buf.toString();
	}

	public void dump(PrintStream out) {
		for(Reference ref : references) {
			ref.dump(out, "   ");
		}
	}
	
}
