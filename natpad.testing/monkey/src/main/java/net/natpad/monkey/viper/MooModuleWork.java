package net.natpad.monkey.viper;

import java.util.HashSet;
import java.util.List;

import net.natpad.caterpillar.versionedtree.CatReadableTreeNode;
import net.natpad.monkey.model.MooEditableContentMap;
import net.natpad.monkey.model.MooEditableNode;
import net.natpad.monkey.model.MooINode;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.MooResourceContent;
import net.natpad.viper.VipISequence;
import net.natpad.viper.model.VipIFile;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.access.VipNodeIdMatcher;

public class MooModuleWork implements MooINodeWork {

		private final VipISequence mooseSequence;
		private CatReadableTreeNode<VipNode> new_node;
		private MooEditableNode editableNode;
		
		boolean forceUpdate = true;
		
		public MooModuleWork(VipISequence mooseSequence, CatReadableTreeNode<VipNode> new_node, MooEditableNode editableNode) {
			this.mooseSequence = mooseSequence;
			this.new_node = new_node;
			this.editableNode = editableNode;
			
			MooResourceContent resourceContent = editableNode.getContent(MooResourceContent.KEY);
			resourceContent.setNode(new_node);
			
		}
		
		@Override
		public void doWork(List<MooINodeWork> workList) {
			HashSet<Long> vipNodesAdded = new HashSet<Long>();
			
			if (editableNode.count()>0) {
				for(int childIdx=0; childIdx<editableNode.count(); childIdx++) {
					MooINode child = editableNode.childAt(childIdx);
					MooResourceContent resourceContent = child.getContent(MooResourceContent.KEY);
					if (resourceContent!=null) {
						CatReadableTreeNode<VipNode> old_child_node = resourceContent.get_node();

						int foundIndex = new_node.cat_tree_node_find_child_index(new VipNodeIdMatcher(old_child_node.cat_tree_node_get_content().getViperId()), -1);
						if (foundIndex>=0) {
							CatReadableTreeNode<VipNode> new_child_node = new_node.cat_tree_node_child_at(foundIndex);
							if (new_child_node != old_child_node || forceUpdate) {
								MooEditableNode editableChild = editableNode.getEditableChild(childIdx);
								MooModuleWork moduleWork = new MooModuleWork(mooseSequence, new_child_node, editableChild);
//								work.forceUpdate = forceUpdate;
								workList.add(moduleWork);
							}
							vipNodesAdded.add(new_child_node.cat_tree_node_get_content().getViperId());
							
						} else {
							MooEditableNode editableChild = editableNode.getEditableChild(childIdx);
							editableChild.markedForDeletion();
						}
						
					}
				}
			}
			
			int newChildCount = new_node.cat_tree_node_child_count();
			// add new entries
			for(int childIdx=0; childIdx<newChildCount; childIdx++) {
				CatReadableTreeNode<VipNode> new_child_node = new_node.cat_tree_node_child_at(childIdx);
				VipNode newChild = new_child_node.cat_tree_node_get_content();
				if (!vipNodesAdded.contains(newChild.viperId)) {
					int z = 100;
					if (newChild.getResource() instanceof VipIFile) {
						z = 50;
					}
					MooEditableContentMap contentMap = new MooEditableContentMap(null);
					contentMap.set(new MooResourceContent(new_child_node));
					MooNode subNode = new MooNode(mooseSequence.next(), newChild.getName(), null, contentMap.finalizeResult(), z);
					MooEditableNode editableChild = editableNode.addNewChild(subNode);
					MooModuleWork moduleWork = new MooModuleWork(mooseSequence, new_child_node, editableChild);
//					work.forceUpdate = forceUpdate;
					workList.add(moduleWork);
				}
			}
		}
		
		
		
		@Override
		public String toString() {
			return "Work()="+editableNode.count()+", newViperNode="+new_node.cat_tree_node_get_content().getName();
		}

		
	}