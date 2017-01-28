package net.natpad.monkey.ui;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Polygon;
import java.awt.Rectangle;
import java.awt.geom.AffineTransform;
import java.awt.geom.GeneralPath;

import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.MooResourceContent;
import net.natpad.viper.model.VipIMap;
import net.natpad.viper.model.VipNode;

public class DefaultNodeRenderer implements INodeRenderer {

	
	@Override
	public void updateLayout(NodeLayout nodeLayout) { 
	}

	
	@Override
	public void paint(Graphics g, NodeLayout nodeLayout) {

		MooNode node = nodeLayout.node;
		int fontHeight = nodeLayout.fontHeight;
		Rectangle boundaries = nodeLayout.boundaries;
		boolean isExpanded = nodeLayout.isExpanded;
		boolean paintMap = false;
		
		MooResourceContent resource = node.getContent(MooResourceContent.KEY);
		
		if (resource!=null) {
			VipNode viperNode = resource.getViperNode();
			if (viperNode.getResource() instanceof VipIMap) {
				paintMap = true;
			}
		}


		

		AffineTransform scaleInstance = AffineTransform.getScaleInstance(fontHeight/100d, fontHeight/100d);
		AffineTransform translateInstance = AffineTransform.getTranslateInstance(boundaries.x+fontHeight, boundaries.y);
		
		translateInstance.concatenate(scaleInstance);
		
		Graphics2D g2 = (Graphics2D) g.create();
		AffineTransform transform = g2.getTransform();
		transform.concatenate(translateInstance);
		g2.setTransform(transform);   

		if (paintMap) {
			drawMap(g2, isExpanded, false);
		} else {
			drawFile(g2);
		}
		
		g2.dispose();
			
	}


	public void drawFile(Graphics2D g2) {
		Polygon poly = new Polygon();
		poly.addPoint(25, 10);
		poly.addPoint(55, 10);
		poly.addPoint(75, 30);
		poly.addPoint(75, 90);
		poly.addPoint(25, 90);
		poly.addPoint(25, 10);
		
		g2.draw(poly);
	}


	public void drawMap(Graphics2D g2, boolean asOpened, boolean withBlue) {
		int x1  = 05;
		int x2  = 10;
		int x3  = 25;
		int x3b = 35;
		int x4  = 75;
		int x5  = 95;

		int y1 = 20;
		int y2 = 30;
		int y3 = 45;
		int y4 = 80;

		
		if (!asOpened) {
			Polygon poly = new Polygon();
			poly.addPoint(x4, y3);
			poly.addPoint(x4, y4);
			poly.addPoint(x1, y4);
			poly.addPoint(x1, y2);
			poly.addPoint(x2, y1);
			poly.addPoint(x3b, y1);
			poly.addPoint(x3b, y2);
			poly.addPoint(x4, y2);
			poly.addPoint(x4, y3);
			
			if (withBlue) {
				g2.setColor(new Color(0.7f,0.7f,1.0f));
			} else {
				g2.setColor(new Color(1.0f,1.0f,0.5f));
			}
			g2.fill(poly);
			
			g2.setColor(new Color(0.0f,0.0f,0.0f));
			g2.draw(poly);
		} else {
			
			if (withBlue) {
				Polygon poly = new Polygon();
				poly.addPoint(x1, y4);
				poly.addPoint(x1, y2);
				poly.addPoint(x2, y1);
				poly.addPoint(x3b, y1);
				poly.addPoint(x3b, y2);
				poly.addPoint(x4, y2);
				poly.addPoint(x4, y3);
				poly.addPoint(x3, y3);
				poly.addPoint(x1, y4);

				g2.setColor(new Color(1.0f,1.0f,0.5f));
				g2.fill(poly);

				g2.setColor(new Color(0.0f,0.0f,0.0f));
				g2.draw(poly);

				poly = new Polygon();
				poly.addPoint(x1, y4);
				poly.addPoint(x3, y3);
				poly.addPoint(x5, y3);
				poly.addPoint(x4, y4);
				poly.addPoint(x1, y4);

				g2.setColor(new Color(0.7f,0.7f,1.0f));
				g2.fill(poly);

				g2.setColor(new Color(0.0f,0.0f,0.0f));
				g2.draw(poly);

			} else {
				Polygon poly = new Polygon();
				GeneralPath gp = new GeneralPath();

				gp.moveTo(x1, y4);
				gp.lineTo(x3, y3);
				gp.lineTo(x5, y3);
				gp.lineTo(x4, y4);
				gp.lineTo(x1, y4);
				gp.lineTo(x1, y2);
				gp.lineTo(x2, y1);
				gp.lineTo(x3b, y1);
				gp.lineTo(x3b, y2);
				gp.lineTo(x4, y2);
				gp.lineTo(x4, y3);

				g2.setColor(new Color(1.0f,1.0f,0.5f));
				g2.fill(gp);

				
				g2.setColor(new Color(0.0f,0.0f,0.0f));
				g2.draw(gp);
			}				
		}

		
		//			poly.addPoint(55, 10);
//			poly.addPoint(75, 30);
//			poly.addPoint(75, 90);
//			poly.addPoint(25, 90);
//			poly.addPoint(25, 10);
//			
//			AffineTransform scaleInstance = AffineTransform.getScaleInstance(fontHeight/100d, fontHeight/100d);
//			AffineTransform translateInstance = AffineTransform.getTranslateInstance(boundaries.x+fontHeight, boundaries.y);
//			
//			translateInstance.concatenate(scaleInstance);
//			
//			Graphics2D g2 = (Graphics2D) g.create();
//			AffineTransform transform = g2.getTransform();
//			transform.concatenate(translateInstance);
//			g2.setTransform(transform);   
//		    g2.draw(poly);
//		    g2.dispose();
	}
	
	
}
