package net.natpad.avaj.vm.subcode;

import java.util.ArrayList;

import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;


public abstract class AbstractSubCode implements ISubCode {

	protected static Logger log = Logger.getInstance(ISubCode.class);
	
	public final int mnemonic_index;
	protected ArrayList<IConsumer> consumerList = new ArrayList<IConsumer>();
	
	protected IValueProducer valueProducer;
	
	public AbstractSubCode(int mnemonic_index) {
		this.mnemonic_index = mnemonic_index;
	}
	
	public int getMnemonicIndex() {
		return mnemonic_index;
	}
	
	public String subCodeName() {
		return TextUtil.getName(getClass(), "SubCode");
	}
	
	
	
	public void addConsumer(IConsumer consumer) {
		consumerList.add(consumer);
	}
	
	
	@Override
	public IConsumer getInputConsumerAt(int index) {
		if (index<0 || index>=consumerList.size()) {
			return null;
		}
		return consumerList.get(index);
	}
	
	public void setValueProducer(IValueProducer valueProducer) {
		this.valueProducer = valueProducer;
	}
	
	@Override
	public IValueProducer getOutputProducer() {
		return valueProducer;
	}
	
}
