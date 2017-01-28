package net.natpad.monkey.ui;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.Shape;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import net.natpad.monkey.model.MooNode;

public class LayoutModel {

	MooNode node;
	boolean layoutIsValid = false;
	List<NodeLayout> nodeLayouts = new ArrayList<NodeLayout>();
	
	NodeLayout root = new NodeLayout(null, -1);
	
	Dimension size = new Dimension();
	
	NodeLayout cursor = null;
	NodeLayout shiftCursor = null;
	
	private final NodeRenderRegistry renderRegistry;
	
	public LayoutModel(NodeRenderRegistry renderRegistry) {
		this.renderRegistry = renderRegistry;
		root.isExpanded = true;
	}
	
	



	public void setRootNode(MooNode node) {
//		if (SwingUtilities.isEventDispatchThread()) {
//			System.err.println("WARNING setRootNode should be called from the Swing-thread");
//		}
		if (this.node == node) {
			return;
		}
		cat_log_debug("node set="+node);
		layoutIsValid = false;
		this.node = node;
	}

	

	public void validate(Graphics gfx) {
		if (layoutIsValid || gfx==null) {
			return;
		}
		layoutIsValid = true;
		/* create map of current NodeLayout Elements */
//		HashMap<Long, NodeLayout> mapOld = new HashMap<Long, NodeLayout>();
//		for(NodeLayout nodeLayout : nodeLayouts) {
//			mapOld.put(new Long(nodeLayout.node.uniqueId), nodeLayout);
//		}
		nodeLayouts.clear();
		NodeLayout oldCursor = cursor;
		cursor = null;
		if (node==null) {
			size.width = 0;
			size.height = 0;
			return;
		}

		
		FontMetrics fontMetrics = gfx.getFontMetrics();
		int fontHeight = fontMetrics.getHeight();
		fontHeight = fontHeight+1;
		fontHeight = fontHeight & 0xffffe;

		int ypos = 0;
		int maxWidth = 1;
		
		
		
		root.setNode(node);
		
		
		ArrayList<NodeLayout> stack = new ArrayList<NodeLayout>();
		stack.add(root);
		
		nodeLayouts.clear();

		
		while(!stack.isEmpty()) {
			NodeLayout workNodeLayout = stack.remove(stack.size()-1);
			if (workNodeLayout.treeLevel>=0) {
				workNodeLayout.refreshRendererList(renderRegistry);
				
				workNodeLayout.updateLayout(gfx, ypos, fontHeight);
				ypos += fontHeight;
				int right = workNodeLayout.getX()+workNodeLayout.getWidth();
				
				
				if (right>maxWidth) {
					maxWidth = right;
				}
				nodeLayouts.add(workNodeLayout);
			}
			
			
			MooNode workNode = workNodeLayout.node;
			ArrayList<NodeLayout> newLayoutChildren = null;
			if (workNodeLayout.isExpanded && workNode!=null && workNode.count()>0) {
				MooNode[] children = workNode.getChildren();
				Arrays.sort(children, new NodeComparator());
				
				if (children!=null && children.length>0) {
					ArrayList<NodeLayout> layoutChildren = workNodeLayout.getChildren();
					int childTreeLevel = workNodeLayout.treeLevel+1;
					newLayoutChildren = new ArrayList<NodeLayout>();
					for(MooNode child : children) {
						NodeLayout childNodeLayout = findLayoutByMooseId(layoutChildren, child);
						if (childNodeLayout == null) {
							childNodeLayout = new NodeLayout(child, childTreeLevel);
						} else {
							childNodeLayout.setNode(child);
							if (childNodeLayout==oldCursor) {
								cursor = childNodeLayout;
								oldCursor = null;
							}
						}
						newLayoutChildren.add(childNodeLayout);
						stack.add(childNodeLayout);
					}
				}
			}
			workNodeLayout.setChildren(newLayoutChildren);
		}
		
		
//		root.enlistRecursive(nodeLayouts);
		
//		
//		
//		ArrayList<MooINode> enlistedChildren = node.enlistChildren(null);
//		for (MooINode mooINode : enlistedChildren) {
//			NodeLayoutWork nodeLayoutWork = new NodeLayoutWork(mooINode, treeLevel, null);
//			stack.add(nodeLayoutWork);
//		}
//		
////		stack.add(node);
//		int maxWidth = 0;
//		while(!stack.isEmpty()) {
//			NodeLayoutWork workNode = stack.remove(stack.size()-1);
//			if (workNode==null) {
//				treeLevel--;
//				continue;
//			}
//			
//			NodeLayout nodeLayout = mapOld.get(new Long(workNode.uniqueId));
//			if (nodeLayout==null) {
//				nodeLayout = new NodeLayout(workNode., workNode);
//			} else {
//				nodeLayout.setNode(workNode);
//				if (nodeLayout==oldCursor) {
//					cursor = nodeLayout;
//					oldCursor = null;
//				}
//			}
//			
//			nodeLayouts.add(nodeLayout);
//
//			
//			nodeLayout.refreshRendererList(renderRegistry);
//			
//			nodeLayout.updateLayout(gfx, treeLevel, ypos, fontHeight);
//			ypos += fontHeight;
//			int right = nodeLayout.getX()+nodeLayout.getWidth();
//			
//			
//			if (right>maxWidth) {
//				maxWidth = right;
//			}
//			
//			if (nodeLayout.isExpanded && workNode.count()>0) {
//				MooNode[] children = workNode.getChildren();
//				Arrays.sort(children, new NodeComparator());
//				
//				if (children!=null && children.length>0) {
//					stack.add(null);
//					treeLevel++;
//					stack.addAll(Arrays.asList(children));
//				}
//			}
//		}
		size.width = maxWidth+1;
		size.height = ypos+1;
	}



