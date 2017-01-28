package net.natpad.avaj.vm.subcode.block;

import net.natpad.avaj.ast.Block;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.ForLoopStatement;
import net.natpad.avaj.ast.IAstNode;
import net.natpad.avaj.ast.IConditionalExpression;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.IStatement;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.IConsumer;
import net.natpad.avaj.vm.subcode.ISubCode;
import net.natpad.avaj.vm.subcode.IValueProducer;

public class SubCodeForLoop implements ISubCode {

	public final ISubCode initBlock;
	public final ISubCode conditionBlock;
	public final Consumer conditionConsumer;
	public final ISubCode forBlock;
	public final ISubCode bodyBlock;

	public SubCodeForLoop(ISubCode initBlock, ISubCode conditionBlock, Consumer conditionConsumer, ISubCode forBlock, ISubCode bodyBlock) {
		this.initBlock = initBlock;
		this.conditionBlock = conditionBlock;
		this.conditionConsumer = conditionConsumer;
		this.forBlock = forBlock;
		this.bodyBlock = bodyBlock;
	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		Block initStatementBlock = new Block();
		ModelStageTwoRuntime branchRuntime = modelRuntime.createBranch(initStatementBlock);
		initBlock.connectStageTwo(branchRuntime);

		
		IStatement iStatement = initStatementBlock.get(0);
		IAstNode forInit = null;
		if (iStatement instanceof IAstNode) {
			forInit = (IAstNode) iStatement;
		}
		
		
		Block astConditionBlock = new Block();
		branchRuntime = modelRuntime.createBranch(astConditionBlock);
		conditionBlock.connectStageTwo(branchRuntime);
		
		System.out.println("zzzzzzzzzzzzzzzzz");
		for(IStatement stat : astConditionBlock) {
			System.out.println("::: "+stat);
		}
		IExpression expression = conditionConsumer.getExpression(null);
		System.out.println("exp="+expression);
		
		IConditionalExpression condExpression = (IConditionalExpression) conditionConsumer.getExpression(DeclarationType.DEFAULT_BOOLEAN);

		
		Block astIncrementBlock = new Block();
		branchRuntime = modelRuntime.createBranch(astIncrementBlock);
		forBlock.connectStageTwo(branchRuntime);
		IStatement iStatement2 = astIncrementBlock.get(0);
		System.out.println("::: "+iStatement2);
		IAstNode forUpdate = null;
		if (iStatement2 instanceof IAstNode) {
			forUpdate = (IAstNode) iStatement2;
		}
		
		
		Block astBodyBlock = new Block();
		branchRuntime = modelRuntime.createBranch(astBodyBlock);
		bodyBlock.connectStageTwo(branchRuntime);
		
		ForLoopStatement forLoopStatement = new ForLoopStatement(forInit, condExpression.invert(), forUpdate, astBodyBlock);
		modelRuntime.appendStatement(forLoopStatement);
//		System.exit(-1);
	}
	
	
	@Override
	public IConsumer getInputConsumerAt(int index) {
		return null;
	}
	
	
	@Override
	public IValueProducer getOutputProducer() {
		return null;
	}
}
