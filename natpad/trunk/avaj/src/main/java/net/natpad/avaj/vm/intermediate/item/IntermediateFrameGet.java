package net.natpad.avaj.vm.intermediate.item;

import java.util.List;

import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.Consumer;
import net.natpad.avaj.vm.subcode.IValueProducer;
import net.natpad.avaj.vm.subcode.impl.FrameValueProducer;
import net.natpad.avaj.vm.subcode.impl.MethodFrameValue;

public class IntermediateFrameGet extends AbstractIntermediateItem  {

	public final int frameIndex;
	
	
	public IntermediateFrameGet(int index, int frameIndex) {
		super(index);
		this.frameIndex = frameIndex;
	}

	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateFrameGet(mnemonicIndex, frameIndex);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		MethodFrameValue frameValue = modelRuntime.getMethodFrameValue(frameIndex);
		if (frameValue==null) {
			frameValue = modelRuntime.createMethodFrameValue(frameIndex);
			log.warn("frame-value requested which did not exists yet: frameValue="+frameValue+", frameIndex="+frameIndex);
		}
		List<IValueProducer> pseudoStack = modelRuntime.pseudoStack;
		for(int idx=pseudoStack.size()-1; idx>=0; idx--) {
			IValueProducer producer = pseudoStack.get(idx);
			if (producer instanceof FrameValueProducer) {
				FrameValueProducer frameValueProducer = (FrameValueProducer) producer;
				if (frameValueProducer.getFrameIndex() == frameIndex) {
					producer.setConsumer(new Consumer(this));
					pseudoStack.remove(idx);
					modelRuntime.pushValueProvider(frameValueProducer);
					
					return;
					
//					if (value==1) {
//						frameValueProducer.opperation = SubCodeFrameGet.ValueProducer.Opperation.POSTFIXINCREMENT;
//					} else {
//						frameValueProducer.opperation = SubCodeFrameGet.ValueProducer.Opperation.POSTFIXDECREMENT;
//					}
//					commandConsumed = true;
				}
			}
		}
		
		
		FrameValueProducer valueProducer = new FrameValueProducer(this, frameValue);
		modelRuntime.pushValueProvider(valueProducer);
	}

	@Override
	public boolean isSame(IIntermediateNode otherNode) {
		if (otherNode instanceof IntermediateFrameGet) {
			IntermediateFrameGet that = (IntermediateFrameGet) otherNode;
			return frameIndex == that.frameIndex;
		}
		return false;
	}
	
	@Override
	public String toString() {
		return TextUtil.getName(getClass())+"[index="+frameIndex+"]";
	}
	
}
