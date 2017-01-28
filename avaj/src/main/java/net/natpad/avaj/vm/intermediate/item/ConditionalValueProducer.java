package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IConditionalExpression;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

class ConditionalValueProducer extends DefaultValueProducer {
	
	
	private final Consumer conditionConsumer;
	private final boolean invertExpression;
	
	public ConditionalValueProducer(IIntermediateNode owner, Consumer conditionConsumer, boolean invertExpression) {
		super(owner);
		this.conditionConsumer = conditionConsumer;
		this.invertExpression = invertExpression;
		addInputConsumer(conditionConsumer);
		
	}
	
	@Override
	public ValueTypeDefinitionList getTypeSet() {
		return conditionConsumer.getTypeSet();
	}
	

	@Override
	public IConditionalExpression getExpression(DeclarationType requested) {
		IConditionalExpression  condExpression = (IConditionalExpression) conditionConsumer.getExpression(DeclarationType.DEFAULT_BOOLEAN);
		if (condExpression!=null && invertExpression) {
			condExpression = condExpression.invert();
		}
		return condExpression;
	}
	
	
	
	
}