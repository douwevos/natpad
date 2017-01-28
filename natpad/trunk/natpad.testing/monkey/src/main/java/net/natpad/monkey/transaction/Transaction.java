package net.natpad.monkey.transaction;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReference;

import net.natpad.caterpillar.CatLogLevel;
import net.natpad.caterpillar.CatLogLevels;
import net.natpad.monkey.model.MooEditableNode;
import net.natpad.monkey.model.MooNode;

@CatLogLevel(CatLogLevels.CAT_LOG_DEBUG)
public class Transaction {

	
	TransactionDispatcher transactionDispatcher;
	public AtomicLong uniqueIdCounter;
	private final AtomicReference<MooNode> rootRef;
	
	public MooNode root;
	
	
	
	static AtomicLong infoCountGen = new AtomicLong();
	public String txInfo = "tx"+infoCountGen.incrementAndGet();

	
	public Transaction(TransactionDispatcher transactionDispatcher, AtomicLong uniqueIdCounter, AtomicReference<MooNode> rootRef) {
		this.transactionDispatcher = transactionDispatcher;
		this.uniqueIdCounter = uniqueIdCounter;
		this.rootRef = rootRef;
	}
	

	public String getCaller() {
		String className = "???";
		String methodName = "???";
		int lineNumber = 0;
		StackTraceElement[] stackTrace = new Exception().getStackTrace();
		for(int idx=0; idx<stackTrace.length; idx++) {
			if (stackTrace[idx].getClassName().indexOf("Transaction")>0) {
				continue;
			}
			className = stackTrace[idx].getClassName();
			lineNumber = stackTrace[idx].getLineNumber();
			methodName = stackTrace[idx].getMethodName();
			break;
		}
		return className+"."+methodName+"("+lineNumber+") ";
	}
	
	
	public void begin() {
		root = rootRef.get();
		cat_log_debug("starting %s: root=%s, caller=%s", txInfo, root, getCaller());
		return;
	}

	
	
	private void dispatchCommit() {
		ArrayList<MooITransactionListener> listeners = transactionDispatcher.getListeners();
		for (MooITransactionListener listener : listeners) {
			listener.transactionCommited(this, this.root);
		}
	}
	

	

	public TxResult commit(MooNode newRoot) {
		if (!"<root>".equals(newRoot.name)) {
			throw new RuntimeException("newRoot of incorrect type:"+newRoot);
		}
		TxResult txResult = TxResult.FAILED;
		boolean result = rootRef.compareAndSet(root, newRoot);
		cat_log_debug("committing %s: root=%s, newRoot=%s, result=%s", txInfo, root, newRoot, result);
		if (result) {
			if (root!=newRoot) {
				root = newRoot;
				txResult = TxResult.SUCCESS;
				dispatchCommit();
			} else {
				txResult = TxResult.SUCCESS_BUT_NO_CHANGES;
			}
//			dump("", System.out);
		}
		cat_log_debug("committed %s: tx-result=%s", txInfo, txResult);
		return txResult;
	}
	
	
	public void dump(String prefix, PrintStream out) {
	}

	public TxResult commitEditable(MooEditableNode editableRootNode) {
		MooNode newRoot = editableRootNode.finalizeResult();
		return commit(newRoot);
	}
	
}
