package net.natpad.avaj.vm.intermediate;


public interface IIntermediateNode {

	public void connectStageOne(IntermediateRunContext groupConnectRuntime);
	public void resolveBreaks(int jumpIndex);

	public IntermediateNodeScope getScope();
	
	public boolean contains(int mnemonicIndex);

	public void dump(StringBuilder dumpBuffer, String prefix);

	public boolean tryPrepend(IIntermediateNode node);

	public boolean isSame(IIntermediateNode otherNode);

	public IIntermediateNode makeCopy(boolean deep);
	
}
