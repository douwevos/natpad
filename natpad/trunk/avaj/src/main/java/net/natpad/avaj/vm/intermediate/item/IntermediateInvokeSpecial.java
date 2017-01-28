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
import net.natpad.avaj.vm.subcode.impl.SubCodeInvokeSpecial;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateInvokeSpecial extends AbstractIntermediateItem {


	public final DeclarationType methodClassDescriptor;
	public final ClassfileMethodHeader parsedMethodHeader;
	
	public IntermediateInvokeSpecial(int index, DeclarationType descriptor, ClassfileMethodHeader parsedMethodHeader) {
		super(index);
		this.methodClassDescriptor = descriptor;
		this.parsedMethodHeader = parsedMethodHeader;
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateInvokeSpecial(mnemonicIndex, methodClassDescriptor, parsedMethodHeader);
	}
	
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		Argument[] args = parsedMethodHeader.getArgs();

		final SubCodeInvokeSpecial invokeSpecial = new SubCodeInvokeSpecial(mnemonicIndex, methodClassDescriptor, parsedMethodHeader, args.length);
		
		for(int idx=args.length-1; idx>=0; idx--) {
			Consumer argConsumer = new Consumer(this, "InvokeSpecial.arg["+idx+"]");
			invokeSpecial.addConsumer(argConsumer);
		}
		
		Consumer instanceConsumer = new Consumer(this, "InvokeSpecial.instance");
		invokeSpecial.addConsumer(instanceConsumer);
		
		if (parsedMethodHeader.isConstructor) {
			Consumer constructorConsumer = new Consumer(this, "InvokeSpecial.constructor");
			invokeSpecial.addConsumer(constructorConsumer);
		}
		
		DefaultValueProducer valueProducer = null;
		if (parsedMethodHeader.isConstructor) {
			valueProducer = new DefaultValueProducer(this);
			valueProducer.typeSet.add(new ValueTypeDefinition(methodClassDescriptor, ValueTypeDefinitionKind.NEW));
		} else {
			boolean isVoid = false;
			DeclarationType returnType = parsedMethodHeader.returnType;
			if (returnType!=null) {
				isVoid = returnType.isVoid();
			}
			if (!isVoid) {
				valueProducer = new DefaultValueProducer(this);
				valueProducer.typeSet.add(new ValueTypeDefinition(returnType, ValueTypeDefinitionKind.RETURN));
			}
		}


		// TODO: try to understand what the code below does
//		IValueProducer peekProducer = modelRuntime.peekProducer(0);
		/* the line below checks whether there is a consumer for the */
		
//		if (peekProducer!=null && peekProducer==invokeSpecial.instanceConsumer.getProducer()) {
//			modelRuntime.popProducer();
//			modelRuntime.pushValueProvider(outputValueProducer);
			// TODO
//			modelRuntime.addCrossTypeMatcher("invokeSpecial", invokeSpecial.instanceConsumer.getTypeSet(), invokeSpecial.getTypeSet());
//		}
//		
//		
		if (valueProducer!=null) {
			invokeSpecial.setValueProducer(valueProducer);
		}

		modelRuntime.addAndRunSubCode(invokeSpecial);
	}
	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateInvokeSpecial) {
			IntermediateInvokeSpecial that = (IntermediateInvokeSpecial) otherNode;
			return that.parsedMethodHeader.equals(parsedMethodHeader);
		}
		return false;
	}
	
	
	
	@Override
	public String toString() {
		String result = TextUtil.getName(getClass());
		if (scope!=null) {
			result+="["+methodClassDescriptor+", parsedHeader="+parsedMethodHeader+"]";
		}
		return result;
	}

}
