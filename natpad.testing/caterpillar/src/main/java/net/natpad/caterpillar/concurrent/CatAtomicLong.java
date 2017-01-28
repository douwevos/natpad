package net.natpad.caterpillar.concurrent;

import java.util.concurrent.atomic.AtomicLong;

public class CatAtomicLong {

	
	private final AtomicLong internal;
	
	
	public CatAtomicLong() {
		internal = new AtomicLong();
	}
	
	public CatAtomicLong(long initial) {
		internal = new AtomicLong(initial);
	}
	

	public boolean compareSet(long expect, long set) {
		return internal.compareAndSet(expect, set);
	}

	public long get() {
		return internal.get();
	}
	
	public void set(long newVal) {
		internal.set(newVal);
	}
	
	public long add(long delta) {
		return internal.addAndGet(delta);
	}
	
	public long decrement() {
		return internal.decrementAndGet();
	}
	
	public long increment() {
		return internal.incrementAndGet();
	}
	
	
	
	
}
