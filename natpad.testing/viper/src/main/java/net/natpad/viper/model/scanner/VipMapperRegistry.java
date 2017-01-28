package net.natpad.viper.model.scanner;

import java.util.ArrayList;

import net.natpad.caterpillar.versionedtree.CatWritableTreeNode;
import net.natpad.viper.model.VipNode;

public class VipMapperRegistry {

	private ArrayList<VipIMapper> mappers = new ArrayList<VipIMapper>();
	private final VipIMapper defaultMapper;
	
	
	public VipMapperRegistry(VipIMapper defaultMapper) {
		this.defaultMapper = defaultMapper;
	}
	
	public void addMapper(VipIMapper mapper) {
		mappers.add(mapper);
	}
	
	

	public VipIScanWork createWorkForNode(CatWritableTreeNode node, boolean recursiveFromParent) {
		for(VipIMapper mapper : mappers) {
			VipIScanWork scanWork = mapper.createWorkForNode(node, recursiveFromParent);
			if (scanWork!=null) {
				return scanWork;
			}
		}
		return defaultMapper.createWorkForNode(node, recursiveFromParent);
	}


}
