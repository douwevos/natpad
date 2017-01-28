package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.ExpressionStatement;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.ast.InvocationExpression;
import net.natpad.avaj.ast.Literal;
import net.natpad.avaj.classfile.ClassfileMethodHeader;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.intermediate.IPostImport;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.type.ImportRepository;

public class SubCodeInvokeStatic extends AbstractSubCode implements IPostImport {

	
	public final DeclarationType classNameDeclDesc;
	public final ClassfileMethodHeader parsedMethodHeader;
	private final int argsCount;
	
	public SubCodeInvokeStatic(int index, DeclarationType classNameDeclDesc, ClassfileMethodHeader parsedMethod, int argsCount) {
		super(index);
		this.parsedMethodHeader = parsedMethod;
		this .argsCount = argsCount;
		this.classNameDeclDesc = classNameDeclDesc;
		
	}
	
	
	@Override
	public void addImport(ImportRepository importRepository) {
		importRepository.attach(classNameDeclDesc);
	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		IExpression args[] = new IExpression[argsCount];
		for(int idx=0; idx<args.length; idx++) {
			args[idx] = consumerList.get(argsCount-1-idx).getExpression(null);
		}
		Identifier fullInvocationName = modelRuntime.createPlainName(classNameDeclDesc, new Identifier(parsedMethodHeader.methodName));
//		Identifier fullInvocationName = importTypeClass.getStaticFieldReference();
		Literal methodExpression = new Literal(fullInvocationName.text);
		valueProducer.setExpression(new InvocationExpression(methodExpression, args));

		if (!valueProducer.isConsumed()) {
			modelRuntime.appendStatement(new ExpressionStatement(valueProducer.getExpression(null)));
		}
	}

	
}
