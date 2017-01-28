package net.natpad.jaguar.monkey;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;


import net.natpad.caterpillar.CatLogLevel;
import net.natpad.caterpillar.CatLogLevels;
import net.natpad.jaguar.monkey.request.JagRequestApplyTransactionChanges;
import net.natpad.monkey.MonkeyService;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.transaction.MooITransactionListener;
import net.natpad.monkey.transaction.Transaction;
import net.natpad.worm.WormService;


@CatLogLevel(CatLogLevels.CAT_LOG_DEBUG)
public class JagMooseConnector implements MooITransactionListener {

	
	
	private final MonkeyService monkeyService;
	
	private final JagMooseContext context;
	
	
	
	
	public JagMooseConnector(MonkeyService monkeyService) {
		this.monkeyService = monkeyService;
		context = new JagMooseContext(monkeyService);
		
		monkeyService.transactionDispatcher.addTransactionListener(this);

//		
//		Transaction transaction = monkeyService.createTransaction();
//		transaction.begin();
//		EditableNode editableNode = EditableNode.ensureEditable(transaction.root);
		


		Transaction transaction = monkeyService.createTransaction();
		transaction.begin();
		this.transactionCommited(transaction, transaction.root);
	}

	
	private WormService getWormService() {
		return monkeyService.getWormService();
	}
	
	@Override
	public void transactionCommited(Transaction transaction, MooNode newRootNode) {
		cat_log_debug(transaction.txInfo+" : newRootNode="+newRootNode);
		JagRequestApplyTransactionChanges request = new JagRequestApplyTransactionChanges(context);
		getWormService().postRequest(request);
	}
	
	
	
}
