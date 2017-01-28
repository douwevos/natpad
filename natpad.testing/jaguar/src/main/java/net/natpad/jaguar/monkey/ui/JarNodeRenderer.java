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

public class JarNodeRenderer implements INodeRenderer {

	@Override
	public void paint(Graphics g, NodeLayout nodeLayout) {

		int fontHeight = nodeLayout.getHeight();
		Rectangle boundaries = nodeLayout.boundaries;

		AffineTransform scaleInstance = AffineTransform.getScaleInstance(fontHeight / 110d, fontHeight / 100d);
		AffineTransform translateInstance = AffineTransform.getTranslateInstance(boundaries.x + fontHeight,
				boundaries.y);

		translateInstance.concatenate(scaleInstance);

		Graphics2D g2 = (Graphics2D) g.create();
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		AffineTransform transform = g2.getTransform();
		transform.concatenate(translateInstance);
		g2.setTransform(transform);
		g2.setColor(Color.GREEN);

		drawJar(g2, fontHeight);
		
//		Polygon poly = new Polygon();
//		poly.addPoint(25, 10);
//		poly.addPoint(55, 10);
//		poly.addPoint(75, 30);
//		poly.addPoint(75, 90);
//		poly.addPoint(25, 90);
//		poly.addPoint(25, 10);
//
//		g2.draw(poly);

		g2.dispose();

	}

	@Override
	public void updateLayout(NodeLayout nodeLayout) {
	}

	void drawJar(Graphics2D gfx, double fontHeight) {

		int size = 100;

		double cap_diff = 0.08;

		double cap_top = 0.13;
		double cap_bottom = 0.30;

		double x10 = (size * 0.05);
		double x1 = (size * 0.10);
		double x11 = (size * 0.15);

		double y1 = (size * (cap_top - cap_diff));
		double y2 = (size * cap_top);
		double y3 = (size * (cap_top + cap_diff));
		double y4 = (size * cap_bottom);
		double y5 = (size * (cap_bottom + cap_diff));

		double y6 = (size * 0.72);
		double y7 = (size * 0.76);

		double y8 = (size * 0.83);

		double y9 = (size * 0.90);

		double x20 = (size * 0.95);
		double x2 = (size * 0.90);
		double x21 = (size * 0.85);

		// cairo_set_line_width(cairo, 0.75);
		gfx.setStroke(new BasicStroke((float) (0.75*100/fontHeight)));

		GeneralPath gp = new GeneralPath();

		/* glass */
		gp.moveTo(x11, y4);

		gp.lineTo(x10, y5);

		gp.lineTo(x10, y6);
		gp.curveTo(x10, y7, x10, y7, x11, y8);

		gp.curveTo(x11, y9, x21, y9, x21, y8);

		gp.curveTo(x20, y7, x20, y7, x20, y6);
		gp.lineTo(x20, y5);

		gp.lineTo(x21, y4);
		gp.closePath();
		// cairo_close_path(cairo);

		gfx.setColor(new Color(0.85f, 0.85f, 1.0f));
		gfx.fill(gp);

		gfx.setColor(new Color(0.0f, 0.0f, 0.0f));
		gfx.draw(gp);

		/* cap */
		gp = new GeneralPath();

		// cairo_set_line_width(cairo, 1.0);
		gfx.setStroke(new BasicStroke((float) (1.0*100/fontHeight)));

		gp.moveTo(x1, y2);
		gp.curveTo(x1, y1, x2, y1, x2, y2);
		gp.lineTo(x2, y4);
		gp.curveTo(x2, y5, x1, y5, x1, y4);
		gp.lineTo(x1, y2);

		gp.curveTo(x1, y3, x2, y3, x2, y2);

		gfx.setColor(new Color(0.3f, 0.8f, 0.3f));
		gfx.fill(gp);
		gfx.setColor(new Color(0.0f, 0.3f, 0.0f));
		gfx.draw(gp);
	}

}
