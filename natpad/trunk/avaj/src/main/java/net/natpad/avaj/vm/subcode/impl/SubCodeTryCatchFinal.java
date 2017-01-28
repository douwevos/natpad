package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.Block;
import net.natpad.avaj.ast.CatchBlock;
import net.natpad.avaj.ast.TryCatchFinalStatement;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.IConsumer;
import net.natpad.avaj.vm.subcode.ISubCode;
import net.natpad.avaj.vm.subcode.IValueProducer;
import net.natpad.avaj.vm.subcode.block.SubCodeBlock;

public class SubCodeTryCatchFinal implements ISubCode {

	
	SubCodeBlock trySubCodeBlock;
	SubCodeFinally finalSubCodeBlock;
	SubCodeCatch catchSubCodes[];
	
	public SubCodeTryCatchFinal(SubCodeBlock trySubCodeBlock, SubCodeFinally finalSubCodeBlock, SubCodeCatch ... catchSubCodes) {
		this.trySubCodeBlock = trySubCodeBlock;
		this.finalSubCodeBlock = finalSubCodeBlock;
		this.catchSubCodes = catchSubCodes;
	}




	@Override
	public void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		Block tryBlock = new Block();
		CatchBlock catchBlocks[] = new CatchBlock[0];
		Block finalBlock = null;
		
		
		ModelStageTwoRuntime tryRuntime = modelRuntime.createBranch(tryBlock);
		trySubCodeBlock.connectStageTwo(tryRuntime);

	
		if (catchSubCodes!=null && catchSubCodes.length>0) {
			catchBlocks = new CatchBlock[catchSubCodes.length];
			int idx = 0;
			for(SubCodeCatch subCodeCatch : catchSubCodes) {
				subCodeCatch.connectStageTwo(modelRuntime);
				catchBlocks[idx] = subCodeCatch.catchBlock;
				idx++;
			}
		}

		if (finalSubCodeBlock!=null) {
			finalSubCodeBlock.connectStageTwo(modelRuntime);
			finalBlock = finalSubCodeBlock.finallyBlock;
		}

		
		TryCatchFinalStatement statement = new TryCatchFinalStatement(tryBlock, finalBlock, catchBlocks);
		modelRuntime.appendStatement(statement);
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
