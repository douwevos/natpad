package net.natpad.avaj.vm.type;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.log.Logger;

public class CrossRuleArrayGet implements ICrossRule {

	static Logger log = Logger.getInstance(ICrossRule.class);

	
	private final ValueTypeDefinitionList main;
	private final ValueTypeDefinitionList arrayList;
	
	public CrossRuleArrayGet(ValueTypeDefinitionList main, ValueTypeDefinitionList arrayList) {
		this.main = main;
		this.arrayList = arrayList;
	}
	
	
	
	@Override
	public boolean apply() {
		for(ValueTypeDefinition valTypeDef : arrayList) {
			DeclarationType arrayTypeDef = valTypeDef.typeDefinition;
			arrayTypeDef = arrayTypeDef.setDimCount(arrayTypeDef.dimCount-1);
			ValueTypeDefinition mainValTypeDef = null;
			for(ValueTypeDefinition testValTypeDef : main) {
				if (testValTypeDef.typeDefinition.equals(arrayTypeDef) && testValTypeDef.kind == valTypeDef.kind) {
					mainValTypeDef = testValTypeDef;
					break;
				}
			}
			
			if (valTypeDef.isIncluded) {
				if (mainValTypeDef==null) {
					mainValTypeDef = new ValueTypeDefinition(arrayTypeDef, valTypeDef.kind);
					main.add(mainValTypeDef);
				}
			} else {
				if (mainValTypeDef!=null) {
					mainValTypeDef.isIncluded = false;
				}
			}
		}
		return arrayList.allCrossRulesApplied();
	}
	
}
