package net.natpad.avaj.vm.subcode.block;

import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.SynchronizedBlock;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.ISubCode;

public class SubCodeSynchronizedBlock extends AbstractSubCode {

	ISubCode subCodeBody;
	Consumer consumer;
	

	public SubCodeSynchronizedBlock(int mnemonic_index, ISubCode subCodeBody, Consumer consumer) {
		super(mnemonic_index);
		this.consumer = consumer;
		this.subCodeBody = subCodeBody;
	}


	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		IExpression syncExpression = consumer.getExpression(null);
		SynchronizedBlock blockCode = new SynchronizedBlock(syncExpression);
		modelRuntime.appendStatement(blockCode);
		
		ModelStageTwoRuntime bodyRuntime = modelRuntime.createBranch(blockCode);
		subCodeBody.connectStageTwo(bodyRuntime);
	}

	
}
