package net.natpad.monkey.ui;

import net.natpad.monkey.model.MooNode;

public interface INodeRenderFactory {

	INodeRenderer requestRendererForNode(MooNode node);

}
