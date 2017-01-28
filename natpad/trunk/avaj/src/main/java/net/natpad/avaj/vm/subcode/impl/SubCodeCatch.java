package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.CatchBlock;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.FieldDeclaration;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.ast.Modifiers;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.ISubCode;

public class SubCodeCatch extends AbstractSubCode {

	public final DeclarationType exceptionType;
	public final ISubCode block;
	
	CatchBlock catchBlock;
	
	public SubCodeCatch(int mnemonicIndex, DeclarationType exceptionType, ISubCode block) {
		super(mnemonicIndex);
		this.exceptionType = exceptionType;
		this.block = block;
	}
	
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		FieldDeclaration field = new FieldDeclaration(new Modifiers(), modelRuntime.importRepository.createFinalDeclarationType(exceptionType), new Identifier("exception"), null);
		catchBlock = new CatchBlock(field);
		ModelStageTwoRuntime branchedRuntime = modelRuntime.createBranch(catchBlock);
		block.connectStageTwo(branchedRuntime);
	}
		
}
