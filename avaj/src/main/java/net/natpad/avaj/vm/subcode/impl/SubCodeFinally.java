package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.FinallyBlock;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.ISubCode;

public class SubCodeFinally extends AbstractSubCode {

	public final ISubCode block;
	FinallyBlock finallyBlock;

	
	
	public SubCodeFinally(int mnemonicIndex, ISubCode block) {
		super(mnemonicIndex);
		this.block = block;
	}
	
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		finallyBlock = new FinallyBlock();
		ModelStageTwoRuntime branchedRuntime = modelRuntime.createBranch(finallyBlock);
		block.connectStageTwo(branchedRuntime);
	}


	
	
}
