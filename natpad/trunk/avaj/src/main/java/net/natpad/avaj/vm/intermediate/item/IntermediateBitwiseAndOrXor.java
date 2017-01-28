package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.classfile.bytecode.Operator;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeBitwiseAndOrXor;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateBitwiseAndOrXor extends AbstractIntermediateItem {


	public final Operator operator;
	
	public IntermediateBitwiseAndOrXor(int index, Operator operator) {
		super(index);
		this.operator = operator;
	}
	
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateBitwiseAndOrXor(mnemonicIndex, operator);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		SubCodeBitwiseAndOrXor subCodeOr = new SubCodeBitwiseAndOrXor(mnemonicIndex, operator);
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		valueProducer.typeSet.add(new ValueTypeDefinition(DeclarationType.DEFAULT_BOOLEAN, ValueTypeDefinitionKind.CONDITION));
		valueProducer.typeSet.add(new ValueTypeDefinition(DeclarationType.DEFAULT_INT, ValueTypeDefinitionKind.CONDITION));
		valueProducer.typeSet.add(new ValueTypeDefinition(DeclarationType.DEFAULT_LONG, ValueTypeDefinitionKind.CONDITION));		
		

		subCodeOr.addConsumer(new Consumer(this));	// right consumer
		subCodeOr.addConsumer(new Consumer(this));	// left consumer
		subCodeOr.setValueProducer(valueProducer);
		
//		modelRuntime.addCrossTypeMatcher("or", subCodeOr.rightConsumer.getTypeSet(), subCodeOr.leftConsumer.getTypeSet());
		
		modelRuntime.addAndRunSubCode(subCodeOr);
	}

	

	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"["+operator+"]";
	}

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		return otherNode instanceof IntermediateBitwiseAndOrXor;
	}

	
}
