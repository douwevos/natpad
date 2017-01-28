package net.natpad.avaj.classfile.label;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import net.natpad.avaj.classfile.bytecode.IMnemonic;
import net.natpad.avaj.classfile.label.Label.LabelType;
import net.natpad.avaj.log.Logger;

public class LabelRepository implements Iterable<Label> {

	static Logger log = Logger.getInstance(IMnemonic.class);
	
	private final Map<Integer, Label> labelSet = new HashMap<Integer, Label>();
	private int hardCount;

	public Label get(int mnemonicIndex) {
		return labelSet.get(mnemonicIndex);
	}

	
	public Label createSoft(int mnemonicIndex, int offset) {
		Label result = labelSet.get(mnemonicIndex);
		if (result == null) {
			result = new Label(LabelType.SOFT, mnemonicIndex, offset, null);
			labelSet.put(mnemonicIndex, result);
		}
		return result;
	}

	public Label createHard(int mnemonicIndex, int bytecodeOffset) {
		Label result = labelSet.get(mnemonicIndex);
		if (result == null) {
			result = new Label(LabelType.HARD, mnemonicIndex, bytecodeOffset, "lab"+hardCount++);
			labelSet.put(mnemonicIndex, result);
		} else if (result.getType()==LabelType.SOFT) {
			result.setType(LabelType.HARD);
			result.setName("lab"+hardCount++);
		}
		return result;
	}

	public void dump() {
		for(Label lab : this) {
			log.debug(""+lab);
		}
	}
	
	
	@Override
	public Iterator<Label> iterator() {
		return new Iterator<Label>() {
			Iterator<Label> internal = labelSet.values().iterator();
			
			@Override
			public boolean hasNext() {
				return internal.hasNext();
			}
			
			@Override
			public Label next() {
				return internal.next();
			}
			@Override
			public void remove() {
				throw new UnsupportedOperationException();
			}
		};
	}
	
}
