package net.natpad.avaj.vm.subcode;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;

public interface IConsumer {

	public void consume(IValueProducer producer);
	
	public IExpression getExpression(DeclarationType description);
	
	public IIntermediateNode getIntermediateNode();

	IValueProducer getProducer();

}