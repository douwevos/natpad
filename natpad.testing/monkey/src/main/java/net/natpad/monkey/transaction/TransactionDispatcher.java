package net.natpad.monkey.transaction;

import java.util.ArrayList;

public class TransactionDispatcher {

	ArrayList<MooITransactionListener> listeners = new ArrayList<MooITransactionListener>(); 
	
	public TransactionDispatcher() {
	}
	
	public void addTransactionListener(MooITransactionListener transactionListener) {
		listeners.add(transactionListener);
	}
	
	
	public ArrayList<MooITransactionListener> getListeners() {
		return listeners;
	}
	
}
