package net.natpad.monkey.ui;

import java.util.ArrayList;
import java.util.List;

import net.natpad.monkey.model.MooNode;

public class NodeRenderRegistry {

	private ArrayList<INodeRenderFactory> renderFactories = new ArrayList<INodeRenderFactory>();
	
	private INodeRenderer defaultRenderer = new DefaultNodeRenderer();
	
	public void addNodeRenderer(INodeRenderFactory nodeRendererFactory) {
		renderFactories.add(nodeRendererFactory);
	}
	
	public List<INodeRenderer> createRenderersForNode(MooNode node) {
		ArrayList<INodeRenderer> result = new ArrayList<INodeRenderer>();
		for(INodeRenderFactory factory : renderFactories) {
			INodeRenderer renderer = factory.requestRendererForNode(node );
			if (renderer!=null) {
				result.add(renderer);
			}
		}
		if (result.isEmpty()) {
			result.add(defaultRenderer);
		}
		return result;
	}
	
}
