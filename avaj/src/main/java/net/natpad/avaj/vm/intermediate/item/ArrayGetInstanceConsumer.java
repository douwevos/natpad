package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.IValueProducer;
import net.natpad.avaj.vm.type.CrossRuleArrayGet;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

public class ArrayGetInstanceConsumer extends Consumer {
	
	private final IValueProducer resultProducer;
	
	public ArrayGetInstanceConsumer(IIntermediateNode node, IValueProducer resultProducer) {
		super(node);
		this.resultProducer = resultProducer;
	}
	
	@Override
	public void consume(IValueProducer producer) {
		// TODO Auto-generated method stub
		super.consume(producer);
		
		ValueTypeDefinitionList resultTypeSet = resultProducer.getTypeSet();
		
		ValueTypeDefinitionList instanceTypeSet = getTypeSet();
		
		resultTypeSet.addCrossRule(new CrossRuleArrayGet(resultTypeSet, instanceTypeSet));
	}
	
}