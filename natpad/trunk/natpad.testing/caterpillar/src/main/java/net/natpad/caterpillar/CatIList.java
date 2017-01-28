package net.natpad.caterpillar;

import java.util.Comparator;

public interface CatIList<T> {


	CatIList<T> add(T object);
	CatIList<T> insertAt(T object, int index);
	CatIList<T> removeAt(int index);
	CatIList<T> setAt(T object, int index);
	T  getAt(int index);
	T  getLast();
	T  getFirst();
	int count();
	int modCount();
	boolean isEmpty();
	CatIIterator<T> iterator();
	CatIList<T> sort(Comparator<T> cmpFunc);
	
}
