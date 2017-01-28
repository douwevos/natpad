package net.natpad.caterpillar;

public class CatObjectPtr<T> {

	protected T t;
	
	
	public void set(T t) {
		this.t = t;
	}
	
	
	public T get() {
		return t;
	}
}
