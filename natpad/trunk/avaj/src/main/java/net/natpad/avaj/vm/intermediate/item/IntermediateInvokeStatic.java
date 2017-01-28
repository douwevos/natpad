package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.Argument;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.classfile.ClassfileMethodHeader;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeInvokeStatic;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateInvokeStatic extends AbstractIntermediateItem {


	public final DeclarationType classNameDeclDesc;
	public final ClassfileMethodHeader parsedMethodHeader;
	
	public IntermediateInvokeStatic(int index, DeclarationType classNameDeclDesc, ClassfileMethodHeader parsedMethodHeader) {
		super(index);
		this.classNameDeclDesc = classNameDeclDesc;
		this.parsedMethodHeader = parsedMethodHeader;
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateInvokeStatic(mnemonicIndex, classNameDeclDesc, parsedMethodHeader);
	}
	
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		Argument[] args = parsedMethodHeader.getArgs();

		final SubCodeInvokeStatic invokeSpecial = new SubCodeInvokeStatic(mnemonicIndex, classNameDeclDesc, parsedMethodHeader, args.length);
		
		for(int idx=args.length-1; idx>=0; idx--) {
			Consumer argConsumer = new Consumer(this, "InvokeSpecial.arg["+idx+"]");
			invokeSpecial.addConsumer(argConsumer);
		}
		
		
		if (!parsedMethodHeader.returnType.isVoid()) {
			DefaultValueProducer valueProducer = new DefaultValueProducer(this);
			valueProducer.typeSet.add(new ValueTypeDefinition(parsedMethodHeader.returnType, ValueTypeDefinitionKind.RETURN));
			invokeSpecial.setValueProducer(valueProducer);
		}

		
		modelRuntime.addPostImporter(invokeSpecial);
		
		modelRuntime.addAndRunSubCode(invokeSpecial);
	}
	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateInvokeStatic) {
			IntermediateInvokeStatic that = (IntermediateInvokeStatic) otherNode;
			return that.parsedMethodHeader.equals(parsedMethodHeader) && classNameDeclDesc.equals(that.classNameDeclDesc);
		}
		return false;
	}
	
	@Override
	public String toString() {
		String result = TextUtil.getName(getClass());
		if (scope!=null) {
			result+="["+classNameDeclDesc+", parsedHeader="+parsedMethodHeader+"]";
		}
		return result;
	}
	
	
}
