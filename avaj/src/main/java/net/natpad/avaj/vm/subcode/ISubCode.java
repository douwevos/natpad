package net.natpad.avaj.vm.subcode;

import net.natpad.avaj.vm.ModelStageTwoRuntime;

public interface ISubCode {

	public void connectStageTwo(ModelStageTwoRuntime modelRuntime);

	
	public IConsumer getInputConsumerAt(int index);

	public IValueProducer getOutputProducer();
	
}
