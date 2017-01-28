package net.natpad.monkey;

import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReference;

import net.natpad.monkey.model.MooContentMap;
import net.natpad.monkey.model.MooEditableNode;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.module.MooModuleTransactionListener;
import net.natpad.monkey.transaction.Transaction;
import net.natpad.monkey.transaction.TransactionDispatcher;
import net.natpad.monkey.transaction.TxResult;
import net.natpad.monkey.viper.MooNewSetSnapshotRequest;
import net.natpad.viper.IViperListener;
import net.natpad.viper.VipISequence;
import net.natpad.viper.ViperService;
import net.natpad.viper.model.VipSnapshot;
import net.natpad.worm.WormService;

public class MonkeyService implements IViperListener, VipISequence {

	private final WormService wormService;
	private final ViperService viperService;
	public final AtomicLong uniqueIdCounter = new AtomicLong();
	
	public TransactionDispatcher transactionDispatcher = new TransactionDispatcher();
	AtomicReference<MooNode> rootRef = new AtomicReference<MooNode>(new MooNode(uniqueIdCounter.incrementAndGet(), "<root>", null, new MooContentMap(), 0));
	
	AtomicReference<VipSnapshot> snapshotRef = new AtomicReference<VipSnapshot>();
	
	public MonkeyService(WormService wormService, ViperService viperService) {
		this.wormService = wormService;
		this.viperService = viperService;
		viperService.addListener(this);
		transactionDispatcher.addTransactionListener(new MooModuleTransactionListener(this));
	}


	
	public ViperService getViperService() {
		return viperService;
	}
	
	public WormService getWormService() {
		return wormService;
	}
	
	
	@Override
	public void snapshotSet(VipSnapshot newSnapshot) {
//		NewSnapshotRequest snapshotRequest = new NewSnapshotRequest(this, snapshotRef, newSnapshot);
//		wormService.postRequest(snapshotRequest);
		MooNewSetSnapshotRequest improvedSnapshotRequest = new MooNewSetSnapshotRequest(this, snapshotRef, newSnapshot);
		wormService.postRequest(improvedSnapshotRequest);
	}
	
	
	public boolean add_node(MooNode module_or_file_node) {
		Transaction transaction = createTransaction();
		int retryCnt = 0;
		boolean result = false;
		while(retryCnt++<10) {
			transaction.begin();
			MooEditableNode editableRootNode = transaction.root.ensureEditable();
			MooEditableNode addNewChild = editableRootNode.addNewChild(new MooEditableNode(module_or_file_node, true));
			
			if (transaction.commitEditable(editableRootNode)!=TxResult.FAILED) {
//				MooModuleRefreshViperRequest refreshViperRequest = new MooModuleRefreshViperRequest(this, module.getUniqueId());
//				wormService.postRequest(refreshViperRequest);
				
				result = true;
				break;
			} else {
				try {
					Thread.sleep(100+(retryCnt*200));
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
//		boolean result = false;
//		int retryCnt = 0;
//		while(retryCnt++<10) {
//			Node node = rootRef.get();
//			Node newRoot = node.addChild(module);
//			if (rootRef.compareAndSet(node, newRoot)) {
//				result = true;
//				break;
//			} else {
//				try {
//					Thread.sleep(100+(retryCnt*200));
//				} catch (InterruptedException e) {
//					e.printStackTrace();
//				}
//			}
//		}
		return result;
	}
	
	
	public Transaction createTransaction() {
		Transaction result = new Transaction(transactionDispatcher, uniqueIdCounter, rootRef);
		return result;
	}
	

	public MooNode getRoot() {
		return rootRef.get();
	}
	
	
	@Override
	public long next() {
		return uniqueIdCounter.incrementAndGet();
	}
	

}
