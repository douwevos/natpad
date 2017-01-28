package net.natpad.jaguar.monkey.request;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import java.util.HashSet;

import net.natpad.caterpillar.CatArray;
import net.natpad.caterpillar.CatFixedArray;
import net.natpad.caterpillar.CatFixedString;
import net.natpad.caterpillar.CatLogLevel;
import net.natpad.caterpillar.CatLogLevels;
import net.natpad.jaguar.monkey.JagMooseContext;
import net.natpad.jaguar.monkey.JagNodeCompiler;
import net.natpad.jaguar.monkey.JagSrcFolderContent;
import net.natpad.jaguar.monkey.JagRequestUpdateSrcFolder;
import net.natpad.jaguar.monkey.context.JagModuleContent;
import net.natpad.jaguar.monkey.context.JagModuleSettings;
import net.natpad.jaguar.monkey.request.JagRequestApplyTransactionChanges.JagCompilerMatcher;
import net.natpad.jaguar.monkey.request.JagRequestApplyTransactionChanges.JagSrcFolderMatcher;
import net.natpad.monkey.model.MooEditableContentMap;
import net.natpad.monkey.model.MooEditableNode;
import net.natpad.monkey.model.MooINode;
import net.natpad.monkey.model.MooModuleContent;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.MooResourceByNameMatcher;
import net.natpad.monkey.model.access.MooIdNodeMatcher;
import net.natpad.monkey.model.access.MooIdPath;
import net.natpad.monkey.transaction.Transaction;
import net.natpad.monkey.transaction.TxResult;
import net.natpad.worm.Request;

@CatLogLevel(CatLogLevels.CAT_LOG_DEBUG)
public class JagRequestRefreshModule extends Request {

	JagMooseContext mooseContext;
	long uniqueId;
	int last_known_node_idx;
	
