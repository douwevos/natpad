package net.natpad.jaguar.monkey.ui;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.font.TextAttribute;
import java.awt.geom.AffineTransform;
import java.awt.geom.GeneralPath;
import java.util.Map;

import net.natpad.monkey.ui.INodeRenderer;
import net.natpad.monkey.ui.NodeLayout;

public class JagCompilerRenderer implements INodeRenderer {

		
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
			g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
			AffineTransform transform = g2.getTransform();
			AffineTransform transformOld = (AffineTransform) transform.clone();
			transform.concatenate(translateInstance);
			g2.setTransform(transform);   

			drawIt(g2);
			
			g2.dispose();
				
		}


	
	void drawIt(Graphics2D g2) {

		g2.setColor(new Color(0.4f, 0.6f, 0.9f));

		GeneralPath gp = new GeneralPath();

		double size = 100;

		gp.moveTo(0.5+0.4364888*size, 0.5+0.9202277*size);
		gp.lineTo(0.5+0.5635112*size, 0.5+0.9202277*size);
		gp.lineTo(0.5+0.58981556*size, 0.5+0.812343*size);
		gp.lineTo(0.5+0.65735066*size, 0.5+0.78436905*size);
		gp.lineTo(0.5+0.75223666*size, 0.5+0.8420551*size);
		gp.lineTo(0.5+0.8420551*size, 0.5+0.75223666*size);
		gp.lineTo(0.5+0.78436905*size, 0.5+0.65735066*size);
		gp.lineTo(0.5+0.812343*size, 0.5+0.58981556*size);
		gp.lineTo(0.5+0.9202277*size, 0.5+0.5635112*size);
		gp.lineTo(0.5+0.9202277*size, 0.5+0.4364888*size);
		gp.lineTo(0.5+0.812343*size, 0.5+0.4101844*size);
		gp.lineTo(0.5+0.78436905*size, 0.5+0.34264934*size);
		gp.lineTo(0.5+0.8420551*size, 0.5+0.24776334*size);
		gp.lineTo(0.5+0.75223666*size, 0.5+0.15794493*size);
		gp.lineTo(0.5+0.65735066*size, 0.5+0.21563092*size);
		gp.lineTo(0.5+0.58981556*size, 0.5+0.18765698*size);
		gp.lineTo(0.5+0.5635112*size, 0.5+0.07977229*size);
		gp.lineTo(0.5+0.4364888*size, 0.5+0.07977229*size);
		gp.lineTo(0.5+0.4101844*size, 0.5+0.18765698*size);
		gp.lineTo(0.5+0.34264934*size, 0.5+0.21563092*size);
		gp.lineTo(0.5+0.24776334*size, 0.5+0.15794493*size);
		gp.lineTo(0.5+0.15794493*size, 0.5+0.24776334*size);
		gp.lineTo(0.5+0.21563092*size, 0.5+0.34264934*size);
		gp.lineTo(0.5+0.18765698*size, 0.5+0.4101844*size);
		gp.lineTo(0.5+0.07977229*size, 0.5+0.4364888*size);
		gp.lineTo(0.5+0.07977229*size, 0.5+0.5635112*size);
		gp.lineTo(0.5+0.18765698*size, 0.5+0.58981556*size);
		gp.lineTo(0.5+0.21563092*size, 0.5+0.65735066*size);
		gp.lineTo(0.5+0.15794493*size, 0.5+0.75223666*size);
		gp.lineTo(0.5+0.24776334*size, 0.5+0.8420551*size);
		gp.lineTo(0.5+0.34264934*size, 0.5+0.78436905*size);
		gp.lineTo(0.5+0.4101844*size, 0.5+0.812343*size);
		gp.closePath();

		g2.fill(gp);

		g2.setStroke(new BasicStroke((float) (5.0)));
		g2.setColor(new Color(0.0f, 0.0f, 0.0f));
		g2.draw(gp);


//		double xc  = 0.5+xoffset+(size*0.5);
//		double yc = 0.5+yoffset+(size*0.5);

		double xc  = 0.5+(size*0.5);
		double yc = 0.5+(size*0.5);

		double ac = size*0.15;
		
		g2.setColor(new Color(1.0f, 1.0f, 1.0f));
		g2.fillArc((int) (xc-ac), (int) (yc-ac), (int) (2.0*ac), (int) (2.0*ac), 0, 360);

		g2.setColor(new Color(0.0f, 0.0f, 0.0f));
		g2.drawArc((int) (xc-ac), (int) (yc-ac), (int) (2.0*ac), (int) (2.0*ac), 0, 360);

//		cairo_move_to(cairo, xc, yc);
//		cairo_arc(cairo, xc, xc, size*0.1, 0, 2*M_PI);
//		cairo_stroke_preserve(cairo);
//		g2.setColor(new Color(1.0f, 1.0f, 1.0f));
//		cairo_fill(cairo);
		
	}
}
