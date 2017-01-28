package net.natpad.avaj.vm.type;

import java.util.HashSet;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.log.Logger;

public class CrossRuleLeftValue implements ICrossRule {

	static Logger log = Logger.getInstance(ICrossRule.class);

	private final ValueTypeDefinitionList left;
	private final ValueTypeDefinitionList right;

	
	public CrossRuleLeftValue(ValueTypeDefinitionList left, ValueTypeDefinitionList right) {
		this.left = left;
		this.right = right;
		log.detail(">>> left="+left+", right="+right+", this="+this);
	}

	

	private void copySet(ValueTypeDefinitionList source, ValueTypeDefinitionList destination) {
		for(ValueTypeDefinition sourceValTypeDef : source) {
			if (sourceValTypeDef.isIncluded) {
				destination.add(sourceValTypeDef);
			}
		}
	}

	
	@Override
	public boolean apply() {
		
//		log.detail("left="+left);
//		log.detail("right="+right);
		
		boolean result = (left.crossRulesRunning() || left.allCrossRulesApplied()) && 
				(right.crossRulesRunning() || right.allCrossRulesApplied());
		
		if (left.size()==0) {
			if (right.size()!=0) {
				copySet(right, left);
			}
		} else if (right.size()==0) {
			copySet(left, right);
		} else {
			
			
			HashSet<DeclarationType> primSetA = new HashSet<DeclarationType>();
			HashSet<DeclarationType> refSetA = new HashSet<DeclarationType>();
			
			for(ValueTypeDefinition valTypeDefA : left) {
				if (valTypeDefA.isIncluded) {
					if (valTypeDefA.typeDefinition.isPrimitive()) {
						primSetA.add(valTypeDefA.typeDefinition);
					} else {
						refSetA.add(valTypeDefA.typeDefinition);
						
					}
				}
			}
			

			HashSet<DeclarationType> primSetB = new HashSet<DeclarationType>();
			HashSet<DeclarationType> refSetB = new HashSet<DeclarationType>();
			
			for(ValueTypeDefinition valTypeDefB : right) {
				if (valTypeDefB.isIncluded) {
					if (valTypeDefB.typeDefinition.isPrimitive()) {
						primSetB.add(valTypeDefB.typeDefinition);
					} else {
						refSetB.add(valTypeDefB.typeDefinition);
						
					}
				}
			}
			
			if (!primSetA.isEmpty() || !primSetB.isEmpty()) {
				HashSet<DeclarationType> primSet = new HashSet<DeclarationType>();
				primSet.addAll(primSetA);
				primSet.removeAll(primSetB);
				if (primSet.size()==0 || primSet.size()==primSetA.size()) {
					log.warn("result would be an empty type-set");
				} else {
					left.excludeSet(primSet, true);
					right.excludeSet(primSet, true);
				}
			}

			if (!refSetA.isEmpty() || !refSetB.isEmpty()) {
				HashSet<DeclarationType> refSet = new HashSet<DeclarationType>();
				refSet.addAll(refSetA);
				refSet.removeAll(refSetB);
				if (refSet.size()==0 || refSet.size()==refSetA.size()) {
					log.warn("result would be an empty type-set");
				} else {
					left.excludeSet(refSet, true);
					right.excludeSet(refSet, true);
				}
			}
		}
		return result;
	}
	
}
