package net.natpad.monkey.ui;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import java.awt.Component;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.font.TextAttribute;
import java.util.Map;

import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.SwingUtilities;

import net.natpad.caterpillar.CatLogLevel;
import net.natpad.caterpillar.CatLogLevels;
import net.natpad.monkey.MonkeyService;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.transaction.MooITransactionListener;
import net.natpad.monkey.transaction.Transaction;
import net.natpad.monkey.ui.NodeLayout.LayoutZone;
import net.natpad.monkey.ui.action.ActionGroup;

@CatLogLevel(CatLogLevels.CAT_LOG_TRACE)
public class MonkeyExplorer extends JPanel implements MooITransactionListener, MouseListener, MouseMotionListener {

	

	MonkeyService monkeyService;
	LayoutModel layoutModel;
	ActionGroup actionGroup;
	
	private Font fontje;
	
	public MonkeyExplorer(MonkeyService monkeyService, NodeRenderRegistry renderRegistry, ActionGroup actionGroup) {
		this.actionGroup = actionGroup;
//		fontje = new Font("Purisa", Font.PLAIN, 14);
		fontje = new Font("UnDinaru Regular", Font.PLAIN, 18);
		
		Map<TextAttribute, Object> attributes = (Map<TextAttribute, Object>) fontje.getAttributes();
		
//		for (TextAttribute attr : attributes.keySet()) {
//			cat_log_debug("::"+attr+", "+attributes.get(attr));
//		}
		
//		attributes.put(TextAttribute.WEIGHT, TextAttribute.WEIGHT_BOLD);
		
		fontje = new Font(attributes);
		
		this.setFont(fontje);
		this.monkeyService = monkeyService;
		this.layoutModel = new LayoutModel(renderRegistry);
//		layoutModel.setRootNode(monkeyService.getRoot());

		monkeyService.transactionDispatcher.addTransactionListener(this);
		
		MooNode root = monkeyService.getRoot();
		transactionCommited(null, root);
		
		
		addMouseListener(this);
	}
	
	@Override
	public void addNotify() {
		super.addNotify();
		this.setFont(fontje);
		getGraphics().setFont(fontje);
	}

	
	static class LayoutModelSetRootNode implements Runnable {
		
		final LayoutModel layoutModel;
		final MooNode node;
		final Component repaintComponent;
		
		public LayoutModelSetRootNode(LayoutModel layoutModel, MooNode node, Component repaintComponent) {
			this.layoutModel = layoutModel;
			this.node = node;
			this.repaintComponent = repaintComponent;
		}
		
		@Override
		public void run() {
			cat_log_debug("root-node=", node);
			layoutModel.setRootNode(node);
			layoutModel.validate(repaintComponent.getGraphics());
			if ((repaintComponent.getWidth()!=layoutModel.size.width)
					|| (repaintComponent.getHeight()!=layoutModel.size.height)) {
				repaintComponent.setSize(layoutModel.size);
			}
			repaintComponent.repaint();
		}
	}
	
	
	public void validateModelAndRepaint() {
		layoutModel.validate(getGraphics());
		if ((getWidth()!=layoutModel.size.width)
				|| (getHeight()!=layoutModel.size.height)) {
			cat_log_debug("newsize="+layoutModel.size.width+", "+layoutModel.size.height);
			setSize(layoutModel.size);
			setPreferredSize(layoutModel.size);
			JScrollPane sp = (JScrollPane) getParent().getParent();
			sp.setPreferredSize(layoutModel.size);
			invalidate();
		}
		repaint();
	}
	
	
	@Override
	public void transactionCommited(Transaction transaction, MooNode newRootNode) {
		cat_log_debug("transaction commited: newRootNode="+newRootNode);
		SwingUtilities.invokeLater(new LayoutModelSetRootNode(layoutModel, newRootNode, this));
	}
	
	@Override
	public void paint(Graphics g) {
		
		Graphics2D g2 = (Graphics2D)g;
	    g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
	    		RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
	    g2.setRenderingHint(RenderingHints.KEY_RENDERING,
	    		RenderingHints.VALUE_RENDER_QUALITY);
	    
		
		g.setFont(fontje);
		layoutModel.validate(g);
		
		layoutModel.paint(g);
	}
	
	
	
	
	@Override
	public void mouseClicked(MouseEvent e) {
	}
	
	@Override
	public void mouseEntered(MouseEvent e) {
	}
	
	@Override
	public void mouseExited(MouseEvent e) {
	}
	
	@Override
	public void mousePressed(MouseEvent e) {
		
		if (e.getButton()==MouseEvent.BUTTON1) {
			
			int modifiers = e.getModifiers();
			int modifiersEx = e.getModifiersEx();
			
			cat_log_debug("modifiers="+modifiers+", modifiersEx="+modifiersEx);
			
			boolean ctrlPressed = (modifiersEx & KeyEvent.CTRL_DOWN_MASK)!=0;
			boolean shiftPressed = (modifiersEx & KeyEvent.SHIFT_DOWN_MASK)!=0;
			int xmouse = e.getX();
			int ymouse = e.getY();
			NodeLayout nodeAt = layoutModel.nodeAt(xmouse, ymouse);
			boolean tryToggleFirst = false;
			if (nodeAt!=null) {
				LayoutZone zone = nodeAt.getZone(xmouse, ymouse);
				if (zone==LayoutZone.EXPANDER) {
					tryToggleFirst = true;
				}
			}
			if (layoutModel.select(nodeAt, tryToggleFirst, ctrlPressed, shiftPressed)) {
				validateModelAndRepaint();
			}
		} else if (e.getButton() == MouseEvent.BUTTON3) {
			JPopupMenu popupMenu2 = createPopupMenu();
			this.add(popupMenu2);
			popupMenu2.show(this, e.getX(), e.getY());
		}
		
	}

	private JPopupMenu createPopupMenu() {
		JPopupMenu popupmenu = new JPopupMenu();
		popupmenu.add(actionGroup.actionAddModule);
		popupmenu.add(actionGroup.actionDeleteResource);
		
		return popupmenu;
	}
	
	
	
	
	@Override
	public void mouseReleased(MouseEvent e) {
	}
	

	@Override
	public void mouseDragged(MouseEvent e) {
	}
	
	@Override
	public void mouseMoved(MouseEvent e) {
		int xmouse = e.getX();
		int ymouse = e.getY();
		NodeLayout nodeAt = layoutModel.nodeAt(xmouse, ymouse);
		if (nodeAt!=null) {
			LayoutZone zone = nodeAt.getZone(xmouse, ymouse);
		}
	}
	
}
