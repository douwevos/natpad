package net.natpad.jaguar.monkey.ui;

import net.natpad.jaguar.monkey.JagClassfileContent;
import net.natpad.jaguar.monkey.JagNodeCompiler;
import net.natpad.jaguar.monkey.JagPackageContent;
import net.natpad.jaguar.monkey.JagSrcFolderContent;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.MooResourceContent;
import net.natpad.monkey.ui.INodeRenderFactory;
import net.natpad.monkey.ui.INodeRenderer;

public class jagNodeRendererFactory implements INodeRenderFactory {

	JarNodeRenderer renderer = new JarNodeRenderer();
	JagSrcFolderRenderer srcFolderRenderer = new JagSrcFolderRenderer();
	JagPackageRenderer packageRenderer = new JagPackageRenderer();
	JagClassfileRenderer classfileRenderer = new JagClassfileRenderer();
	JagCompilerRenderer compilerRenderer = new JagCompilerRenderer();
	
	@Override
	public INodeRenderer requestRendererForNode(MooNode node) {
		
		MooResourceContent resourceContent = node.getContent(MooResourceContent.KEY);
		if (resourceContent!=null) {
			String name = node.name;
			if ((name.endsWith(".jar")) || (name.endsWith(".ear")) || (name.endsWith(".war"))) {
				return renderer;
			}
		}
		if (node.getContent(JagSrcFolderContent.KEY)!=null) {
			return srcFolderRenderer;
		}
		if (node.getContent(JagPackageContent.KEY)!=null) {
			return packageRenderer;
		}
		if (node.getContent(JagClassfileContent.KEY)!=null) {
			return classfileRenderer;
		}
		if (node.getContent(JagNodeCompiler.KEY)!=null) {
			return compilerRenderer;
		}
		return null;
	}
	
}
