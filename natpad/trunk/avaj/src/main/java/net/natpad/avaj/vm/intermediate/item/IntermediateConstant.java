package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.util.Util;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeConstantLiteral;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

public class IntermediateConstant extends AbstractIntermediateItem {
	
	public final IExpression defaultExpression;
	private final ValueTypeDefinitionList typeSet;
	
	public IntermediateConstant(int mnemonicIndex, ValueTypeDefinitionList typeSet, IExpression defaultExpression) {
		super(mnemonicIndex);
		this.typeSet = typeSet;
		this.defaultExpression = defaultExpression;
		log.debug("typeset.size="+typeSet.size());
	}
	
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateConstant(mnemonicIndex, typeSet, defaultExpression);
	}

	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		SubCodeConstantLiteral subCode = new SubCodeConstantLiteral(mnemonicIndex, defaultExpression);
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		valueProducer.setExpression(defaultExpression);
		typeSet.copyTo(valueProducer.getTypeSet());
		subCode.setValueProducer(valueProducer);
		modelRuntime.addAndRunSubCode(subCode);		
	}
	
	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateConstant) {
			IntermediateConstant that = (IntermediateConstant) otherNode;
			boolean expressionEquals = Util.nullSafeEquals(defaultExpression, that.defaultExpression);
			boolean typeSetEequals = Util.nullSafeEquals(typeSet, that.typeSet);
			log.detail("expressionEquals="+expressionEquals);
			log.detail("typeSetEequals="+typeSetEequals);
			return typeSetEequals && expressionEquals;
		}
		return false;
	}
	

	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"[expr="+defaultExpression+", typed-set="+typeSet+"]";
	}
}
