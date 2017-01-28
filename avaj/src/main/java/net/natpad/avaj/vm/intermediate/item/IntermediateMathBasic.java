package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.classfile.bytecode.Operator;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeMathBasic;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

public class IntermediateMathBasic extends AbstractIntermediateItem {

	public final Operator operator;
	
	
	// TODO operator unary_minus is a non-infix operator
	public IntermediateMathBasic(int mnemonicIndex, Operator operator) {
		super(mnemonicIndex);
		this.operator = operator;
	}
	
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateMathBasic(mnemonicIndex, operator);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext groupConnectRuntime) {
		SubCodeMathBasic subCodeMathBasic = new SubCodeMathBasic(mnemonicIndex, operator);
		Consumer leftConsumer = new Consumer(this);
		Consumer rightConsumer = new Consumer(this);

		
		subCodeMathBasic.addConsumer(rightConsumer);
		groupConnectRuntime.pushValueConsumer(rightConsumer);
		subCodeMathBasic.addConsumer(leftConsumer);
		groupConnectRuntime.pushValueConsumer(leftConsumer);
		
		
		ValueTypeDefinitionList rightTypeSet = rightConsumer.getTypeSet();
		ValueTypeDefinitionList leftTypeSet = leftConsumer.getTypeSet();
		
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		subCodeMathBasic.setValueProducer(valueProducer);
		
		// next line needs to be called before the provider is pushed
		createSafeTypeSet(leftTypeSet, rightTypeSet, valueProducer.getTypeSet());
		
		groupConnectRuntime.pushValueProvider(valueProducer);
		groupConnectRuntime.add(subCodeMathBasic);
	}
	
	private void createSafeTypeSet(ValueTypeDefinitionList leftTypeSet, ValueTypeDefinitionList rightTypeSet, ValueTypeDefinitionList resulTypeSet) {
		if (leftTypeSet.size()==0) {
			if (rightTypeSet.size()==0) {
				/* both typesets are empty we do nothing */
				return;
			}
			for(ValueTypeDefinition valTypeDef : rightTypeSet) {
				if (valTypeDef.isIncluded) {
					resulTypeSet.add(valTypeDef);
				}
			}
		} else if (rightTypeSet.size()==0) {
			for(ValueTypeDefinition valTypeDef : leftTypeSet) {
				if (valTypeDef.isIncluded) {
					resulTypeSet.add(valTypeDef);
				}
			}
		} else {
			for(ValueTypeDefinition valTypeDef : leftTypeSet) {
				if (valTypeDef.isIncluded) {
					resulTypeSet.add(valTypeDef);
				}
			}
			
		}
	}

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateMathBasic) {
			IntermediateMathBasic that = (IntermediateMathBasic) otherNode;
			return that.operator==operator;
		}
		return false;
	}

	
	

	@Override
	public String toString() {
		String result = TextUtil.getName(getClass());
		if (scope!=null) {
			result+="[operator="+operator+"]";
		}
		return result;
	}	
	
}
