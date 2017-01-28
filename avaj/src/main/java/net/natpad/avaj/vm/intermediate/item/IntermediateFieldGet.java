package net.natpad.avaj.vm.intermediate.item;


import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.util.Util;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeFieldGet;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateFieldGet extends AbstractIntermediateItem {

	public final DeclarationType fieldSource;
	public final DeclarationType fieldDeclType;
	public final Identifier fieldName;
	public final boolean isNonStatic;

	
	
	public IntermediateFieldGet(int index, DeclarationType fieldDeclType, DeclarationType fieldSource, Identifier fieldName, boolean isNonStatic) {
		super(index);
		this.fieldSource = fieldSource;
		this.fieldDeclType = fieldDeclType;
		this.fieldName = fieldName;
		this.isNonStatic = isNonStatic;
	}

	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateFieldGet(mnemonicIndex, fieldDeclType, fieldSource, fieldName, isNonStatic);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		SubCodeFieldGet subCodeFieldGet = new SubCodeFieldGet(mnemonicIndex, fieldSource, fieldName, isNonStatic);
		if (isNonStatic) {
			Consumer sourceConsumer = new Consumer(this);
			subCodeFieldGet.addConsumer(sourceConsumer);
		}
		
		DefaultValueProducer valueProducer = new DefaultValueProducer(this);
		valueProducer.getTypeSet().add(new ValueTypeDefinition(fieldDeclType, ValueTypeDefinitionKind.CONSTANT));
		subCodeFieldGet.setValueProducer(valueProducer);
		
		modelRuntime.addAndRunSubCode(subCodeFieldGet);
	}
	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateFieldGet) {
			IntermediateFieldGet that = (IntermediateFieldGet) otherNode;
			return Util.nullSafeEquals(fieldDeclType, that.fieldDeclType) && 
					Util.nullSafeEquals(fieldSource, that.fieldSource) &&
					Util.nullSafeEquals(fieldName, that.fieldName) &&
					isNonStatic==that.isNonStatic;
		}
		return false;
	}

	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"["+fieldName+", src="+fieldSource+", fieldDeclType="+fieldDeclType+", is-non-static="+isNonStatic+"]";
	}
	
}
