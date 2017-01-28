package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateNodeScope;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.block.SubCodeBlock;
import net.natpad.avaj.vm.subcode.impl.SubCodeIfThenElse;

public class IntermediateConditionPusher implements IIntermediateNode {

	static Logger log = Logger.getInstance(IIntermediateNode.class);
	
	public final IIntermediateNode condition;
	public final IIntermediateNode thenModel;
	public final IIntermediateNode elseModel;
	public final boolean invertExpression;

	private IntermediateNodeScope scope = new IntermediateNodeScope(-1, -1);
	
	public IntermediateConditionPusher(IIntermediateNode condition, IIntermediateNode thenModel, IIntermediateNode elseModel, boolean invertExpression) {
		this.condition = condition;
		this.thenModel = thenModel;
		this.elseModel = elseModel;
		this.invertExpression = invertExpression;
	}
	
	public IIntermediateNode makeCopy(boolean deep) {
		if (deep) {
			return new IntermediateConditionPusher(condition.makeCopy(true), thenModel.makeCopy(true), elseModel.makeCopy(true), invertExpression);
		}
		return new IntermediateConditionPusher(condition, thenModel, elseModel, invertExpression);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext groupConnectRuntime) {
		condition.connectStageOne(groupConnectRuntime);
		IntermediateRunContext branchRuntime = groupConnectRuntime.createBranch();
		thenModel.connectStageOne(branchRuntime);
		SubCodeBlock thenSubCodes = branchRuntime.subCodeBlock;
		
		branchRuntime = groupConnectRuntime.createBranch();
		elseModel.connectStageOne(branchRuntime);
		SubCodeBlock elseSubCodes = branchRuntime.subCodeBlock;

		SubCodeIfThenElse ifThenElseBlock = new SubCodeIfThenElse(thenSubCodes, elseSubCodes);
//		groupConnectRuntime.add(ifThenElseBlock);
		Consumer consumer = new Consumer(this);
		ifThenElseBlock.addConsumer(consumer);
		groupConnectRuntime.pushValueConsumer(consumer);
		ConditionalValueProducer valueProducer = new ConditionalValueProducer(this, consumer, invertExpression);
		ifThenElseBlock.setValueProducer(valueProducer);
		groupConnectRuntime.pushValueProvider(valueProducer);
	}

	@Override
	public void resolveBreaks(int jumpIndex) {
		condition.resolveBreaks(jumpIndex);
		thenModel.resolveBreaks(jumpIndex);
		elseModel.resolveBreaks(jumpIndex);
	}
	
	@Override
	public IntermediateNodeScope getScope() {
		scope.setExitIndex(elseModel.getScope().getExitIndex());
		return scope;
	}
	
	@Override
	public boolean contains(int mnemonicIndex) {
		return condition.contains(mnemonicIndex) || thenModel.contains(mnemonicIndex) || elseModel.contains(mnemonicIndex);
	}
	
	public boolean tryPrepend(IIntermediateNode node) {
		return condition.tryPrepend(node);
	}
	

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateConditionPusher) {
			IntermediateConditionPusher that = (IntermediateConditionPusher) otherNode;
			return condition.isSame(that.condition) && thenModel.isSame(that.thenModel) && elseModel.isSame(that.elseModel);
		}
		return false;
	}
	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"[scope="+getScope()+"]";
	}
	
	@Override
	public void dump(StringBuilder dumpBuffer, String prefix) {
		dumpBuffer.append(prefix).append(toString()).append(Logger.ln);
		prefix+="   ";
		dumpBuffer.append(prefix).append("condition").append(Logger.ln);
		String prefixSub = prefix+"   ";
		condition.dump(dumpBuffer, prefixSub);

		dumpBuffer.append(prefix).append("on-true").append(Logger.ln);
		thenModel.dump(dumpBuffer, prefixSub);
		
		if (elseModel!=null) {
			dumpBuffer.append(prefix).append("on-false").append(Logger.ln);
			elseModel.dump(dumpBuffer, prefixSub);
		}

		
	}
}
