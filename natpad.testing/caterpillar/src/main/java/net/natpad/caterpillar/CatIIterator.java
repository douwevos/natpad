package net.natpad.caterpillar;

import java.util.Iterator;

public interface CatIIterator<T> extends Iterator<T> {
	
	
	T next();
	boolean hasNext();
	boolean isLast();
	
}
