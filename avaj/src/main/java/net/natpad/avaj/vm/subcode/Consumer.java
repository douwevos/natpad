package net.natpad.avaj.vm.subcode;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;


public class Consumer implements IConsumer {

	static Logger log = Logger.getInstance(IConsumer.class);

	
	public final IIntermediateNode node;
	private IValueProducer producer;
	String description;

	Exception ownerTrace;
	
	public Consumer(IIntermediateNode node, String description) {
		ownerTrace = new Exception("CONSUMER !!!");
		this.node = node;
		if (description==null) {
			description = TextUtil.getName(node.getClass(), "Intermediate");
		}
		this.description = description;
	}

	public Consumer(IIntermediateNode node) {
		this(node,null);
	}

	
	public IIntermediateNode getIntermediateNode() {
		return node;
	}
	
	
	public IExpression getExpression(DeclarationType description) {
		if (description==null) {
			description = getTypeSet().getBestType();
		}
//		return producer==null ? null : producer.getExpression();
		
		IExpression result = producer.getExpression(description);
//		log.trace("gettt:::"+description+", result="+result);
		return result;
	}
	
	public ValueTypeDefinitionList getTypeSet() {
		if (producer==null) {
			log.error("NO PRODUCER !!!!");
			log.exception(ownerTrace);
		}
		
		return producer.getTypeSet();
	}
	

	public boolean isConsumed() {
		return producer!=null;
	}
	
	@Override
	public void consume(IValueProducer producer) {
		this.producer = producer;
	}

	public IValueProducer getProducer() {
		return producer;
	}
	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"["+hashCode()+", "+description+", "+(producer==null ? "no-producer-yet" : ""+producer.hashCode())+"]";
	}
	
}
