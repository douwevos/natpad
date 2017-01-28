package net.natpad.jaguar.monkey;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;
import net.natpad.jaguar.monkey.JagRequestUpdateSrcFolder.JagPackageMatcher;
import net.natpad.monkey.model.MooEditableContentMap;
import net.natpad.monkey.model.MooEditableNode;
import net.natpad.monkey.model.MooINode;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.MooResourceContent;
import net.natpad.monkey.model.access.MooINodeMatcher;
import net.natpad.monkey.model.access.MooIdPath;
import net.natpad.viper.VipISequence;
import net.natpad.viper.model.VipIFile;
import net.natpad.viper.model.VipIMap;
import net.natpad.worm.Request;

public class JagRequestUpdatePackage extends Request {

	private final VipISequence mooIdSequence;
	private final MooEditableNode editableSrcFolder;
	private final MooNode resNode;
	private final MooEditableNode editablePackage;
	private final MooIdPath packagePath;
	
	public JagRequestUpdatePackage(VipISequence mooIdSequence, MooEditableNode editableSrcFolder, MooNode resNode, MooEditableNode editablePackage, MooIdPath packagePath) {
		this.mooIdSequence = mooIdSequence;
		this.editableSrcFolder = editableSrcFolder;
		this.resNode = resNode;
		this.editablePackage = editablePackage;
		this.packagePath = packagePath;
	}
	
	@Override
	public void runRequest() {
		/* scan for new packages and update existing */
		for(int childIdx = resNode.count()-1; childIdx>=0; childIdx--) {
			MooINode child = resNode.childAt(childIdx);
			if (child.getName().startsWith(".")) {
				continue;
			}
			MooNode ochild = (MooNode) child.getOriginal();
			MooResourceContent childResContent = child.getContent(MooResourceContent.KEY);
			cat_log_debug("child["+childIdx+" = "+ochild.getClass().getName());
			if (childResContent!=null) {
				Object childVipContent = childResContent.getViperNode().getResource();
				if (childVipContent instanceof VipIMap) {
					String packageName = editablePackage.getName();
//					String packageName = resNode.name;
					if (packageName.length()>0) {
						packageName += ".";
					}
					packageName += child.getName();
					int foundIndex = editableSrcFolder.findIndex(new JagPackageMatcher(packageName), -1);
					cat_log_debug("child["+childIdx+"] is a map : packageName="+packageName+", foundIndex="+foundIndex);
					JagPackageContent jagPackageContent = null;
					MooEditableNode editablePackage = null;
					if (foundIndex>=0) {
						editablePackage = editableSrcFolder.getEditableChild(foundIndex);
					} else {
						MooEditableContentMap econtentMap = new MooEditableContentMap(null);
						jagPackageContent = new JagPackageContent();
						econtentMap.set(jagPackageContent);
						MooNode packageNode = new MooNode(mooIdSequence.next(), packageName, null, econtentMap.finalizeResult(), 10);
						editablePackage = editableSrcFolder.addNewChild(packageNode);
					}
					
					MooIdPath subPkgPath = packagePath.addId(editablePackage.getUniqueId(), childIdx);
					JagRequestUpdatePackage requestUpdatePackage = new JagRequestUpdatePackage(mooIdSequence, editableSrcFolder, ochild, editablePackage, subPkgPath);
					requestUpdatePackage.runRequest();
					
					
				} else if (childVipContent instanceof VipIFile) {
					cat_log_debug("child["+childIdx+" is a map ");
					String packageName = child.getName();
					int foundIndex = editablePackage.findIndex(new JagClassfileMatcher(packageName), -1);
					JagClassfileContent jagNodeclassfile = null;
					MooEditableNode editablePackageNode = null;
					if (foundIndex>=0) {
						editablePackageNode = editablePackage.getEditableChild(foundIndex);
						// TODO the uniqueId might have changed
					} else {
						MooIdPath classfilePath = packagePath.addId(child.getUniqueId(), childIdx);
						MooEditableContentMap econtentMap = new MooEditableContentMap(null);
						jagNodeclassfile = new JagClassfileContent(classfilePath);
						econtentMap.set(jagNodeclassfile);
						MooNode classNode = new MooNode(mooIdSequence.next(), packageName, null, econtentMap.finalizeResult(), 10);
						
						editablePackageNode = editablePackage.addNewChild(classNode);
					}

					
//					MonIdPath subPkgPath = packagePath.addId(editablePackage.getUniqueId(), childIdx);

					
//					new JagRequestUpdatePackage(monkeyService, resNode, editablePackage);
				}
			}
		}

	}

	
	

	static class JagClassfileMatcher implements MooINodeMatcher {

		private final String classfileName;
		
		public JagClassfileMatcher(String classfileName) {
			this.classfileName = classfileName;
		}
		
		@Override
		public boolean matches(MooINode node) {
			if (node.getContent(JagClassfileContent.KEY)!=null) {
				if (classfileName.equals(node.getName())) {
					return true;
				}
			}
			return false;
		}
		
	}

	
}
