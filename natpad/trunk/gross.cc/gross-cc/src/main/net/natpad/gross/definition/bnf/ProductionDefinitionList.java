package net.natpad.gross.definition.bnf;

import java.util.ArrayList;
import java.util.Collections;

public class ProductionDefinitionList {


	public final ProductionDefinitionList prodList;
	public final ProductionDefinition productionDef;
	
	public ProductionDefinitionList(ProductionDefinitionList prodList, ProductionDefinition productionDef) {
		this.prodList = prodList;
		this.productionDef = productionDef;
	}

	
	
	public ProductionDefinition[] enlist() {
		ArrayList<ProductionDefinition> enlisted = new ArrayList<ProductionDefinition>();
		ProductionDefinitionList tdl = this;
		while(tdl!=null) {
			enlisted.add(tdl.productionDef);
			tdl = tdl.prodList;
		}
		Collections.reverse(enlisted);
		return enlisted.toArray(new ProductionDefinition[enlisted.size()]);
	}

}
