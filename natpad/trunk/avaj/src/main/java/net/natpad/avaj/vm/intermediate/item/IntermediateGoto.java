package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.BreakStatement;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.AbstractSubCode;

public class IntermediateGoto extends AbstractIntermediateItem {

	private boolean isABreak = false;
	
	public IntermediateGoto(int index, int exitIndex) {
		super(index);
		super.getScope().setExitIndex(exitIndex);
	}
	
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateGoto(mnemonicIndex, getScope().getExitIndex());
	}
	
	@Override
	public void resolveBreaks(int jumpIndex) {
		log.detail("jumpIndex="+jumpIndex+", mnemonicIndex="+mnemonicIndex);
		if (jumpIndex==getScope().getExitIndex()) {
			getScope().setExitIndex(mnemonicIndex+1);
			isABreak = true;
		}
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		if (isABreak) {
			modelRuntime.add(new AbstractSubCode(mnemonicIndex) {
				@Override
				public void connectStageTwo(ModelStageTwoRuntime model2Runtime) {
					model2Runtime.appendStatement(new BreakStatement());
				}
			});
		}
	}

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateGoto) {
			IntermediateGoto that = (IntermediateGoto) otherNode;
			return getScope().getExitIndex() == that.getScope().getExitIndex();
		}
		return false;
	}
	
		
}
