package net.natpad.viper.model;

import java.util.concurrent.atomic.AtomicInteger;

public class VipNodeObservers {

	AtomicInteger refCount = new AtomicInteger();

	
	
	public boolean hasReferences() {
		return refCount.get()>0;
	}
	
	
	public void reference() {
		refCount.incrementAndGet();
	}

	public void unreference() {
		refCount.decrementAndGet();
	}

	
}
