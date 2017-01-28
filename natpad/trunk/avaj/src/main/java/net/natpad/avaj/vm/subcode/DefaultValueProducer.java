package net.natpad.avaj.vm.subcode;

import java.util.ArrayList;
import java.util.List;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.ast.Literal;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.subcode.impl.ExpressionHolder;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

public class DefaultValueProducer implements IValueProducer {

	private ArrayList<IConsumer> inputConsumers = new ArrayList<IConsumer>();
	private IConsumer consumer = null;
	public ValueTypeDefinitionList typeSet;
	protected ExpressionHolder expressionHolder;
	protected final IIntermediateNode owner;
	
	
	public DefaultValueProducer(IIntermediateNode owner) {
		expressionHolder = new ExpressionHolder();
		typeSet = new ValueTypeDefinitionList();
		this.owner = owner;
	}
	
	
	public DefaultValueProducer(IIntermediateNode owner, ValueTypeDefinitionList typeSet, ExpressionHolder expressionHolder) {
		this.typeSet = typeSet;
		this.expressionHolder = expressionHolder;
		this.owner = owner;
	}
	
	@Override
	public IValueProducer duplicate() {
		return new DefaultValueProducer(owner, typeSet, expressionHolder);
	}
	
	@Override
	public IIntermediateNode getIntermediateNode() {
		return owner;
	}
	
	@Override
	public IExpression getExpression(DeclarationType requested) {
		IExpression expression = expressionHolder.getExpression();
		if (expression instanceof Literal) {
			Literal lit = (Literal) expression;
			if (DeclarationType.DEFAULT_BOOLEAN.equals(requested)) {
				if (lit.value instanceof Integer) {
					int liti = ((Integer) lit.value).intValue();
					if (liti==0 || liti==1) {
						return new Literal(new Boolean(liti==1));
					}
				}
			} else if (DeclarationType.DEFAULT_CHAR.equals(requested)) {
				if (lit.value instanceof Integer) {
					int liti = ((Integer) lit.value).intValue();
					String t = "";
					switch(liti) {
						case '\t' : {
							t = "\\t";
						} break;
						case '\r' : {
							t = "\\r";
						} break;
						case '\n' : {
							t = "\\n";
						} break;
						default : {
							t = t+(char) liti;
						}
					}
					
					return new Identifier("'"+t+"'");
				}
			}
		}
		return expression;		
	}

	@Override
	public void setExpression(IExpression expression) {
		expressionHolder.setExpression(expression);
	}
	
	@Override
	public ValueTypeDefinitionList getTypeSet() {
		return typeSet;
	}
	
	
	@Override
	public boolean isConsumed() {
		return consumer!=null;
	}
	
	@Override
	public IConsumer getConsumer() {
		return consumer;
	}
	
	@Override
	public void setConsumer(IConsumer consumer) {
		this.consumer = consumer;
	}
	
	
	@Override
	public void addInputConsumer(IConsumer inputConsumer) {
		inputConsumers.add(inputConsumer);
	}
	
	
	public List<IConsumer> getInputConsumers() {
		return inputConsumers;
	}
	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"["+hashCode()+", expression="+getExpression(null)+", typeset="+getTypeSet()+"]";
	}
	
}
