package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.Argument;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.classfile.ClassfileMethodHeader;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.DefaultValueProducer;
import net.natpad.avaj.vm.subcode.impl.SubCodeInvokeVirtual;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateInvokeVirtual extends AbstractIntermediateItem {


	public final ClassfileMethodHeader parsedMethodHeader;
	
	public IntermediateInvokeVirtual(int index, ClassfileMethodHeader parsedMethodHeader) {
		super(index);
		this.parsedMethodHeader = parsedMethodHeader;
		log.detail("parsedMethodHeader="+parsedMethodHeader);
	}
	
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateInvokeVirtual(mnemonicIndex, parsedMethodHeader);
	}
	
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		Argument[] args = parsedMethodHeader.getArgs();

		final SubCodeInvokeVirtual invokeSpecial = new SubCodeInvokeVirtual(mnemonicIndex, parsedMethodHeader, args.length);
		
		for(int idx=args.length-1; idx>=0; idx--) {
			Consumer argConsumer = new Consumer(this);
			invokeSpecial.addConsumer(argConsumer);
			modelRuntime.pushValueConsumer(argConsumer);
		}

		
		Consumer instanceConsumer = new Consumer(this);
		invokeSpecial.addConsumer(instanceConsumer);
		modelRuntime.pushValueConsumer(instanceConsumer);
		
		DefaultValueProducer valueProducer = null;
//		if (parsedMethodHeader.isConstructor) {
//			valueProducer = new DefaultValueProducer(this);
//			valueProducer.typeSet.add(new ValueTypeDefinition(methodClassDescriptor, ValueTypeDefinitionKind.NEW));
//		} else {
			boolean isVoid = false;
			DeclarationType returnType = parsedMethodHeader.returnType;
			if (returnType!=null) {
				isVoid = returnType.isVoid();
			}
			if (!isVoid) {
				valueProducer = new DefaultValueProducer(this);
				valueProducer.typeSet.add(new ValueTypeDefinition(returnType, ValueTypeDefinitionKind.RETURN));
			}
//		}

		
		if (valueProducer!=null) {
			invokeSpecial.setValueProducer(valueProducer);
			modelRuntime.pushValueProvider(valueProducer);
		}
		
		modelRuntime.add(invokeSpecial);
	}
	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateInvokeVirtual) {
			IntermediateInvokeVirtual that = (IntermediateInvokeVirtual) otherNode;
			return (parsedMethodHeader.equals(that.parsedMethodHeader));
		}
		return false;
	}
	
}
