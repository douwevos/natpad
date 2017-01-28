package net.natpad.jaguar.monkey;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;
import net.natpad.monkey.model.MooEditableContentMap;
import net.natpad.monkey.model.MooEditableNode;
import net.natpad.monkey.model.MooINode;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.MooResourceContent;
import net.natpad.monkey.model.access.MooINodeMatcher;
import net.natpad.monkey.model.access.MooIdPath;
import net.natpad.viper.VipISequence;
import net.natpad.viper.model.VipIMap;
import net.natpad.worm.Request;

public class JagRequestUpdateSrcFolder extends Request {

	
	private final VipISequence mooIdSequence;
	private final MooEditableNode editableRoot;
	private final MooEditableNode editableSrcFolder;
	private final MooIdPath srcFolderPath;
	

	public JagRequestUpdateSrcFolder(VipISequence mooIdSequence, MooEditableNode editableRoot, MooEditableNode editableSrcFolder,
			MooIdPath srcFolderPath) {
		this.mooIdSequence = mooIdSequence;
		this.editableRoot = editableRoot;
		this.editableSrcFolder = editableSrcFolder;
		this.srcFolderPath = srcFolderPath;
	}

	@Override
	public void runRequest() {
		/* first resolve the actual source folder */
		MooEditableNode srcFolderNode = srcFolderPath.createEditableNode(editableRoot);
//		INode srcFolderNode = editableModule;
//		for(int nodeIdx : srcFolderPath) {
//			cat_log_debug(""+srcFolderNode.getName()+" , path-index="+nodeIdx);
//			srcFolderNode = srcFolderNode.childAt(nodeIdx);
//			if (srcFolderNode==null) {
//				System.err.println("ERROR :::: path invalid");
//				// TODO all old packages need to be removed
//				return;
//			}
//		}
		if (srcFolderNode == null) {
			System.err.println("ERROR :::: path invalid");
			// TODO all old packages need to be removed
			return;
		}
		
		cat_log_debug("updating packages: srcFolderNode.count()="+srcFolderNode.count());
		
		/* scan for new packages and update existing */
		for(int childIdx = srcFolderNode.count()-1; childIdx>=0; childIdx--) {
			MooINode child = srcFolderNode.childAt(childIdx);
			if (child.getName().startsWith(".")) {
				continue;
			}

			MooNode ochild = (MooNode) child.getOriginal();
			MooResourceContent childResContent = child.getContent(MooResourceContent.KEY);
			cat_log_debug("child["+childIdx+" = "+ochild.getClass().getName());
			if (childResContent!=null) {
				Object viperNode = childResContent.getViperNode().getResource();
				cat_log_debug("child["+childIdx+"] name = "+child.getName()+", vip-node="+viperNode.getClass().getName());
				if (viperNode instanceof VipIMap) {
					String packageName = child.getName();
					int foundIndex = editableSrcFolder.findIndex(new JagPackageMatcher(packageName), -1);
					cat_log_debug("child["+childIdx+"] is a map : packageName="+packageName+", foundIndex="+foundIndex);
					JagPackageContent jagNodePackage = null;
					MooEditableNode editablePackage = null;
					if (foundIndex>=0) {
						editablePackage = editableSrcFolder.getEditableChild(foundIndex);
					} else {
						MooEditableContentMap econtentMap = new MooEditableContentMap(null);
						jagNodePackage = new JagPackageContent();
						econtentMap.set(jagNodePackage);
						MooNode n = new MooNode(mooIdSequence.next(), packageName, null, econtentMap.finalizeResult(), 200);
						editablePackage = editableSrcFolder.addNewChild(n);
					}

					MooIdPath subPkgPath = srcFolderPath.addId(editablePackage.getUniqueId(), childIdx);
					JagRequestUpdatePackage requestUpdatePackage = new JagRequestUpdatePackage(mooIdSequence, editableSrcFolder, ochild, editablePackage, subPkgPath);
					requestUpdatePackage.runRequest();
					
				} else {
					/* TODO what about classes in the default package */
				}
			}
		}
		
		
		/* remove old packages */
		
	}
	
	
	

	static class JagPackageMatcher implements MooINodeMatcher {

		private final String srcFolderName;
		
		public JagPackageMatcher(String srcFolderName) {
			this.srcFolderName = srcFolderName;
		}
		
		@Override
		public boolean matches(MooINode node) {
			if (node.getContent(JagPackageContent.KEY)!=null) {
				if (srcFolderName.equals(node.getName())) {
					return true;
				}
			}
			return false;
		}
		
	}
	
}
