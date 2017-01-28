package net.natpad.dung.module;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;

public class DependencyTree {

	public final Module root;
	
	private Node node;
	
	public Node[][] levels;

	public DependencyTree(Module root) {
		this.root = root;

		HashMap<IDependency, Node> map = new HashMap<IDependency, Node>();

		node = new Node(root, map);
		int maxLevel = node.setLevel(0, new HashSet<IDependency>());
		maxLevel++;
		levels = new Node[maxLevel][];
		ArrayList<Node> nodeInLevel = new ArrayList<>();
		for(int level=0; level<maxLevel; level++) {
			for(Node node : map.values()) {
				if (node.level == level) {
					nodeInLevel.add(node);
				}
			}
//			System.out.println("level:"+level+" nodes:"+nodeInLevel.size());
			Node nn[] = new Node[nodeInLevel.size()];
			levels[level] = nodeInLevel.toArray(nn);
			nodeInLevel.clear();
		}
	}

	public int levelCount() {
		return levels.length;
	}
	
	public IDependency[] getAtLevel(int level) {
		Node[] nodes = levels[level];
		IDependency[] result = new IDependency[nodes.length];
		for(int idx=0; idx<nodes.length; idx++) {
			result[idx] = nodes[idx].dependency;
		}
		return result;
	}
	
	
	public static class Node {

		public final IDependency dependency;
		private int level=-1;
		private Node children[];

		public Node(IDependency dependency, HashMap<IDependency, Node> map) {
			map.put(dependency, this);
			this.dependency = dependency;
			IDependency[] depchildren = dependency.children();
			if (depchildren != null) {
				children = new Node[depchildren.length];
				for (int idx = 0; idx < children.length; idx++) {
					IDependency iDependency = depchildren[idx];
					Node childNode = map.get(iDependency);
					if (childNode == null) {
						childNode = new Node(iDependency, map);
					}
					children[idx] = childNode;
				}
			}
		}

		public int setLevel(int level, HashSet<IDependency> cirDet) {
//			System.out.println("this:"+this+", level="+level);
			if (level<=this.level) {
				return this.level;
			}
			int result = level;
			if (cirDet.add(this.dependency)) {
				this.level = level;
				level++;
				if (children!=null) {
					for (Node childNode : children) {
						int setLevel = childNode.setLevel(level, cirDet);
						if (result<setLevel) {
							result = setLevel;
						}
					}
				}
				cirDet.remove(this.dependency);
			} else {
				throw new RuntimeException("ciruclair dependency detected:"+dependency);
			}
			return result;
		}
	}
}
