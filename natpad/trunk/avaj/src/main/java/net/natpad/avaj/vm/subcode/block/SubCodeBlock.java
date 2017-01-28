package net.natpad.avaj.vm.subcode.block;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.AbstractSubCode;
import net.natpad.avaj.vm.subcode.IConsumer;
import net.natpad.avaj.vm.subcode.ISubCode;
import net.natpad.avaj.vm.subcode.IValueProducer;

public class SubCodeBlock implements ISubCode, Iterable<ISubCode> {

	static Logger log = Logger.getInstance(ISubCode.class);
	
	protected List<ISubCode> subCodeList = new ArrayList<ISubCode>();

	
	public SubCodeBlock() {
	}
	
	public final void append(ISubCode model) {
		subCodeList.add(model);
	}
	
	
	
	
	@Override
	public Iterator<ISubCode> iterator() {
		return subCodeList.iterator();
	}
	
	

	@Override
	public final void connectStageTwo(ModelStageTwoRuntime modelRuntime) {
		int idx=0;
		for(ISubCode subCode : subCodeList) {
			try {
				if (subCode instanceof AbstractSubCode) {
					modelRuntime.setMnemonicIndex(((AbstractSubCode) subCode).getMnemonicIndex());
				}
				subCode.connectStageTwo(modelRuntime);
				log.debug(idx+" # "+subCode);
				idx++;
			} catch(Throwable t) {
				t.printStackTrace();
			}
		}
	}


	@Override
	public IConsumer getInputConsumerAt(int index) {
		return null;
	}
	
	
	@Override
	public IValueProducer getOutputProducer() {
		return null;
	}
	
//	@Override
//	public void dump(StringBuilder dumpBuffer, String prefix) {
//		int offset = -1;
//		for(int idx=0; idx<subCodeList.size(); idx++) {
//			ISubCode model = subCodeList.get(idx);
//			if (offset<0) {
//				if (model instanceof AbstractSubCode) {
//					offset = ((AbstractSubCode) model).index;
//				} else {
//					offset = 0;
//				}
//			}
//			dumpBuffer.append(prefix).append(idx+offset);
//			dumpBuffer.append(" ");
//			dumpBuffer.append(model).append(Logger.ln);
//			if (model instanceof ISubCodeBlock) {
//				((ISubCodeBlock) model).dump(dumpBuffer, prefix+"   ");
//			}
//		}
//	}



	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"[subCodeList.size="+subCodeList.size()+"]";
	}

	
	
}
