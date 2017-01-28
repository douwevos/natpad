package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateNodeScope;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.impl.SubCodeReturn;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class IntermediateReturn extends AbstractIntermediateItem {
	
	public final DeclarationType returnTypeDescription;
	
	public IntermediateReturn(int index, DeclarationType returnTypeDescription) {
		super(index);
		getScope().setExitIndex(IntermediateNodeScope.INDEX_EXIT);
		this.returnTypeDescription = returnTypeDescription;
	}
	
	public IIntermediateNode makeCopy(boolean deep) {		return new IntermediateReturn(mnemonicIndex, returnTypeDescription);
	}
	
	
	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		return otherNode instanceof IntermediateReturn;
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		SubCodeReturn subCodeReturn = new SubCodeReturn(this.mnemonicIndex, returnTypeDescription);
		modelRuntime.add(subCodeReturn);
		if (returnTypeDescription!=null) {
			Consumer returnConsumer = new Consumer(this);
			subCodeReturn.addConsumer(returnConsumer);
			modelRuntime.pushValueConsumer(returnConsumer);
			returnConsumer.getTypeSet().add(new ValueTypeDefinition(returnTypeDescription,  ValueTypeDefinitionKind.RETURN));
		}
	}

	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder(TextUtil.getName(getClass())).append("[");
		if (returnTypeDescription!=null) {
			buf.append("return-type:"+returnTypeDescription);
		}
		return buf.append("]").toString();

	}
	
}
