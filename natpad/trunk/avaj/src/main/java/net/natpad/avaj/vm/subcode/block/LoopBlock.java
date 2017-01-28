package net.natpad.avaj.vm.subcode.block;

import net.natpad.avaj.ast.Block;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IConditionalExpression;
import net.natpad.avaj.ast.WhileStatement;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.IConsumer;
import net.natpad.avaj.vm.subcode.ISubCode;
import net.natpad.avaj.vm.subcode.IValueProducer;

public class LoopBlock implements ISubCode {

	public final Consumer condition_consumer;
	
	ISubCode subCodes[];
	
	public LoopBlock(Consumer consumer, ISubCode ... codes) {
		this.condition_consumer = consumer;
		subCodes = codes;
	}
	
	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		Block blockCode = new Block();
		WhileStatement whileStat = new WhileStatement(blockCode);
		modelRuntime.appendStatement(whileStat);
		ModelStageTwoRuntime branchRuntime = modelRuntime.createBranch(blockCode);
		for(ISubCode subCode : subCodes) {
			subCode.connectStageTwo(branchRuntime);
		}
		IConditionalExpression condExpression = (IConditionalExpression) condition_consumer.getExpression(DeclarationType.DEFAULT_BOOLEAN);
		whileStat.setCondintionalExpression(condExpression.invert());
	}
	
	
	@Override
	public IConsumer getInputConsumerAt(int index) {
		if (index==0) {
			return condition_consumer;
		}
		return null;
	}
	
	
	@Override
	public IValueProducer getOutputProducer() {
		return null;
	}
	
}