	public JagRequestRefreshModule(JagMooseContext mooseContext, long uniqueId, int last_known_node_idx) {
		this.mooseContext = mooseContext;
		this.uniqueId = uniqueId;
		this.last_known_node_idx = last_known_node_idx;
		
		setTimeOut(1000l);
	}
	
	
	@Override
	public void runRequest() {
		
		while(true) {
			Transaction tx = mooseContext.monkeyService.createTransaction();
			tx.begin();
			MooNode rootNode = tx.root;
			MooIdNodeMatcher mooIdMatcher = new MooIdNodeMatcher(uniqueId);
			int nodeIndex = rootNode.findIndex(mooIdMatcher, last_known_node_idx);
			cat_log_debug(tx.txInfo+" : nodeIndex="+nodeIndex+", rootNode="+rootNode);
	
			if (nodeIndex>=0) {
				MooINode child = rootNode.childAt(nodeIndex);
				
				MooEditableNode refreshModule = refreshModule(tx, child, nodeIndex);
				cat_log_debug(tx.txInfo+" : refreshModule="+refreshModule);
				TxResult txResult = tx.commitEditable(refreshModule);
				if (txResult!=TxResult.FAILED) {
					cat_log_debug(tx.txInfo+" : done");
					return;
				} else {
					cat_log_debug("failed commit");
				}
			} else {
				cat_log_debug(tx.txInfo+" : uniqueId="+uniqueId+" not found");
			}
		}
	}


	
	private MooEditableNode refreshModule(Transaction transaction, MooINode moduleNode, int nodeIndex) {
		cat_log_debug(transaction.txInfo+"");


		boolean shouldRefresh = true;

		cat_log_debug("shouldRefresh="+shouldRefresh);

		
		MooIdPath mainPath = new MooIdPath(new long[0]);
//		mainPath.addId(moduleNode.getUniqueId(), nodeIndex);
		
		if (shouldRefresh) {
			MooEditableNode editableRoot = transaction.root.ensureEditable();
			MooEditableNode editableChild = editableRoot.getEditableChild(nodeIndex);
			
			
			MooIdPath modulePath = mainPath.addId(moduleNode.getUniqueId(), nodeIndex);

			MooModuleContent moduleContent = moduleNode.getContent(MooModuleContent.KEY);
			
			// TODO test moduleContent if java nature is enabled
			
			JagModuleContent jagContent = moduleNode.getContent(JagModuleContent.KEY);
			
			if (jagContent==null) {
				jagContent = new JagModuleContent();
				editableChild.setContent(jagContent);
			}
			
			JagModuleContent newJagContext = jagContent.reconfigure(moduleContent.getModel());
			
			
			JagModuleSettings moduleSettings = newJagContext.getModuleSettings();
			CatFixedArray<CatArray<CatFixedString>> sourceFolders = moduleSettings.getSourceFolders();
			

			HashSet<Integer> indexesFresh = new HashSet<Integer>();
			
			cat_log_debug("sourceFolders="+sourceFolders.size());
			
			
			for(CatArray<CatFixedString> sourceFolder : sourceFolders) {
				String entries[] = new String[sourceFolder.size()];
				StringBuilder nodename = new StringBuilder();
				int eidx = 0;
				for(CatFixedString e : sourceFolder) {
					entries[eidx] = e.toString();
					if (eidx>0) {
						nodename.append('/');
					}
					nodename.append(e.toString());
					eidx++;
				}
				String nodeName = nodename.toString();
				cat_log_debug("nodeName="+nodeName);
			
				
				/* source folders */
				long srcMainJavaPathIds[] = new long[entries.length];
				int[] srcMainJavaPath = findNodePath(moduleNode, entries, srcMainJavaPathIds);
				int foundIndex = moduleNode.findIndex(new JagSrcFolderMatcher(nodeName), -1);
				cat_log_debug("srcMainJavaPath="+srcMainJavaPath+", foundIndex="+foundIndex);
				if (srcMainJavaPath!=null) {
					MooIdPath srcFolderPath = modulePath.addMultipleIds(srcMainJavaPathIds);
					MooINode srcFolderNode = null;
					JagSrcFolderContent srcFolderContent = null;
					MooEditableNode editableSrcFolder = null;
					if (foundIndex>=0) {
						srcFolderNode = moduleNode.childAt(foundIndex);
						srcFolderContent = srcFolderNode.getContent(JagSrcFolderContent.KEY);
						editableSrcFolder = editableChild.getEditableChild(foundIndex);
						indexesFresh.add(foundIndex);
					} else {
						indexesFresh.add(editableChild.count());
						MooEditableContentMap econtentMap = new MooEditableContentMap(null);
						srcFolderContent = new JagSrcFolderContent();
						econtentMap.set(srcFolderContent);
						srcFolderNode = new MooNode(transaction.uniqueIdCounter.incrementAndGet(), nodeName, null, econtentMap.finalizeResult(), 200);
						editableSrcFolder = editableChild.addNewChild(srcFolderNode);
						cat_log_debug("added new child");
					}
					
					
					JagRequestUpdateSrcFolder srcFolderReq = new JagRequestUpdateSrcFolder(mooseContext.monkeyService, editableRoot, editableSrcFolder, srcFolderPath);
					srcFolderReq.runRequest();
					
					
				} else {
					if (foundIndex>=0) {
						MooEditableNode toDelete = editableChild.getEditableChild(foundIndex);
						toDelete.markedForDeletion();
					}
				}
				
			}
			
			
			
			/* remove old source folders */
			for(int idx=0; idx<editableChild.count(); idx++) {
				
				MooINode childAt = editableChild.childAt(idx);
				JagSrcFolderContent srcFolderContent = childAt.getContent(JagSrcFolderContent.KEY);
				if (srcFolderContent != null) {
					if (!indexesFresh.contains(idx)) {
						editableChild.getEditableChild(idx).markedForDeletion();
					}
				}
			}
			
			
			
			/* compiler */
			int foundIndex = editableChild.findIndex(new JagCompilerMatcher(), -1);
			MooNode nodeCompiler = null;
			MooEditableNode editableCompiler = null;
			if (foundIndex>=0) {
//				nodeCompiler = (JagNodeCompiler) module.childAt(foundIndex);
//				editableCompiler = editableChild.getEditableChild(foundIndex);
			} else {
				nodeCompiler = mooseContext.getCompilerNode("java");
				if (nodeCompiler!=null) {
////					nodeCompiler = new JagNodeCompiler(transaction.uniqueIdCounter.incrementAndGet());
					editableCompiler = editableChild.addNewChild(nodeCompiler);
//					cat_log_debug("added new child");
				} else {
					// TODO remove compiler from module
				}
			}
			
			return editableRoot;
			
		}
		return null;
	}

	
//	int findIndex = editableRootNode.findIndex(new JagCompilerMatcher());
//	if (findIndex<0) {
//		editableRootNode.addNewChild(compilerMap.getCompilerNode("java"));
//	}
				
	
	
	

	public int[] findNodePath(MooINode node, String path[], long uniqueIds[]) {
		int result[] = new int[path.length];
		
		int out = 0;
		for(String entry : path) {
			int findIndex = node.findIndex(new MooResourceByNameMatcher(entry), -1);
			if (findIndex>=0) {
				result[out++] = findIndex;
				node = node.childAt(findIndex);
				uniqueIds[out-1] = node.getUniqueId();
			} else {
				result = null;
				break;
			}
		}
		return result;
	}
	

}
