package net.natpad.monkey.model;

import java.io.InputStream;

import net.natpad.caterpillar.versionedtree.CatReadableTreeNode;
import net.natpad.sht.ShoParser;
import net.natpad.sht.model.Model;
import net.natpad.viper.model.VipIFile;
import net.natpad.viper.model.VipNode;

public class MooModuleContent implements MooIContent {

	public static final String SETTINGS_FNAME = "natpad-project.sht";
	public final static String KEY = "MooModuleContent";
	
	private volatile boolean isFixed = false;
	
	private CatReadableTreeNode<VipNode> base_node;
	
	
	private boolean refreshViperInfo = true;
	private int settingsFileIndex;
	private VipNode settingsVipNode;
	private Model model;
	
	
	public MooModuleContent(CatReadableTreeNode<VipNode> base_node) {
		this.base_node = base_node;
		this.settingsFileIndex = -1;
		if (base_node==null) {
			new Exception().printStackTrace();
		}
	}
	
	private MooModuleContent ensureEditable() {
		if (isFixed) {
			MooModuleContent copy = new MooModuleContent(base_node);
			copy.refreshViperInfo = refreshViperInfo;
			copy.settingsFileIndex = settingsFileIndex;
			copy.settingsVipNode = settingsVipNode;
			copy.model = model;
			return copy;
		}
		return this;
	}
	
	public void markFixed() {
		isFixed = true;
	}
	
	@Override
	public String getKey() {
		return KEY;
	}

	
	public boolean refreshViperInfo() {
		return refreshViperInfo;
	}

	
	public MooModuleContent setRefreshViperInfo(boolean refreshViperInfo) {
		if (refreshViperInfo == this.refreshViperInfo) {
			return this;
		}
		MooModuleContent result = ensureEditable();
		result.refreshViperInfo = refreshViperInfo;
		return result;
	}

	public CatReadableTreeNode<VipNode> getBaseNode() {
		return base_node;
	}

	public MooModuleContent setBaseNode(CatReadableTreeNode<VipNode> base_node) {
		if (base_node == this.base_node) {
			return this;
		}
		if (base_node==null) {
			new Exception().printStackTrace();
		}
		MooModuleContent result = ensureEditable();
		result.base_node = base_node;
		result.refreshViperInfo = true;
		return result;
	}

	
	@Override
	public String toString() {
		return "MooModuleContent[base_node="+base_node+", refreshViperInfo="+refreshViperInfo+"]";
	}

	public MooModuleContent setSettingsInfo(int settingsFileIndex, VipNode settingsVipNode) {
		if ((this.settingsFileIndex == settingsFileIndex) && (settingsVipNode==this.settingsVipNode)) {
			return this;
		}
		MooModuleContent result = ensureEditable();
		result.settingsFileIndex = settingsFileIndex;
		if (settingsVipNode != this.settingsVipNode) {
			result.settingsVipNode = settingsVipNode;
			result.model = loadSettings(settingsVipNode);
		}
		return result;
	}

	
	private Model loadSettings(VipNode settingsVipNode) {
		Model result = null;
		if (settingsVipNode!=null) {
			VipIFile content = (VipIFile) settingsVipNode.getResource();
			if (content!=null) {
				try {
					InputStream inputStream = content.openInputStream();
					ShoParser shoParser = new ShoParser();
					result = shoParser.parse(inputStream);
//					cat_log_debug(""+parse.toString());
					result.dump(System.out);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		return result;
	}

	
	public Model getModel() {
		return model;
	}
	
//	
//	private VipNode settingsVipNode;
//	private MooResourceContent settingsResource;
//	private int settingsResourceChildIdx;
//	private AtomicReference<ModuleContext> context;
//
//
//	@Override
//	public void cloneValues(MooNode source) {
//		super.cloneValues(source);
//		this.settingsResource = ((MooModuleContent) source).settingsResource;
//		this.settingsResourceChildIdx = ((MooModuleContent) source).settingsResourceChildIdx;
//		this.settingsVipNode = ((MooModuleContent) source).settingsVipNode;
//		context = new AtomicReference<>(((MooModuleContent) source).getModuleContext());
//	}
//
//	private boolean isSettingsFile(MooNode node) {
//		if (!SETTINGS_FNAME.equals(node.name)) {
//			return false;
//		}
//		if (node instanceof MooResourceContent) {
//			MooResourceContent rn = (MooResourceContent) node;
//			VipIResource content = rn.getViperNode().getContent();
//			if (content instanceof VipIFile) {
//				
//			} else {
//				return false;
//			}
//		} else {
//			return false;
//		}
//		return true;
//	}
//	
//	public boolean testSettingsNode() {
//		/* test the fast track first */
//		if (settingsResourceChildIdx>=0 && settingsResourceChildIdx<count()) {
//			MooNode childAt = (MooNode) childAt(settingsResourceChildIdx);
//			if (childAt instanceof MooResourceContent) {
//				MooResourceContent rn = (MooResourceContent) childAt;
//				if (rn.getViperNode()==viperNode) {
//					return false;
//				}
//			}
//		}
//
//		MooResourceContent foundSettingsFile = null;
//		int foundIdx = -1;
//		for(int idx=0; idx<count(); idx++) {
//			MooNode childAt = (MooNode) childAt(idx);
//			cat_log_debug("childAt="+childAt.name);
//			if (isSettingsFile(childAt)) {
//				foundSettingsFile = (MooResourceContent) childAt;
//				break;
//			}
//		}
//		this.settingsResource = foundSettingsFile;
//		this.settingsResourceChildIdx = foundIdx;
//		if (settingsResource!=null) {
//			settingsVipNode = foundSettingsFile.getViperNode();
//		} else {
//			settingsVipNode = null;
//		}
//		System.err.println("settingsResource="+settingsResource+", settingsVipNode="+settingsVipNode);
//		return true;
//	}
//	
//	
//	@Override
//	public MooEditableNode ensureEditable() {
//		return new MooEditableModule(this, false);
//	}
//	
//	
//	public VipNode getSettingsVipNode() {
//		return settingsVipNode;
//	}
//	
//	public ModuleContext getModuleContext() {
//		return context.get();
//	}
//
//	boolean setModuleContext(ModuleContext newContext) {
//		ModuleContext oldContext = context.getAndSet(newContext);
//		return !(oldContext.equals(newContext));
//	}
//	
//	
//
//	@Override
//	public MooNode finalizeResult()  {
//		MooModuleContent oldModule = (MooModuleContent) getOriginal();
//		MooModuleContent newModule = (MooModuleContent) super.finalizeResult();
//		cat_log_debug("FINRES!!!!!!!!!!!="+oldModule+", newModule="+newModule);
//		if (oldModule!=newModule) {
//			if (newModule.testSettingsNode()) {
//				VipNode settingsVipNode = newModule.getSettingsVipNode();
//				if (settingsVipNode!=null) {
//					VipIFile content = (VipIFile) settingsVipNode.getContent();
//					if (content!=null) {
//						try {
//							InputStream inputStream = content.openInputStream();
//							ShoParser shoParser = new ShoParser();
//							Model parse = shoParser.parse(inputStream);
//							ModuleContext newModuleContext = newModule.getModuleContext().setModel(parse);
//							newModule.setModuleContext(newModuleContext);
//							cat_log_debug(""+parse.toString());
//							
//							parse.dump(System.out);
//						} catch (Exception e) {
//							e.printStackTrace();
//						}
//					}
//				}
//			}
//		}
//		return newModule;
//	}
//	
}
