package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.IMnemonicBlock;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.label.Label;
import net.natpad.avaj.classfile.label.LabelRepository;
import net.natpad.avaj.util.TextUtil;

public class OpGoto extends AbstractMnemonic {

	public final int branchOffset;
	
	
	public OpGoto(Operation operation, int offset, int cnt, int branchOffset) {
		super(operation, offset, cnt);
		this.branchOffset = branchOffset;
	}
	
	@Override
	public int getContinuesOffset() {
		return -1;
	}
	
	@Override
	public int getBranchOffset() {
		return branchOffset+offset;
	}
	
	@Override
	public String toString(IMnemonicBlock block) {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		fillSpaces(buf);
		
		int jump = offset+branchOffset;
		if (block == null) {
			buf.append(TextUtil.hex(jump, 4));
		} else {
			LabelRepository labelRepository = block.getLabelRepository();
			Label labelAt = labelRepository.get(block.findByBytecodeOffset(jump));
			buf.append(labelAt.getName());
		}
		return buf.toString();
	}
	
}
