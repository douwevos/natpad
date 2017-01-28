package net.natpad.avaj.ast;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

public class Modifiers implements Iterable<Modifier> {

	private final List<Modifier> modifiers = new ArrayList<Modifier>();
	
	public Modifiers(short val) {
		for(Modifier modifier : Modifier.values()) {
			if ((val & modifier.mask) != 0) {
				modifiers.add(modifier);
			}
		}
	}

	
	public Modifiers(Modifier ... modifiers) {
		if (modifiers!=null) {
			this.modifiers.addAll(Arrays.asList(modifiers));
			Collections.sort(this.modifiers, new Comparator<Modifier>() {
				public int compare(Modifier o1, Modifier o2) {
					return o1==o2 ? 0 : o1.priority<o2.priority ? -1 : 1;
				}
			});
		}
	}
	
	public boolean isInterface() {
		return find(Modifier.INTERFACE);
	}

	public boolean isPublic() {
		return find(Modifier.PUBLIC);
	}

	public boolean isStatic() {
		return find(Modifier.STATIC);
	}

	
	public boolean find(Modifier toFind) {
		for(Modifier modifier : modifiers) {
			if (modifier==toFind) {
				return true;
			}
		}
		return false;
	}

	
	@Override
	public Iterator<Modifier> iterator() {
		return modifiers.iterator();
	}

	
}
