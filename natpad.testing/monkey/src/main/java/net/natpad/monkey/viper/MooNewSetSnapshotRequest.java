package net.natpad.monkey.viper;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import java.util.HashSet;
import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReference;

import net.natpad.caterpillar.CatLogLevel;
import net.natpad.caterpillar.CatLogLevels;
import net.natpad.caterpillar.versionedtree.CatReadableTreeNode;
import net.natpad.monkey.MonkeyService;
import net.natpad.monkey.model.MooEditableNode;
import net.natpad.monkey.model.MooINode;
import net.natpad.monkey.model.MooModuleContent;
import net.natpad.monkey.model.module.MooModuleRefreshViperRequest;
import net.natpad.monkey.transaction.Transaction;
import net.natpad.monkey.transaction.TxResult;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.VipSnapshot;
import net.natpad.worm.Request;
import net.natpad.worm.WormService;

@CatLogLevel(CatLogLevels.CAT_LOG_DEBUG)
public class MooNewSetSnapshotRequest extends Request {


	MonkeyService monkeyService;
	private final AtomicReference<VipSnapshot> snapshotRef;
	public final VipSnapshot snapshot;

	static AtomicLong infoCountGen = new AtomicLong();
	long infoCount = infoCountGen.incrementAndGet();
	
	public MooNewSetSnapshotRequest(MonkeyService monkeyService, AtomicReference<VipSnapshot> snapshotRef, VipSnapshot snapshot) {
		this.monkeyService = monkeyService;
		this.snapshotRef = snapshotRef;
		this.snapshot = snapshot;
		
		setTimeOutDelta(150);
		
	}

	

	@Override
	public void runRequest() {
		Transaction transaction = monkeyService.createTransaction();
		int  retryCnt = 0;
		HashSet<MooINode> nodesWhichNeedRefresh = new HashSet<MooINode>(); 
		while(retryCnt++<10) {	// TODO: report error when we didn't succeed after 10 retries
			nodesWhichNeedRefresh.clear();
			transaction.begin();
			VipSnapshot currentSnapshot = snapshotRef.get();
			cat_log_debug(infoCount+" >> currentSnapshot="+currentSnapshot+", snapshot="+snapshot+", snapshotRef="+snapshotRef.hashCode());
			if (currentSnapshot!=snapshot || retryCnt>1) {
				if (currentSnapshot!=null && snapshot!=null) {
					if (currentSnapshot.version>snapshot.version) {
						break;
					}
				}
				boolean compareAndSet = snapshotRef.compareAndSet(currentSnapshot, snapshot);
				cat_log_debug(infoCount+" >> compareAndSet="+compareAndSet);
				if (!compareAndSet) {
					continue;
				}
				currentSnapshot = snapshotRef.get();
				cat_log_debug(infoCount+" >> working currentSnapshot="+currentSnapshot+", snapshot="+snapshot+", transaction.root="+transaction.root);
				MooEditableNode editableRootNode = transaction.root.ensureEditable();
				
				
				for(int count = editableRootNode.count()-1; count>=0; count--) {
					MooEditableNode editableChild = editableRootNode.getEditableChild(count);
					MooModuleContent moduleContent = editableChild.getContent(MooModuleContent.KEY);
					if (moduleContent != null) {

						CatReadableTreeNode<VipNode> old_module_content = moduleContent.getBaseNode();
						CatReadableTreeNode<VipNode> new_module_content = snapshot.refresh(old_module_content);

						MooModuleContent newModuleContent = moduleContent.setBaseNode(new_module_content);
						if (newModuleContent!=moduleContent && new_module_content.cat_tree_node_get_location()!=-1) {
							editableChild.setContent(newModuleContent);
							nodesWhichNeedRefresh.add(editableChild.getOriginal());
						}
					}
				}
				
				
				if (transaction.commitEditable(editableRootNode)!=TxResult.FAILED) {
					dispatchRefreshRequests(nodesWhichNeedRefresh);
					break;
				} else {
					cat_log_debug(infoCount+" >> failed set");
					try {
						Thread.sleep(100+retryCnt*200);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			} else {
				break;
			}
		}
		cat_log_debug(infoCount+" >> finished");
		return;

	}



	private void dispatchRefreshRequests(HashSet<MooINode> nodesWhichNeedRefresh) {
		if (nodesWhichNeedRefresh.isEmpty()) {
			return;
		}
		WormService wormService = monkeyService.getWormService();
		for(MooINode node : nodesWhichNeedRefresh) {
			MooModuleRefreshViperRequest refreshViperRequest = new MooModuleRefreshViperRequest(monkeyService, node.getUniqueId());
			wormService.postRequest(refreshViperRequest);
			
		}
		
	}
	
}
