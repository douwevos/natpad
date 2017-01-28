package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.ConditionType;
import net.natpad.avaj.classfile.bytecode.IMnemonicBlock;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.label.Label;
import net.natpad.avaj.classfile.label.LabelRepository;
import net.natpad.avaj.util.TextUtil;

public class OpCompareInt extends AbstractMnemonic {

	private final int branchOffset;
	private final boolean withZero;
	private final ConditionType condition;
	
	
	public OpCompareInt(Operation operation, int offset, int branchoffset, ConditionType condition, boolean withZero) {
		super(operation, offset, 3);
		this.branchOffset = branchoffset;
		this.condition = condition;
		this.withZero = withZero;
//		switch(operation) {
//			case OP_IFEQ : condition=ConditionType.EQUAL; withZero=true; break;
//			case OP_IFNE : condition=ConditionType.NOT_EQUAL; withZero=true; break;
//			case OP_IFLT : condition=ConditionType.LESS_THEN; withZero=true; break;
//			case OP_IFGE : condition=ConditionType.GREATER_OR_EQUAL; withZero=true; break;
//			case OP_IFGT : condition=ConditionType.GREATER_THEN; withZero=true; break;
//			case OP_IFLE : condition=ConditionType.LESS_OR_EQUAL; withZero=true; break;
//			case OP_IF_ICMPEQ : condition=ConditionType.EQUAL; withZero=false; break;
//			case OP_IF_ICMPNE : condition=ConditionType.NOT_EQUAL; withZero=false; break;
//			case OP_IF_ICMPLT : condition=ConditionType.LESS_THEN; withZero=false; break;
//			case OP_IF_ICMPGE : condition=ConditionType.GREATER_OR_EQUAL; withZero=false; break;
//			case OP_IF_ICMPGT : condition=ConditionType.GREATER_THEN; withZero=false; break;
//			case OP_IF_ICMPLE : condition=ConditionType.LESS_OR_EQUAL; withZero=false; break;
//			default : condition=ConditionType.LESS_OR_EQUAL; withZero=false; break;
//		}
	}
	
	@Override
	public int getBranchOffset() {
		return offset+branchOffset;
	}

	public boolean isWithZero() {
		return withZero;
	}

	
	public ConditionType getConditionType() {
		return condition;
	}
	
	@Override
	public String toString() {
		return toString(null);
	}

	@Override
	public String toString(IMnemonicBlock block) {
		StringBuilder buf = new StringBuilder("");
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
