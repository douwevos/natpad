package net.natpad.avaj.vm.type;

import java.util.Set;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.log.Logger;

public class CrossRuleMatchForCondition implements ICrossRule {

	static Logger log = Logger.getInstance(ICrossRule.class);

	
	private final ValueTypeDefinitionList listA;
	private final ValueTypeDefinitionList listB;
	
	public CrossRuleMatchForCondition(ValueTypeDefinitionList listA, ValueTypeDefinitionList listB) {
		this.listA = listA;
		this.listB = listB;
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
		
		boolean result = (listA.crossRulesRunning() || listA.allCrossRulesApplied()) && 
				(listB.crossRulesRunning() || listB.allCrossRulesApplied());
		
		if (listA.size()==0) {
			if (listB.size()!=0) {
				copySet(listB, listA);
			}
		} else if (listB.size()==0) {
			copySet(listA, listB);
		} else {
			

			Set<DeclarationType> setA = listA.enlistIncluded();
			Set<DeclarationType> setB = listA.enlistIncluded();

			setA.retainAll(setB);
			
			if (setA.size()==0) {
				log.warn("result would be an empty type-set");
			} else {
				listA.excludeSet(setA, true);
				listB.excludeSet(setA, true);
			}
		}
		return result;
	}
	
}
