package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.util.Util;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.impl.SubCodeFieldPut;
import net.natpad.avaj.vm.type.CrossRuleLeftValue;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

public class IntermediateFieldPut extends AbstractIntermediateItem {

	public final DeclarationType fieldSource;
	public final DeclarationType fieldDeclType;
	public final Identifier fieldName;
	public final boolean isNonStatic;
	
	public IntermediateFieldPut(int index, DeclarationType fieldDeclType, DeclarationType fieldSource, Identifier fieldName, boolean isNonStatic) {
		super(index);
		this.fieldSource = fieldSource;
		this.fieldDeclType = fieldDeclType;
		this.fieldName = fieldName;
		this.isNonStatic = isNonStatic;
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateFieldPut(mnemonicIndex, fieldDeclType, fieldSource, fieldName, isNonStatic);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		SubCodeFieldPut subCode = new SubCodeFieldPut(mnemonicIndex, fieldDeclType, fieldSource, fieldName, isNonStatic);
		Consumer fieldConsumer = new Consumer(this);
		subCode.addConsumer(fieldConsumer);
		Consumer sourceConsumer = null;
		if (isNonStatic) {
			sourceConsumer = new Consumer(this);
			subCode.addConsumer(sourceConsumer);
		}
//		rightHandSide.addCrossRule(crossRule);
//		modelRuntime.addCrossTypeMatcher("put-field", subCode.consumer.getTypeSet(), typeSet);
		modelRuntime.addAndRunSubCode(subCode);

		if (isNonStatic) {
			ValueTypeDefinitionList leftHandSide = fieldConsumer.getTypeSet();
			ValueTypeDefinitionList rightHandSide = sourceConsumer.getTypeSet();
			modelRuntime.addValueTypeDefinitionList(leftHandSide);
			modelRuntime.addValueTypeDefinitionList(rightHandSide);
			CrossRuleLeftValue crossRule = new CrossRuleLeftValue(leftHandSide, rightHandSide);
			leftHandSide.addCrossRule(crossRule);
		}
	
	}
	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateFieldPut) {
			IntermediateFieldPut that = (IntermediateFieldPut) otherNode;
			
			return Util.nullSafeEquals(fieldDeclType, that.fieldDeclType) &&
					Util.nullSafeEquals(fieldSource, that.fieldSource) &&
					Util.nullSafeEquals(fieldName, that.fieldName);
		}
		return false;
	}
	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"[]";
	}
}