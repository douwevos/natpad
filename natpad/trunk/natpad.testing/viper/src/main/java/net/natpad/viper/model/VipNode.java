package net.natpad.viper.model;

import java.lang.reflect.Constructor;
import java.util.concurrent.atomic.AtomicInteger;


public final class VipNode {

	
	final int adres = (int) (Math.random()*0x7fffff);
	
	public long viperId;
	private VipNodeObservers observers;
	
	protected VipIResource resource;
	
	
	public AtomicInteger holdCounter;
	public AtomicInteger recursiveCounter;

	
	protected VipNode() {
	}

	public VipNode(long viperId, VipIResource resource) {
		this(viperId, resource, null, null);
	}


	
	public VipNode(long viperId, VipIResource resource, AtomicInteger holdCounter, AtomicInteger recursiveCounter) {
		this.viperId = viperId;
		this.observers = new VipNodeObservers();
		this.resource = resource;
		this.holdCounter = holdCounter==null ? new AtomicInteger() : holdCounter;
		this.recursiveCounter = recursiveCounter==null ? new AtomicInteger() : recursiveCounter;
	}

	
	private void cloneValues(VipNode source) {
		viperId = source.viperId;
		resource = source.resource;
		observers = source.observers;
		holdCounter = source.holdCounter;
		recursiveCounter = source.recursiveCounter;
	}
	
	
	public final VipNode cloneNode() {
		try {
			Constructor<? extends VipNode> constructor = getClass().getDeclaredConstructor();
			constructor.setAccessible(true);
			VipNode result = constructor.newInstance();
			result.cloneValues(this);
			return result;
		} catch(Exception e) {
			e.printStackTrace();
		}
		return this;
	}
	
	public long getViperId() {
		return viperId;
	}
	
	
	public String getName() {
		return resource.getName();
	}
	
	public VipIResource getResource() {
		return resource;
	}

	public VipNode setResource(VipIResource new_content) {
		if (new_content==resource) {
			return this;
		}
		VipNode result = new VipNode();
		result.cloneValues(this);
		result.resource = new_content;
		return result;
	}


	public AtomicInteger getHoldCounter() {
		return holdCounter;
	}
	
	public void holdUp() {
		holdCounter.incrementAndGet();
	}
	
	public void holdDown() {
		holdCounter.decrementAndGet();
	}
	
	public boolean shouldHold() {
		return holdCounter.get()>0;
	}

	public AtomicInteger getRecursiveCounter() {
		return recursiveCounter;
	}
	
	public void recursiveUp() {
		recursiveCounter.incrementAndGet();
	}
	
	public void recursiveDown() {
		recursiveCounter.decrementAndGet();
	}
	
	
	public boolean shouldScanRecursive() {
		return recursiveCounter.get()>0;
	}
	
		
	@Override
	public String toString() {
		String cname = getClass().getName();
		int lastIndexOf = cname.lastIndexOf(".");
		if (lastIndexOf>0) {
			cname = cname.substring(lastIndexOf+1);
		}
		return cname+"["+Integer.toHexString(adres)+", vip-id="+viperId+", hash="+(hashCode()%1000)+": content="+resource+", hold="+holdCounter.get()+", recurse="+recursiveCounter.get()+"]";
	}


	public VipNodeObservers getObserverInfo() {
		return observers;
	}

	
}
