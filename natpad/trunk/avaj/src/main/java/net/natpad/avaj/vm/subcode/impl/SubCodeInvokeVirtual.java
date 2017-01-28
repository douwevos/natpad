package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.ExpressionStatement;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.ast.InvocationExpression;
import net.natpad.avaj.classfile.ClassfileMethodHeader;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeInvokeVirtual extends AbstractSubCode {

	public final ClassfileMethodHeader parsedMethodHeader;
	
	public final int argsCount;
	
	public SubCodeInvokeVirtual(int index, ClassfileMethodHeader parsedMethodHeader, int argsCount) {
		super(index);
		this.argsCount = argsCount;
		this.parsedMethodHeader = parsedMethodHeader;

	}
	
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		IExpression args[] = new IExpression[argsCount];
		for(int idx=0; idx<args.length; idx++) {
			args[idx] = consumerList.get(argsCount-1-idx).getExpression(null);
		}

		
		IExpression outputExpression = null;

		DeclarationType classNameDeclDesc = null;
		outputExpression = new InvocationExpression(consumerList.get(argsCount).getExpression(classNameDeclDesc), new Identifier(parsedMethodHeader.methodName), args);

		boolean isConsumed = false;
		if (valueProducer!=null) {
			valueProducer.setExpression(outputExpression);
			isConsumed = valueProducer.isConsumed();
		} 
		
		if (!isConsumed) {
			modelRuntime.appendStatement(new ExpressionStatement(outputExpression));
		}
	}



	@Override
	public String toString() {
		return subCodeName()+"["+parsedMethodHeader.methodName+", "+valueProducer+", args:"+argsCount+"]";
	}	
}
