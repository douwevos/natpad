package net.natpad.monkey.transaction;

import net.natpad.monkey.model.MooNode;

public interface MooITransactionListener {

	public void transactionCommited(Transaction transaction, MooNode newRootNode);
	
}
