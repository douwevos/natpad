package net.natpad.jaguar.monkey.ui;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Polygon;
import java.awt.Rectangle;
import java.awt.font.TextAttribute;
import java.awt.geom.AffineTransform;
import java.util.Map;

import net.natpad.monkey.ui.INodeRenderer;
import net.natpad.monkey.ui.NodeLayout;

public class JagClassfileRenderer implements INodeRenderer {

	
	@Override
	public void updateLayout(NodeLayout nodeLayout) { 
	}

	
	@Override
	public void paint(Graphics g, NodeLayout nodeLayout) {

		int fontHeight = nodeLayout.fontHeight;
		Rectangle boundaries = nodeLayout.boundaries;

		

		AffineTransform scaleInstance = AffineTransform.getScaleInstance(fontHeight/100d, fontHeight/100d);
		AffineTransform translateInstance = AffineTransform.getTranslateInstance(boundaries.x+fontHeight, boundaries.y);
		
		translateInstance.concatenate(scaleInstance);
		
		Graphics2D g2 = (Graphics2D) g.create();
		AffineTransform transform = g2.getTransform();
		AffineTransform transformOld = (AffineTransform) transform.clone();
		transform.concatenate(translateInstance);
		g2.setTransform(transform);   

		drawFile(g2);
		
		
		
		int maxAscent = g2.getFontMetrics().getMaxAscent();
		translateInstance = AffineTransform.getTranslateInstance(boundaries.x+fontHeight+fontHeight*0.41, boundaries.y+maxAscent-fontHeight*0.027);
		transformOld.concatenate(translateInstance);
		scaleInstance = AffineTransform.getScaleInstance(14.0/fontHeight, 14.0/fontHeight);
		transformOld.concatenate(scaleInstance);
		
		g2.setTransform(transformOld);   
		g2.setColor(new Color(0f,0.6f,0f));


		Font fontje = new Font("Ubuntu", Font.PLAIN, 15);
		Map<TextAttribute, Object> attributes = (Map<TextAttribute, Object>) fontje.getAttributes();
		attributes.put(TextAttribute.WEIGHT, TextAttribute.WEIGHT_ULTRABOLD);
		fontje = new Font(attributes);
		g2.setFont(fontje);

		

		g2.drawString("J",0f,0f);
		
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

}
