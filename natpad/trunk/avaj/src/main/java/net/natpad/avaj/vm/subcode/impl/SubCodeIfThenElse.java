package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.Block;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.IfThenElse;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.ISubCode;

public class SubCodeIfThenElse extends AbstractSubCode {

	
	public final ISubCode thenModel;
	public final ISubCode elseModel;
	
	

	public SubCodeIfThenElse(ISubCode thenModel) {
		super(-1);
		this.thenModel = thenModel;
		this.elseModel = null;
	}

	
	public SubCodeIfThenElse(ISubCode thenModel, ISubCode elseModel) {
		super(-1);
		this.thenModel = thenModel;
		this.elseModel = elseModel;
	}
	

	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {

		Block blockThen = new Block();
		Block blockElse = null;
		
		IExpression condExpression = valueProducer.getExpression(null);
		
		IfThenElse ifThenElse = null;
		if (elseModel!=null) {
			blockElse = new Block();
			ifThenElse = new IfThenElse(condExpression, blockThen, blockElse);
		} else {
			ifThenElse = new IfThenElse(condExpression, blockThen);
		}
		modelRuntime.appendStatement(ifThenElse);
		ModelStageTwoRuntime branchRuntime = modelRuntime.createBranch(blockThen);
		thenModel.connectStageTwo(branchRuntime);
		
		if (elseModel!=null) {
			branchRuntime = modelRuntime.createBranch(blockElse);
			elseModel.connectStageTwo(branchRuntime);
		}
	}
	
}
