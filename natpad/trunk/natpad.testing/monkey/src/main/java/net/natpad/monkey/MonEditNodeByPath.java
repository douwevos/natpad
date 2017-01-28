package net.natpad.monkey;

import static net.natpad.caterpillar.CatLogger.*;

import net.natpad.caterpillar.CatLogger;
import net.natpad.monkey.model.MooEditableNode;
import net.natpad.monkey.model.access.MooIdPath;
import net.natpad.monkey.transaction.Transaction;
import net.natpad.monkey.transaction.TxResult;
import net.natpad.worm.Request;

public abstract class MonEditNodeByPath extends Request {


	
	protected final MonkeyService monkeyService;
	protected final MooIdPath idPath;
	
	public MonEditNodeByPath(MonkeyService monkeyService, MooIdPath idPath) {
		this.monkeyService = monkeyService;
		this.idPath = idPath;
	}
	

	
	
	@Override
	public void runRequest() {
		Transaction transaction = monkeyService.createTransaction();
		transaction.begin();
		int  retryCnt = 0;
		while(retryCnt++<10) {	// TODO: report error when we didn't succeed after 10 retries
			transaction.begin();
			
			MooEditableNode editableRootNode = transaction.root.ensureEditable();
			MooEditableNode editableNode = idPath.createEditableNode(editableRootNode);
			if (editableNode==null) {
				cat_log_error("Path unresolvable:%p", idPath);
				// TODO: allow to recreate path based upon unique node id 
				return;
			}
			runModifications(editableNode);

			if (transaction.commitEditable(editableRootNode)!=TxResult.FAILED) {
				cat_log_error(" >> set success:");
				break;
			} else {
				cat_log_error(" >> failed set");
				try {
					Thread.sleep(100+retryCnt*200);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		
		}
	}
	
	
	public abstract void runModifications(MooEditableNode editableNode);

	
}
