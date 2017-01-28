package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateNodeScope;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.block.SubCodeBlock;
import net.natpad.avaj.vm.subcode.impl.SubCodeIfThenElse;

public class IntermediateIfThenElse implements IIntermediateNode {

	public final IIntermediateNode condition;
	public final IIntermediateNode thenModel;
	public final IIntermediateNode elseModel;

	private final IntermediateNodeScope scope = new IntermediateNodeScope(-1,-1);
	
	public IntermediateIfThenElse(IIntermediateNode condition, IIntermediateNode thenModel) {
		super();
		this.condition = condition;
		this.thenModel = thenModel;
		this.elseModel = null;
	}

	
	public IntermediateIfThenElse(IIntermediateNode condition, IIntermediateNode thenModel, IIntermediateNode elseModel) {
		super();
		this.condition = condition;
		this.thenModel = thenModel;
		this.elseModel = elseModel;
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		if (deep) {
			return new IntermediateIfThenElse(condition.makeCopy(true), thenModel.makeCopy(true), elseModel.makeCopy(true));
		}
		return new IntermediateIfThenElse(condition, thenModel, elseModel);
	}

	@Override
	public IntermediateNodeScope getScope() {
		if (elseModel==null) {
			scope.setExitIndex(thenModel.getScope().getExitIndex());
		} else {
			scope.setExitIndex(elseModel.getScope().getExitIndex());
		}
		return scope;
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		condition.connectStageOne(modelRuntime);
		
		IntermediateRunContext branchRuntime = modelRuntime.createBranch();
		thenModel.connectStageOne(branchRuntime);
		SubCodeBlock thenSubCodes = branchRuntime.subCodeBlock;
		
		SubCodeIfThenElse ifThenElseBlock = null;
		if (elseModel!=null) {
			branchRuntime = modelRuntime.createBranch();
			elseModel.connectStageOne(branchRuntime);
			SubCodeBlock elseSubCodes = branchRuntime.subCodeBlock;
			ifThenElseBlock = new SubCodeIfThenElse(thenSubCodes, elseSubCodes);
		} else {
			ifThenElseBlock = new SubCodeIfThenElse(thenSubCodes);
		}

		
		Consumer consumer = new Consumer(this);
		ifThenElseBlock.addConsumer(consumer);
		modelRuntime.pushValueConsumer(consumer);
		ConditionalValueProducer valueProducer = new ConditionalValueProducer(this, consumer, false);
		ifThenElseBlock.setValueProducer(valueProducer);

		modelRuntime.add(ifThenElseBlock);
	}

	@Override
	public void resolveBreaks(int jumpIndex) {
		condition.resolveBreaks(jumpIndex);
		thenModel.resolveBreaks(jumpIndex);
		if (elseModel!=null) {
			elseModel.resolveBreaks(jumpIndex);
		}
	}
	
	//	@Override
	public boolean contains(int index) {
		return condition.contains(index)
			|| thenModel.contains(index)
			|| (elseModel!=null && elseModel.contains(index));
	}
	
	public boolean tryPrepend(IIntermediateNode node) {
		return condition.tryPrepend(node);
	}
	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"[]";
	}
	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateIfThenElse) {
			IntermediateIfThenElse that = (IntermediateIfThenElse) otherNode;
			if (condition.isSame(that.condition) && thenModel.isSame(that.thenModel)) {
				return (elseModel==that.elseModel) || (elseModel!=null && that.elseModel!=null && elseModel.isSame(that.elseModel));
			}
		}
		return false;
	}
	
	
	@Override
	public void dump(StringBuilder dumpBuffer, String prefix) {
		dumpBuffer.append(prefix).append(toString()).append(Logger.ln);
		prefix+="   ";
		dumpBuffer.append(prefix).append("if").append(Logger.ln);
		String prefixSub = prefix+"   ";
		condition.dump(dumpBuffer, prefixSub);

		dumpBuffer.append(prefix).append("then").append(Logger.ln);
		thenModel.dump(dumpBuffer, prefixSub);
		
		if (elseModel!=null) {
			dumpBuffer.append(prefix).append("else").append(Logger.ln);
			elseModel.dump(dumpBuffer, prefixSub);
		}
	}
}


