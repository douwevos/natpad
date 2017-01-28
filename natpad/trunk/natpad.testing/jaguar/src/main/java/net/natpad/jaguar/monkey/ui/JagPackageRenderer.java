package net.natpad.jaguar.monkey.ui;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.geom.AffineTransform;
import java.awt.geom.GeneralPath;

import net.natpad.monkey.ui.INodeRenderer;
import net.natpad.monkey.ui.NodeLayout;

public class JagPackageRenderer implements INodeRenderer {

	@Override
	public void paint(Graphics g, NodeLayout nodeLayout) {
		int fontHeight = nodeLayout.fontHeight;
		boolean isExpanded = nodeLayout.isExpanded;
		Rectangle boundaries = nodeLayout.boundaries;
		boolean hasChildren = nodeLayout.node.count()>0;
		
		
		AffineTransform scaleInstance = AffineTransform.getScaleInstance(fontHeight/100d, fontHeight/100d);
		AffineTransform translateInstance = AffineTransform.getTranslateInstance(boundaries.x+fontHeight, boundaries.y);
		
		translateInstance.concatenate(scaleInstance);
		
		Graphics2D g2 = (Graphics2D) g.create();
		AffineTransform transform = g2.getTransform();
		transform.concatenate(translateInstance);
		g2.setTransform(transform);   
		
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		drawPackage(g2, hasChildren, 100);
		
		g2.dispose();
		
	}

	
	@Override
	public void updateLayout(NodeLayout nodeLayout) {
	}

	
	
	public void drawPackage(Graphics2D g2, boolean non_empty, double size) {


		double x1  = size*0.15;
		double x2  = size*0.20;
		double x3  = size*0.50;
		double x4  = size*0.80;
		double x5  = size*0.85;


		double y1 = size*0.15;
		double y2 = size*0.20;
		double y3 = size*0.50;
		double y4 = size*0.80;
		double y5 = size*0.85;



		double radius = 0.15 *size;

		
		GeneralPath gp = new GeneralPath();

		gp.moveTo(x2, y2 + radius);
		gp.curveTo(x2 , y2, x2 , y2, x2 + radius, y2);
		gp.lineTo(x4 - radius, y2);
		gp.curveTo(x4, y2, x4, y2, x4, y2 + radius);
		gp.lineTo(x4 , y4 - radius);
		gp.curveTo(x4, y4, x4, y4, x4 - radius, y4);
		gp.lineTo(x2 + radius, y4);
		gp.curveTo(x2, y4, x2, y4, x2, y4- radius);
		gp.closePath();

		if (non_empty) {
			g2.setColor(new Color(1.0f,0.8f,0.4f));
		} else {
			g2.setColor(new Color(0.8f,0.8f,1.0f));
		}

		g2.fill(gp);

//		cairo_set_line_width(cairo, 1.5);
		g2.setStroke(new BasicStroke((float) (3.0*200/size)));

		
//		cairo_set_source_rgba(cairo, 0.7,0.5,0.1, 0.7);
		g2.setColor(new Color(0.4f,0.2f,0.0f, 0.7f));
		g2.draw(gp);

//		cairo_set_line_width(cairo, 0.5);
		g2.setStroke(new BasicStroke((float) (2.5)));


		g2.setColor(new Color(0.0f,0.0f,0.0f));

		gp = new GeneralPath();
		gp.moveTo(x1, y3);
		gp.lineTo(x5, y3);
		g2.draw(gp);

		
		gp = new GeneralPath();
		gp.moveTo(x3, y1);
		gp.lineTo(x3, y5);
		g2.draw(gp);
		
	}
	
}
