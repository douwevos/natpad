package net.natpad.avaj.vm.intermediate;

import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;

public abstract class AbstractIntermediateItem implements IIntermediateNode {

	protected static Logger log = Logger.getInstance(IIntermediateNode.class);
	
	public final int mnemonicIndex;
	
	protected IntermediateNodeScope scope;
	
	public AbstractIntermediateItem(int mnemonicIndex) {
		this.mnemonicIndex = mnemonicIndex;
		scope = new IntermediateNodeScope(mnemonicIndex+1, IntermediateNodeScope.INDEX_NONE);
	}
	

	@Override
	public IntermediateNodeScope getScope() {
		return scope;
	}
	
	@Override
	public void resolveBreaks(int jumpIndex) {
	}
	
	@Override
	public boolean contains(int mnemonicIndex) {
		return mnemonicIndex == this.mnemonicIndex;
	}
	
	public boolean tryPrepend(IIntermediateNode node) {
		return false;
	}
	
	@Override
	public void dump(StringBuilder dumpBuffer, String prefix) {
		dumpBuffer.append(prefix).append(toString()).append(Logger.ln);
	}
	
	@Override
	public String toString() {
		String result = TextUtil.getName(getClass());
		if (scope!=null) {
			result+="[scope="+scope+"]";
		}
		return result;
	}
}
