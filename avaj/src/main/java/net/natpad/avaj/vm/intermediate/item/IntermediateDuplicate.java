package net.natpad.avaj.vm.intermediate.item;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpDuplicate.DupType;
import net.natpad.avaj.vm.intermediate.AbstractIntermediateItem;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.IValueProducer;

public class IntermediateDuplicate extends AbstractIntermediateItem {

	public final DupType dupType;
	
	public IntermediateDuplicate(int index, DupType dupType) {
		super(index);
		this.dupType = dupType;
	}
	
	@Override
	public IIntermediateNode makeCopy(boolean deep) {
		return new IntermediateDuplicate(mnemonicIndex, dupType);
	}
	
	@Override
	public void connectStageOne(IntermediateRunContext modelRuntime) {
		switch(dupType) {
			case DUP : {
				IValueProducer valueProducer = modelRuntime.peekProducer(0);
				if (valueProducer!=null) {
					modelRuntime.pushValueProvider(valueProducer.duplicate());
				} else {
					log.error("invalid stack:"+this);
				}
			} break;
			case DUP2 : {
				IValueProducer valueProducer2 = modelRuntime.peekProducer(0);
				if (valueProducer2!=null) {
					DeclarationType bestType = valueProducer2.getTypeSet().getBestType();
					boolean useForm2 = false;
					if (bestType!=null) {
						if (bestType.isCategory2()) {
							useForm2 = true;
						}
					}
					log.detail("dup2: useForm2="+useForm2+", valueProducer2="+valueProducer2);
					if (!useForm2) {
						IValueProducer valueProducer1 = modelRuntime.peekProducer(1);
						if (valueProducer1!=null) {
							modelRuntime.pushValueProvider(valueProducer1.duplicate());
						} else {
							log.error("form2: invalid stack:"+this);
						}
					}
					modelRuntime.pushValueProvider(valueProducer2.duplicate());
				} else {
					log.error("no producer: invalid stack:"+this);
				}
			} break;
			case DUP_X1 : {
				IValueProducer valueProducer = modelRuntime.peekProducer(0);
				if (valueProducer!=null) {
					modelRuntime.pushValueProvider(valueProducer.duplicate(),2);
				} else {
					log.error("invalid stack:"+this);
				}
				
			} break;
			case DUP_X2 : {
				IValueProducer valueProducer = modelRuntime.peekProducer(0);
				IValueProducer valueTestProducer = modelRuntime.peekProducer(1);
				if (valueProducer!=null && valueTestProducer!=null) {
					
					DeclarationType bestType = valueTestProducer.getTypeSet().getBestType();
					boolean useForm2 = false;
					if (bestType!=null) {
						if (bestType.isCategory2()) {
							useForm2 = true;
						}
					}
					
					
					modelRuntime.pushValueProvider(valueProducer.duplicate(), useForm2 ? 2 : 3);
				} else {
					log.error("invalid stack:"+this);
				}
			} break;
			case DUP2_X1 : {
				throw new RuntimeException("needs implementation");
			} 
			case DUP2_X2 : {
				throw new RuntimeException("needs implementation");
			} 
		}
	}
	
	@Override
	public boolean isSame(IIntermediateNode other) {
		if (other==this) {
			return true;
		}
		if (other instanceof IntermediateDuplicate) {
			IntermediateDuplicate that = (IntermediateDuplicate) other;
			return that.dupType == dupType;
		}
		return false;
	}
	
}
