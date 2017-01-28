package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IConditionalExpression;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.WrappedConditionalExpression;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.IConsumer;

public class ConditionalItemConsumer extends Consumer {

	static Logger log = Logger.getInstance(IConsumer.class);
	
	
	public final boolean invert;
	
	public ConditionalItemConsumer(IIntermediateNode node, boolean invert) {
		super(node);
		this.invert = invert;
	}
	
	public IExpression getExpression(DeclarationType requested) {
		IExpression result = super.getExpression(DeclarationType.DEFAULT_BOOLEAN);
		if (result==null) {
			log.error("no boolean expression for consumer yet");
			return result;
		}
		if (invert) {
			log.debug("inverting:"+result);
			IConditionalExpression conditionExpression = WrappedConditionalExpression.ensureIsConditional(result);
			result = conditionExpression.invert();
		}
		return result;
	}
	

	
}