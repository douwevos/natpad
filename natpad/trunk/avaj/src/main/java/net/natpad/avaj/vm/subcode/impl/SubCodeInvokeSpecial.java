package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.ExpressionStatement;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.ast.InvocationExpression;
import net.natpad.avaj.classfile.ClassfileMethodHeader;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class SubCodeInvokeSpecial extends AbstractSubCode {

	public final DeclarationType classNameDeclDesc;
	public final ClassfileMethodHeader parsedMethodHeader;
	
	public final int argsCount;
	
	
	public SubCodeInvokeSpecial(int index, DeclarationType classNameDeclDesc, ClassfileMethodHeader parsedMethodHeader, int argsCount) {
		super(index);
		this.argsCount = argsCount;
		this.parsedMethodHeader = parsedMethodHeader;
		this.classNameDeclDesc = classNameDeclDesc;
		
	}
	
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		IExpression args[] = new IExpression[argsCount];
		for(int idx=0; idx<args.length; idx++) {
			args[idx] = consumerList.get(argsCount-1-idx).getExpression(null);
		}

		IExpression outputExpression = null;
//		IValueProducer producerFromInstanceConsumer = instanceConsumer.getProducer();
		
		boolean isConsumed = false;
//		log.debug("instanceExpression="+instanceConsumer.getExpression(null));
		if (parsedMethodHeader.isConstructor) {
			outputExpression = new InvocationExpression(consumerList.get(argsCount).getExpression(classNameDeclDesc), args);
//			producerFromInstanceConsumer.setExpression(outputExpression);
//			isConsumed = producerFromInstanceConsumer.isConsumed();
		} else {
			outputExpression = new InvocationExpression(consumerList.get(argsCount).getExpression(classNameDeclDesc), new Identifier(parsedMethodHeader.methodName), args);
		}
		
		if (valueProducer!=null) {
			valueProducer.setExpression(outputExpression);
			isConsumed = valueProducer.isConsumed();
		} 
		
		/* valueProducer neither exists or wasn't consumed, so we create an expression-statement */
		if (!isConsumed) {
			modelRuntime.appendStatement(new ExpressionStatement(outputExpression));
		}

	}

	
	@Override
	public String toString() {
		return subCodeName()+"["+classNameDeclDesc+", "+valueProducer+", args-count="+argsCount+"]";
	}

	
}
