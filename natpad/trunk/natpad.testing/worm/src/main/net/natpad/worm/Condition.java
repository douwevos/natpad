package net.natpad.worm;

import java.util.ArrayList;

public class Condition {

	public final ArrayList<IConditionObserver> selectorList = new ArrayList<IConditionObserver>();
	
	int select;
	
	public void up() {
		ArrayList<IConditionObserver> localList;
		synchronized (this) {
			select++;
			localList = new ArrayList<IConditionObserver>(selectorList);
		}
		for(IConditionObserver selector : localList) {
			selector.conditionReporting();
		}
	}
	
	
	public synchronized int registerSelector(IConditionObserver selector) {
		selectorList.add(selector);
		return select;
	}
	
	
	public synchronized int getSelect() {
		return select;
	}
	
	
}
