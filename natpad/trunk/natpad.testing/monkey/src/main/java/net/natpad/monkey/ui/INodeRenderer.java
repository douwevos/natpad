package net.natpad.monkey.ui;

import java.awt.Graphics;

public interface INodeRenderer {

	void updateLayout(NodeLayout nodeLayout);

	void paint(Graphics g, NodeLayout nodeLayout);

}
