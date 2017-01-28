package net.natpad.avaj.classfile.attribute;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.util.TextUtil;

public class LocalVariableTableEntry {

	public final int startPc;
	public final int length;
	public final int nameIndex;
	public final int descriptorIndex;
	public final int index;
	
	public final String nameText;
	public final DeclarationType declarationType;
	
	
	public LocalVariableTableEntry(int startPc, int length, int nameIndex, String nameText, int descriptorIndex, DeclarationType declarationType, int index) {
		this.startPc = startPc;
		this.length = length;
		this.nameIndex = nameIndex;
		this.nameText = nameText;
		this.descriptorIndex = descriptorIndex;
		this.declarationType = declarationType;
		this.index = index;
	}
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder("LocalVariableTableEntry[startPc=").append(TextUtil.hex(startPc, 4));
		buf.append(", length="+length);
		buf.append(", nameIndex="+nameIndex);
		buf.append(", descriptorIndex="+descriptorIndex);
		buf.append(", index="+index);
		buf.append(", descType="+declarationType);
		buf.append(", name="+nameText);
		buf.append("]");
		return buf.toString();
	}

}
