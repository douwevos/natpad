package net.natpad.avaj.vm.subcode;

import java.util.List;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

public interface IValueProducer {

	public ValueTypeDefinitionList getTypeSet();
	
	public IExpression getExpression(DeclarationType description);
	
	public void setExpression(IExpression expression);
	
	public void addInputConsumer(IConsumer inputConsumer);
	
	public void setConsumer(IConsumer consumer);
	public IConsumer getConsumer();
	
	public boolean isConsumed();
	
	IValueProducer duplicate();
	
	public IIntermediateNode getIntermediateNode();

	List<IConsumer> getInputConsumers();
	
}
