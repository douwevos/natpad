package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.PostOrPrefixType;
import net.natpad.avaj.ast.PostfixExpression;
import net.natpad.avaj.ast.PrefixExpression;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.IValueProducer;

public class FrameValueProducer extends DefaultValueProducer {

	public enum Opperation { 
		PLAIN, POSTFIXINCREMENT, PREFIXINCREMENT, POSTFIXDECREMENT, PREFIXDECREMENT;
	};


	public FrameValueProducer.Opperation opperation = Opperation.PLAIN;

	
	public FrameValueProducer(IIntermediateNode owner, MethodFrameValue methodFrameValue) {
		super(owner, methodFrameValue, new ExpressionHolder());
	}

	private FrameValueProducer(IIntermediateNode owner, MethodFrameValue methodFrameValue, ExpressionHolder expressionHolder) {
		super(owner, methodFrameValue, expressionHolder);
	}

	
	public int getFrameIndex() {
		MethodFrameValue methodFrameValue = (MethodFrameValue) getTypeSet();
		return methodFrameValue.frameIndex;
	}

	@Override
	public IValueProducer duplicate() {
		/* TODO: is this correct */
		return new FrameValueProducer(owner, (MethodFrameValue) getTypeSet(), expressionHolder);
	}

	public IExpression getExpression(DeclarationType typeDescr) {
		if (expressionHolder.getExpression()!=null) {
			return expressionHolder.getExpression();
		}
		MethodFrameValue methodFrameValue = (MethodFrameValue) getTypeSet();
		switch(opperation) {
			case PLAIN : {
				return methodFrameValue.getExpression(typeDescr);
			}
			case POSTFIXINCREMENT : {
				return new PostfixExpression(PostOrPrefixType.INCREMENT, methodFrameValue.getExpression(typeDescr));
			}
			case POSTFIXDECREMENT : {
				return new PostfixExpression(PostOrPrefixType.DECREASE, methodFrameValue.getExpression(typeDescr));
			}
			case PREFIXINCREMENT : {
				return new PrefixExpression(PostOrPrefixType.INCREMENT, methodFrameValue.getExpression(typeDescr));
			}
			case PREFIXDECREMENT : {
				return new PrefixExpression(PostOrPrefixType.DECREASE, methodFrameValue.getExpression(typeDescr));
			}
		}
		return methodFrameValue.getExpression(typeDescr);
	}
	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"["+getExpression(null)+"]";
	}

}