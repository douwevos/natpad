package net.natpad.monkey.model.module;

import net.natpad.monkey.MonkeyService;
import net.natpad.monkey.model.MooModuleContent;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.transaction.MooITransactionListener;
import net.natpad.monkey.transaction.Transaction;

public class MooModuleTransactionListener implements MooITransactionListener {

	private final MonkeyService monkeyService;
	
	public MooModuleTransactionListener(MonkeyService monkeyService) {
		this.monkeyService = monkeyService;
	}
	
	
	public void transactionCommited(Transaction transaction, MooNode newRootNode) {
		MooNode[] children = newRootNode.getChildren();
		if (children!=null) {
			for (MooNode child : children) {
				MooModuleContent moduleContent = child.getContent(MooModuleContent.KEY);
				if (moduleContent != null) {
					if (moduleContent.refreshViperInfo()) {
						MooModuleRefreshViperRequest request = new MooModuleRefreshViperRequest(monkeyService, child.getUniqueId());
						monkeyService.getWormService().postRequest(request);
					}
				}
			}
		}
	}
}
