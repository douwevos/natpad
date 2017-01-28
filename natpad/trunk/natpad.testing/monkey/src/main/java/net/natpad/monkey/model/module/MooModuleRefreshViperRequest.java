package net.natpad.monkey.model.module;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;
import static net.natpad.caterpillar.CatLogger.cat_log_trace;

import java.util.ArrayList;

import net.natpad.caterpillar.CatLogLevel;
import net.natpad.caterpillar.CatLogLevels;
import net.natpad.caterpillar.versionedtree.CatReadableTreeNode;
import net.natpad.monkey.MonkeyService;
import net.natpad.monkey.model.MooEditableNode;
import net.natpad.monkey.model.MooINode;
import net.natpad.monkey.model.MooModuleContent;
import net.natpad.monkey.model.MooResourceByNameMatcher;
import net.natpad.monkey.model.MooResourceContent;
import net.natpad.monkey.model.access.MooIdNodeMatcher;
import net.natpad.monkey.transaction.Transaction;
import net.natpad.monkey.transaction.TxResult;
import net.natpad.monkey.viper.MooINodeWork;
import net.natpad.monkey.viper.MooModuleWork;
import net.natpad.viper.model.VipNode;
import net.natpad.worm.Request;

@CatLogLevel(CatLogLevels.CAT_LOG_DEBUG)
public class MooModuleRefreshViperRequest extends Request {

	
	MonkeyService monkeyService;
	long moduleId;
	
	public MooModuleRefreshViperRequest(MonkeyService monkeyService, long moduleId) {
		this.monkeyService = monkeyService;
		this.moduleId = moduleId;
	}
	
	@Override
	public void runRequest() {
		Transaction transaction = monkeyService.createTransaction();
		int  retryCnt = 0;
		while(retryCnt++<10) {	// TODO: report error when we didn't succeed after 10 retries
			transaction.begin();
			int moduleNodeIndex = transaction.root.findIndex(new MooIdNodeMatcher(moduleId), -1);
			if (moduleNodeIndex<0) {
				cat_log_debug("module was removed:moduleId=%d", moduleId);
				/* the module does not exist anymore ... so were done */
				break;
			}
			
			
			MooINode moduleNode = transaction.root.childAt(moduleNodeIndex);
			MooModuleContent moduleContent = moduleNode.getContent(MooModuleContent.KEY);
			cat_log_debug("moduleContent=%s", moduleContent);
			if (moduleContent==null) {
				/* this module is erronous ... were done */
				break;
			}
			if (!moduleContent.refreshViperInfo()) {
				/* apparently it was already refreshed */
				break;
			}
			
			
			MooEditableNode editableRootNode = transaction.root.ensureEditable();
			MooEditableNode editableModule = editableRootNode.getEditableChild(moduleNodeIndex);
			
			CatReadableTreeNode<VipNode> base_node = moduleContent.getBaseNode();
			cat_log_debug("base_node=%s", base_node);
			MooModuleWork moduleWork = new MooModuleWork(monkeyService, base_node, editableModule);
			ArrayList<MooINodeWork> workList = new ArrayList<MooINodeWork>();
			workList.add(moduleWork);
			while(!workList.isEmpty()) {
				MooINodeWork work = workList.remove(workList.size()-1);
				work.doWork(workList);
				cat_log_trace(" doing work :"+work);
			}
			
			
			moduleContent = refreshModuleSettings(moduleContent, editableModule);
			
			moduleContent = moduleContent.setRefreshViperInfo(false);
			editableModule.setContent(moduleContent);
			
			if (transaction.commitEditable(editableRootNode)!=TxResult.FAILED) {
				break;
			} else {
				cat_log_debug("failed commit");
				try {
					Thread.sleep(100+retryCnt*200);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			
		}
		
	}

	private MooModuleContent refreshModuleSettings(MooModuleContent moduleContent, MooEditableNode editableModule) {
		MooResourceByNameMatcher nameMatcher = new MooResourceByNameMatcher(MooModuleContent.SETTINGS_FNAME);
		int settingsFileIndex = editableModule.findIndex(nameMatcher, -1);
		VipNode settingsVipNode = null;
		if (settingsFileIndex>=0) {
			MooINode childAt = editableModule.childAt(settingsFileIndex);
			MooResourceContent settingsResContent = childAt.getContent(MooResourceContent.KEY);
			settingsVipNode = settingsResContent.getViperNode();
		}
		
		moduleContent = moduleContent.setSettingsInfo(settingsFileIndex, settingsVipNode);
		return moduleContent;
	}
	
}