	private NodeLayout findLayoutByMooseId(ArrayList<NodeLayout> layoutChildren, MooNode child) {
		if (layoutChildren!=null) {
			for (NodeLayout nodeLayout : layoutChildren) {
				if (nodeLayout.node.uniqueId==child.uniqueId) {
					return nodeLayout;
				}
			}
		}
		return null;
	}





	public void paint(Graphics g) {
		cat_log_debug("painting layout-model:nodeLayouts.size="+nodeLayouts.size());
		g.setColor(Color.black);
		
		Shape clip = g.getClip();
		Rectangle clipBounds = clip.getBounds();
		Rectangle testBounds = new Rectangle(clipBounds);
		cat_log_debug("rect-y:"+clipBounds.y+", x="+clipBounds.x+", clipBounds.width="+clipBounds.width);
		
		int bottom = 0;
		for(NodeLayout nodeLayout : nodeLayouts) {
			Rectangle nlbounds = nodeLayout.boundaries;
			bottom = nlbounds.height+nlbounds.y;
			testBounds.y = nlbounds.y;
			testBounds.height = nlbounds.height;
			
			if (clip.intersects(testBounds)) {
				nodeLayout.paint(g, nodeLayout==cursor);
			}
		}
		
		if (clipBounds.y+clipBounds.height>=bottom) {
			g.clearRect(clipBounds.x, bottom, clipBounds.width, clipBounds.y+clipBounds.height-bottom);
		}
	}

	
	
	public NodeLayout nodeAt(int xpos, int ypos) {
		NodeLayout result = null;
		if (nodeLayouts.size()>0) {
			int entryHeight = nodeLayouts.get(0).getHeight();
			int offset = ypos/entryHeight;
			if (offset>=0 && offset<nodeLayouts.size()) {
				result = nodeLayouts.get(offset);
			}
		}
		return result;
	}


	public void setSelection(int initPos, int endPos) {
		if (endPos<initPos) {
			initPos = initPos - endPos;
			endPos = endPos + initPos;
			initPos = endPos - initPos;
		}

		for (int idx = 0; idx < nodeLayouts.size(); idx++) {
			NodeLayout nodeLayout = nodeLayouts.get(idx);
			boolean isInSelection = (idx >= initPos && idx <= endPos);
			if (nodeLayout.isSelected != isInSelection) {
				nodeLayout.isSelected = !nodeLayout.isSelected;
				nodeLayout.needsRepaint = true;
			}
		}
	}
	

	public boolean select(NodeLayout nodeAt, boolean tryToggleFirst, boolean ctrlPressed, boolean shiftPressed) {
		if (tryToggleFirst) {
			if (nodeAt.is_a_map()) {
				nodeAt.isExpanded = !nodeAt.isExpanded;
				nodeAt.needsRepaint = true;
				layoutIsValid = false;
				return true;
			}
		}
		if (cursor!=nodeAt) {
			if (cursor!=null) {
				cursor.needsRepaint = true;
			}
			cursor = nodeAt;
			if (cursor!=null) {
				cursor.needsRepaint = true;
			}
			if (!shiftPressed) {
				shiftCursor = cursor;
			}
		}
			
		cat_log_debug("ctrlPressed="+ctrlPressed+", shiftPressed="+shiftPressed);
		if (ctrlPressed) {
			nodeAt.isSelected = !nodeAt.isSelected;
		} else if (shiftPressed) {
			if (shiftCursor!=null) {
				int initPos = shiftCursor.getY()/shiftCursor.getHeight();
				int endPos = cursor.getY()/cursor.getHeight();
				setSelection(initPos, endPos);
			}
		} else if (cursor!=null) {
			int endPos = cursor.getY()/cursor.getHeight();
			setSelection(endPos, endPos);
		}
		return true;
	}
	
}
