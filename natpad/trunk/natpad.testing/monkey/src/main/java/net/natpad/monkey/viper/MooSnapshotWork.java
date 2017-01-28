package net.natpad.monkey.viper;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import java.util.List;

import net.natpad.caterpillar.versionedtree.CatReadableTreeNode;
import net.natpad.caterpillar.CatLogLevel;
import net.natpad.caterpillar.CatLogLevels;
import net.natpad.monkey.model.MooEditableNode;
import net.natpad.monkey.model.MooINode;
import net.natpad.monkey.model.MooModuleContent;
import net.natpad.monkey.model.MooResourceContent;
import net.natpad.viper.VipISequence;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.VipSnapshot;


@CatLogLevel(CatLogLevels.CAT_LOG_TRACE)
class MooSnapshotWork implements MooINodeWork {

	private final VipSnapshot snapshot;
	private final MooEditableNode editableNode;
	private final boolean forceUpdate = false;
	private final VipISequence mooseSequence;

	public MooSnapshotWork(VipISequence mooseSequence, VipSnapshot snapshot, MooEditableNode editableNode) {
		this.snapshot = snapshot;
		this.editableNode = editableNode;
		this.mooseSequence = mooseSequence;
	}

	public void doWork(List<MooINodeWork> workList) {

		if (editableNode.count() > 0) {
			for (int childIdx = 0; childIdx < editableNode.count(); childIdx++) {
				MooINode child = editableNode.childAt(childIdx);
				MooModuleContent moduleContent = child.getContent(MooModuleContent.KEY);
				MooResourceContent resourceContent = child.getContent(MooResourceContent.KEY);
				cat_log_debug("snapshot item[%d] = %o, module-content=%o, resource-content=%o", childIdx, child, moduleContent, resourceContent);
				if ((moduleContent != null) && (resourceContent!=null)) {

					CatReadableTreeNode<VipNode> node = resourceContent.get_node();
					CatReadableTreeNode<VipNode> refreshed_node = snapshot.refresh(node);
					
					if (node != refreshed_node || forceUpdate) {
						MooEditableNode editableChild = editableNode.getEditableChild(childIdx);
						MooModuleWork moduleWork = new MooModuleWork(mooseSequence, refreshed_node, editableChild);
						// work.forceUpdate = forceUpdate;
						workList.add(moduleWork);
					}
				}
			}
		}
	}

	@Override
	public String toString() {
		return "SnapshotWork:editableNode.count()=" + editableNode.count();
	}
}