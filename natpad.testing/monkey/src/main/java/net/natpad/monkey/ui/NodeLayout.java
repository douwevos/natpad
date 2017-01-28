package net.natpad.monkey.ui;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Polygon;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;
import java.util.List;

import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.MooResourceContent;
import net.natpad.viper.model.VipIMap;
import net.natpad.viper.model.VipNode;

public class NodeLayout {

	enum LayoutZone {
		NONE,
		EXPANDER,
		ICON,
		NAME,
		EXTRA
	}
	
	public MooNode node;
	private boolean paint_as_map;
	int treeLevel;
	public int fontHeight;
	public boolean isExpanded = false;
	boolean isSelected = false;
	
	boolean needsRepaint = true;
	boolean boundariesInvalid = true;
	public Rectangle boundaries = new Rectangle();
	List<INodeRenderer> renderers;
	

	String cachedName = "";
	int nameWidth;
	int errorWidth;
	
	public ArrayList<NodeLayout> children;
	
	
	public NodeLayout(MooNode node, int treeLevel) {
		setNode(node);
		this.treeLevel = treeLevel;
	}
	
	
	public void setNode(MooNode node) {
		boundariesInvalid = this.node!=node;
		this.node = node;
	}
	
	
	
	public void refreshRendererList(NodeRenderRegistry renderRegistry) {
		if (renderers==null) {
			renderers = renderRegistry.createRenderersForNode(node);
		}
	}

	

	public void updateLayout(Graphics gfx, int ypos, int fontHeight) {
		Rectangle oldBound = new Rectangle(boundaries);
		
		boundaries.y = ypos;
		boundaries.x = treeLevel*fontHeight;
		boundaries.height = fontHeight;
		this.fontHeight = fontHeight;

		paint_as_map = false;
		if (node.count()>0) {
			paint_as_map = true;
		} else {
			MooResourceContent resource = node.getContent(MooResourceContent.KEY);
			if (resource!=null) {
				VipNode viperNode = resource.getViperNode();
				if (viperNode.getResource() instanceof VipIMap) {
					paint_as_map = true;
				}
			}
		}
		
		if (!node.name.equals(cachedName) || boundaries.height!=oldBound.height) {
			cachedName = node.name;
			Rectangle2D stringBounds = gfx.getFontMetrics().getStringBounds(cachedName, gfx);
			needsRepaint = true;
			nameWidth = (int) Math.ceil(stringBounds.getWidth());
		}
		for(INodeRenderer nodeRenderer : renderers) {
			nodeRenderer.updateLayout(this);
		}
		boundaries.width = nameWidth;
		boundaries.width += fontHeight*2;
		
		
		int errorCount = node.getErrorCount();
		if (errorCount>0) {
			
			Font of = gfx.getFont();
			Font f2 = new Font(of.getName(), Font.PLAIN, of.getSize()-3);
			gfx.setFont(f2);
			String txt = ""+errorCount;
			Rectangle2D stringBounds = gfx.getFontMetrics().getStringBounds(txt, gfx);
			errorWidth = 10 + (int) Math.ceil(stringBounds.getWidth());
			boundaries.width += errorWidth+4;
			gfx.setFont(of);
			

		} else {
			errorWidth = 0;
		}

		
		
		if (!needsRepaint) {
			needsRepaint = !oldBound.equals(boundaries);
		}
	}

