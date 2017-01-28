package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.Assignment;
import net.natpad.avaj.ast.DeclarationStatement;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.intermediate.IPostImport;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.IConsumer;
import net.natpad.avaj.vm.type.ImportRepository;

public class SubCodeFramePut extends AbstractSubCode implements IPostImport {

	public final boolean isDeclarator;
	
	public final MethodFrameValue frameValue;
	
	
	
	public SubCodeFramePut(int index, MethodFrameValue frameValue, boolean isDeclarator) {
		super(index);
		this.frameValue = frameValue;
		this.isDeclarator = isDeclarator;
	}
	
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		IExpression lvalue = frameValue.getExpression(null);
		DeclarationType bestType = frameValue.getBestType();
		IConsumer consumer = consumerList.get(0);
		log.debug("bestType="+bestType+", consumer="+consumer);
		IExpression rvalue = consumer.getExpression(bestType);
		if (rvalue.equals(new Identifier("/* throw-expression */"))) {
			return;
		}
		Assignment assignment = new Assignment(lvalue, rvalue);
		if (isDeclarator) {
			DeclarationStatement declStat = null;
			declStat = new DeclarationStatement(modelRuntime.importRepository.createFinalDeclarationType(bestType), assignment);
			modelRuntime.appendStatement(declStat);
		} else {
			modelRuntime.appendStatement(assignment);
		}
	}
	
	@Override
	public void addImport(ImportRepository importRepository) {
		DeclarationType bestType = frameValue.getBestType();
		log.debug("frameValue="+frameValue);
		importRepository.attach(bestType);
		
	}
	
	public String toString() {
		return TextUtil.getName(getClass())+"["+hashCode()+"]";
	}
	
}
