package net.natpad.monkey.ui;

import java.util.Comparator;

import net.natpad.monkey.model.MooNode;

public class NodeComparator implements Comparator<MooNode> {

	
	
	public NodeComparator() {
		// TODO Auto-generated constructor stub
	}
	
	
	@Override
	public int compare(MooNode o1, MooNode o2) {
		if (o1.zorder>o2.zorder) {
			return 1;
		} else if (o1.zorder<o2.zorder) {
			return -1;
		}
		return -o1.name.compareTo(o2.name);
	}
	
}
