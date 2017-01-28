package net.natpad.avaj.vm.intermediate.item;

import java.util.List;

import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.InfixExpression;
import net.natpad.avaj.ast.Literal;
import net.natpad.avaj.classfile.attribute.LocalVariableTableEntry;
import net.natpad.avaj.classfile.bytecode.Operator;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.IValueProducer;
import net.natpad.avaj.vm.subcode.impl.FrameValueProducer;
import net.natpad.avaj.vm.subcode.impl.MethodFrameValue;
import net.natpad.avaj.vm.subcode.impl.SubCodeIncrement;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateIncrement extends AbstractIntermediateItem {

	
	public final int frameIndex, value;
	
	public IntermediateIncrement(int index, int frameIndex, int value) {
		super(index);
		this.frameIndex = frameIndex;
		this.value = value;
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateIncrement(mnemonicIndex, frameIndex, value);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		List<IValueProducer> producerStack = modelRuntime.producerStack;
		boolean commandConsumed = false;
		if (value==1 || value ==-1) {
			/* TODO: we can not just trust the stack, or can we ? */
			for(int idx=producerStack.size()-1; !commandConsumed && idx>=0; idx--) {
				IValueProducer producer = producerStack.get(idx);
				if (producer instanceof FrameValueProducer) {
					FrameValueProducer frameValueProducer = (FrameValueProducer) producer;
					if (frameValueProducer.getFrameIndex() == frameIndex) {
						if (value==1) {
							frameValueProducer.opperation = FrameValueProducer.Opperation.POSTFIXINCREMENT;
						} else {
							frameValueProducer.opperation = FrameValueProducer.Opperation.POSTFIXDECREMENT;
						}
						commandConsumed = true;
					}
				}
			}
		}
		
		if (!commandConsumed) {
			MethodFrameValue methodFrameValue = modelRuntime.createMethodFrameValue(frameIndex);
			if (modelRuntime.localVariableTable!=null) {
				LocalVariableTableEntry variableTableEntry = modelRuntime.localVariableTable.findEntry(frameIndex, mnemonicIndex);
				if (variableTableEntry!=null) {
					methodFrameValue.setName(variableTableEntry.nameText);
					methodFrameValue.add(new ValueTypeDefinition(variableTableEntry.declarationType, ValueTypeDefinitionKind.LOCALVALUETABLE));
				}
			}

			SubCodeIncrement subCodeIncrement = new SubCodeIncrement(mnemonicIndex, methodFrameValue, value);
			
			IValueProducer valueProducer = null;
			if (value==1) {
				FrameValueProducer frameValueProducer = new FrameValueProducer(this, methodFrameValue);
				frameValueProducer.opperation = FrameValueProducer.Opperation.PREFIXINCREMENT;
				valueProducer = frameValueProducer;
			} else if (value==-1) {
				FrameValueProducer frameValueProducer = new FrameValueProducer(this, methodFrameValue);
				frameValueProducer.opperation = FrameValueProducer.Opperation.PREFIXDECREMENT;
				valueProducer = frameValueProducer;
			} else {
				IExpression incrementExpression = new Literal(new Integer(value));
				InfixExpression expression = new InfixExpression(Operator.ADD, methodFrameValue.getExpression(null), incrementExpression);
				DefaultValueProducer defaultValueProducer = new DefaultValueProducer(this);
				defaultValueProducer.setExpression(expression);
				valueProducer = defaultValueProducer;
			}
			
			
			if (valueProducer!=null) {
				subCodeIncrement.setValueProducer(valueProducer);
				modelRuntime.pushPseudoProducer(valueProducer);
			}
			modelRuntime.add(subCodeIncrement);
		}
	}
	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateIncrement) {
			IntermediateIncrement that = (IntermediateIncrement) otherNode;
			return that.frameIndex==frameIndex && that.value==value;
		}
		return false;
	}
	
	
	

	@Override
	public String toString() {
		String result = TextUtil.getName(getClass());
		if (scope!=null) {
			result+="[frameIndex="+frameIndex+", value="+value+"]";
		}
		return result;
	}
}
