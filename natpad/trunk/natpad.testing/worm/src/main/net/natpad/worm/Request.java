package net.natpad.worm;

import java.util.HashMap;

public class Request implements IConditionObserver {
	

	
	static int idCounter = 1000;
	
//	int id = idCounter++;
	public int id;
	boolean hasTimeOut;
	long timeOut;
	volatile boolean isNotified;
	Object lock;
	
	final HashMap<Condition, ConditionState> conditionMap = new HashMap<Condition, ConditionState>();
	
	public Request() {
		synchronized (Request.class) {
			id = idCounter;
			idCounter++;
//			System.out.println(id+" request="+getClass().getName());
		}
	}
	
	public void runRequest() {
	}
	
	public void addNotify(Object lock) {
		this.lock = lock;
	}
	
	public void setTimeOut(long timeOut) {
		hasTimeOut = true;
		this.timeOut = timeOut;
	}

	public void setTimeOutDelta(long timeOut) {
		hasTimeOut = true;
		this.timeOut = System.currentTimeMillis()+timeOut;
		
	}

	
	public boolean hasTimeOut() {
		return hasTimeOut;
	}


	public long getTimeOut() {
		return timeOut;
	}
	
	public void conditionReporting() {
		synchronized(lock) {
//			System.out.println("["+id+"] set notified");
			isNotified = true;
			lock.notifyAll();
		}
	}
	
	public void updateStates() {
		HashMap<Condition, ConditionState> localConditionMap = null;
		synchronized (conditionMap) {
			localConditionMap = new HashMap<Condition, ConditionState>(conditionMap);
		}

//		System.out.println("["+id+"] update holders");
		for(ConditionState holder : localConditionMap.values()) {
			holder.setHold();
		}
	}
	
	public void addCondition(Condition condition) {
		synchronized(conditionMap) {
			if (!conditionMap.containsKey(condition)) {
				ConditionState holder = new ConditionState(condition, this);
				conditionMap.put(condition, holder);
			}
		}
	}

	
	public void removeCondition(Condition condition) {
		synchronized (conditionMap) {
			conditionMap.remove(condition);
		}
	}

	public boolean testCondition(Condition condition) {
		boolean result = false;
		ConditionState holder = conditionMap.get(condition);
		if (holder!=null) {
			result = holder.wasSelected();
//			System.out.println("["+id+"] condition added to request: condition="+condition+", result="+result);
		} else {
//			System.out.println("["+id+"] condition not added to request: condition="+condition);
		}
		return result;
	}


	
	
}
