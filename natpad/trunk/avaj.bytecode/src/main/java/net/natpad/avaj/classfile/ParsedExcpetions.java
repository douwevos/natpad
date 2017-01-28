package net.natpad.avaj.classfile;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import net.natpad.avaj.ast.Name;


public class ParsedExcpetions implements Iterable<Name> {

	
	private final List<Name> list = new ArrayList<Name>();
	
	public void add(Name typeRef) {
		list.add(typeRef);
    }
	
	public int count() {
		return list.size();
	}
	
	public Name get(int index) {
		return list.get(index);
	}

	
	@Override
	public Iterator<Name> iterator() {
		return new Iterator<Name>() {

			Iterator<Name> internal = list.iterator();
			
			@Override
			public boolean hasNext() {
				return internal.hasNext();
			}
			
			@Override
			public Name next() {
				return internal.next();
			}
			
			@Override
			public void remove() {
				throw new UnsupportedOperationException();
			}
		};
	}
	
	
}
