package net.natpad.avaj.classfile.bytecode;

import net.natpad.avaj.classfile.label.LabelRepository;


public interface IMnemonicBlock extends Iterable<IMnemonic> {

	public IMnemonic get(int index);
	public int count();

	public int findByBytecodeOffset(int bytecodeOffset);
	public LabelRepository getLabelRepository();


}