	public void paint(Graphics g, boolean isCursor) {
	
		
		Rectangle clipBounds = g.getClipBounds();
//		g.setClip(null);
		g.setColor(isSelected ? new Color(172, 205, 138) : Color.WHITE);
		g.fillRect(clipBounds.x, boundaries.y, clipBounds.width, boundaries.height);
//		g.fillRect(0, boundaries.y, 100000, boundaries.height);
		
//		g.setClip(clipBounds);
		g.setColor(Color.LIGHT_GRAY);
		if (isCursor) {
			g.drawRect(clipBounds.x, boundaries.y, clipBounds.width-1, boundaries.height-1);
		}
		

//		cat_log_debug("fontHeight="+fontHeight+", node.count()="+node.count());
		
		if (paint_as_map)  {

			Polygon poly = new Polygon();
//			int rs = 15;
			int r = 9;
			int r2 = r*2;
			poly.addPoint(r-25, r2-50);
			poly.addPoint(25-r, 0);
			poly.addPoint(r-25, 50-r2);

			AffineTransform scaleInstance = AffineTransform.getScaleInstance(fontHeight/100d, fontHeight/100d);
			AffineTransform translateInstance = AffineTransform.getTranslateInstance(boundaries.x+fontHeight/2d, boundaries.y+fontHeight/2d);
			
//			scaleInstance.concatenate(rotateInstance);
			translateInstance.concatenate(scaleInstance);
			if (isExpanded) {
				AffineTransform rotateInstance = AffineTransform.getRotateInstance(Math.PI/2);
				translateInstance.concatenate(rotateInstance);
			}
			
			Graphics2D g2 = (Graphics2D) g.create();
			g2.setColor(Color.DARK_GRAY.brighter());
			AffineTransform transform = g2.getTransform();
			transform.concatenate(translateInstance);
			g2.setTransform(transform);   
			
			g2.fill(poly);
			g2.dispose();

			
//			g.drawRoundRect(boundaries.x+3, boundaries.y+3, fontHeight-6, fontHeight-6, 2, 2);
//			g.drawLine(boundaries.x+6, (boundaries.y+fontHeight/2), boundaries.x+fontHeight-6, (boundaries.y+fontHeight/2));
//			if (!isExpanded) {
//				g.drawLine((boundaries.x+fontHeight/2), boundaries.y+6, (boundaries.x+fontHeight/2), boundaries.y+fontHeight-6);
//			}
		}
		
		
		g.setColor(Color.BLACK);
//		g.drawRect(boundaries.x, boundaries.y, boundaries.width, boundaries.height);


		for(INodeRenderer nodeRenderer : renderers) {
			nodeRenderer.paint(g, this);
		}

		
		

		
		int ascent = g.getFontMetrics().getAscent();
		g.drawString(cachedName, fontHeight*2+boundaries.x, boundaries.y+ascent);
		
		
		int errorCount = node.getErrorCount();
		if (errorCount>0) {
			Graphics2D gl = (Graphics2D) g.create();
			String txt = ""+errorCount;
			gl.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
			if (node.getRecursiveErrorCount().localCount>0) {
				gl.setColor(new Color(255,0,0));
			} else {
				gl.setColor(new Color(255, 128, 128));
			}
			int xpo = fontHeight*2+boundaries.x+nameWidth+4;
			gl.fillRoundRect(xpo, boundaries.y, errorWidth-2, fontHeight-1, 10, 10);
			gl.setColor(new Color(255,255,255));
			Font of = gl.getFont();
			Font f2 = new Font(of.getName(), Font.PLAIN, of.getSize()-3);
			gl.setFont(f2);
			gl.drawString(txt, xpo+4, boundaries.y+ascent-2);
		}

		
	}

	
	public LayoutZone getZone(int xpos, int ypos) {
		xpos -= boundaries.x;
		if (xpos<0 || xpos>=boundaries.width) {
			return LayoutZone.NONE;
		}
		if (xpos<=fontHeight) {
			return LayoutZone.EXPANDER;
		}
		if (xpos<=2*fontHeight) {
			return LayoutZone.ICON;
		}
		if (xpos<=2*fontHeight+nameWidth) {
			return LayoutZone.ICON;
		}
		return LayoutZone.EXTRA;
	}

	
	public boolean is_a_map() {
		return paint_as_map;
	}

	public int getY() {
		return boundaries.y;
	}

	public int getX() {
		return boundaries.x;
	}

	public int getWidth() {
		return boundaries.width;
	}

	public int getHeight() {
		return boundaries.height;
	}


	public void setChildren(ArrayList<NodeLayout> newChildren) {
		this.children = newChildren;
	}
	
	public ArrayList<NodeLayout> getChildren() {
		return children;
	}


//	public void enlistRecursive(List<NodeLayout> nodeLayouts) {
//		if (this.treeLevel>=0) {
//			nodeLayouts.add(this);
//		}
//		if (children!=null) {
//			for (NodeLayout child : children) {
//				child.enlistRecursive(nodeLayouts);
//			}
//		}
//	}
	
	
	
}
