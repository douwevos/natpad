package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.ConditionType;
import net.natpad.avaj.classfile.bytecode.IMnemonicBlock;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.label.Label;
import net.natpad.avaj.classfile.label.LabelRepository;
import net.natpad.avaj.util.TextUtil;

public class OpCompareReference extends AbstractMnemonic {

	public ConditionType condition;
	public boolean withZero;
	public int branchOffset;
	
	public OpCompareReference(Operation operation, int offset, int branchOffset, ConditionType condition, boolean withZero) {
		super(operation, offset, 3);
		this.branchOffset = branchOffset;
		this.condition = condition;
		this.withZero = withZero;
	}
	
	@Override
	public int getBranchOffset() {
		return offset+branchOffset;
	}


	
	public ConditionType getConditionType() {
		return condition;
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
