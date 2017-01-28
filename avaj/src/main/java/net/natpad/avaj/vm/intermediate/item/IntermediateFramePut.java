package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.classfile.attribute.LocalVariableTableEntry;
import net.natpad.avaj.classfile.bytecode.IMnemonic;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.impl.MethodFrameValue;
import net.natpad.avaj.vm.subcode.impl.SubCodeFramePut;
import net.natpad.avaj.vm.type.CrossRuleLeftValue;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

public class IntermediateFramePut extends AbstractIntermediateItem {

	public final int frameIndex;
	
	public IntermediateFramePut(int index, int frameIndex) {
		super(index);
		this.frameIndex = frameIndex;
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateFramePut(mnemonicIndex, frameIndex);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		boolean isDeclarator = false;
		MethodFrameValue frameValue = modelRuntime.getMethodFrameValue(frameIndex);
		if (frameValue==null) {
			frameValue = modelRuntime.createMethodFrameValue(frameIndex);
			isDeclarator = true;
		}
		
		if (modelRuntime.localVariableTable!=null) {
			
			IMnemonic mnemonic = modelRuntime.mnemonicBlock.get(mnemonicIndex);
			LocalVariableTableEntry variableTableEntry = modelRuntime.localVariableTable.findEntry(frameIndex, mnemonic.getOffset()+mnemonic.getLength());
			if (variableTableEntry!=null) {
				frameValue.setName(variableTableEntry.nameText);
				frameValue.add(new ValueTypeDefinition(variableTableEntry.declarationType, ValueTypeDefinitionKind.LOCALVALUETABLE));
			}
		}
		
		SubCodeFramePut subCode = new SubCodeFramePut(mnemonicIndex, frameValue, isDeclarator);
		if (isDeclarator) {
			modelRuntime.addPostImporter(subCode);
		}
		
		Consumer consumer = new Consumer(this);
		subCode.addConsumer(consumer);
//		modelRuntime.addCrossTypeMatcher("assignment", subCode.consumer.getTypeSet(), frameValue);
		
		modelRuntime.addAndRunSubCode(subCode);
		
		
		ValueTypeDefinitionList leftHandSide = frameValue;
		ValueTypeDefinitionList rightHandSide = consumer.getTypeSet();
		modelRuntime.addValueTypeDefinitionList(leftHandSide);
		modelRuntime.addValueTypeDefinitionList(rightHandSide);
		CrossRuleLeftValue crossRule = new CrossRuleLeftValue(leftHandSide, rightHandSide);
		leftHandSide.addCrossRule(crossRule);
//		rightHandSide.addCrossRule(crossRule);
		
		
	}
	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateFramePut) {
			IntermediateFramePut that = (IntermediateFramePut) otherNode;
			return that.frameIndex==frameIndex;
		}
		return false;
	}
	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"[field="+frameIndex+"]";
	}
}