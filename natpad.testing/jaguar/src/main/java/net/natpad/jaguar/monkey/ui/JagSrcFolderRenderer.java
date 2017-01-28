package net.natpad.jaguar.monkey.ui;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.geom.AffineTransform;

import net.natpad.monkey.ui.DefaultNodeRenderer;
import net.natpad.monkey.ui.INodeRenderer;
import net.natpad.monkey.ui.NodeLayout;

public class JagSrcFolderRenderer implements INodeRenderer {

	
	JagPackageRenderer packageRender = new JagPackageRenderer();
	DefaultNodeRenderer defaultNodeRenderer = new DefaultNodeRenderer();
	
	@Override
	public void paint(Graphics g, NodeLayout nodeLayout) {
		int fontHeight = nodeLayout.fontHeight;
		boolean isExpanded = nodeLayout.isExpanded;
		Rectangle boundaries = nodeLayout.boundaries;
		
		AffineTransform scaleInstance = AffineTransform.getScaleInstance(fontHeight/100d, fontHeight/100d);
		AffineTransform translateInstance = AffineTransform.getTranslateInstance(boundaries.x+fontHeight, boundaries.y);
		
		translateInstance.concatenate(scaleInstance);
		
		Graphics2D g2 = (Graphics2D) g.create();
		AffineTransform transform = g2.getTransform();
		transform.concatenate(translateInstance);
		g2.setTransform(transform);   

		defaultNodeRenderer.drawMap(g2, isExpanded, true);

		translateInstance = AffineTransform.getTranslateInstance(32, 32);
		transform.concatenate(translateInstance);
		g2.setTransform(transform);   
		
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		packageRender.drawPackage(g2, true, 65);
		
		g2.dispose();
		
		
	}

	@Override
	public void updateLayout(NodeLayout nodeLayout) {
	}

	
}
