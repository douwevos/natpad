package net.natpad.viper.model.scanner;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import java.util.ArrayList;
import java.util.Arrays;

import net.natpad.caterpillar.CatLogLevel;
import net.natpad.caterpillar.CatLogLevels;
import net.natpad.caterpillar.versionedtree.CatWritableTreeNode;
import net.natpad.viper.model.VipNode;

public class VipNodeScanner {

	
	private final VipMapperRegistry mapperRegistry;
	
	public VipNodeScanner(VipMapperRegistry mapperRegistry) {
		this.mapperRegistry = mapperRegistry;
	}

	
	public void scan(CatWritableTreeNode<VipNode> node) {
		cat_log_debug("scan:node="+node);
		ArrayList<VipIScanWork> stack = new ArrayList<VipIScanWork>();
		VipIScanWork mainWork = mapperRegistry.createWorkForNode(node, false);
		stack.add(mainWork);
		
		while(!stack.isEmpty()) {
			
			int index = stack.size()-1;
			VipIScanWork workNode = stack.get(index);
			cat_log_debug("workNode="+workNode);
			
			VipIScanWork[] childWork = workNode.initChildWork(mapperRegistry);
			if (childWork!=null) {
				stack.addAll(Arrays.asList(childWork));
			} else {
				workNode.runScan();
				stack.remove(index);
			}
		}
		cat_log_debug("mainWork.resultNode="+mainWork);
	}

}
