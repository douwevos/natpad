package net.natpad.avaj.vm;

import net.natpad.avaj.classfile.attribute.LineNumberTable;
import net.natpad.avaj.classfile.attribute.LineNumberTableEntry;
import net.natpad.avaj.classfile.bytecode.IMnemonicBlock;

public class MnemonicLineNumberTable extends LineNumberTable {

	
	public MnemonicLineNumberTable(IMnemonicBlock mnemonicBlock, LineNumberTable lineNumberTable) {
		for(LineNumberTableEntry entry : lineNumberTable) {
			int mnemonicIndex = mnemonicBlock.findByBytecodeOffset(entry.startPc);
			add(new LineNumberTableEntry(mnemonicIndex, entry.lineNumber));
		}
	}

	public int getLeastLineNumber() {
		int result = -1;
		for(LineNumberTableEntry entry : this) {
			if ((entry.lineNumber!=-1 && entry.lineNumber<result) || (result == -1)) {
				result = entry.lineNumber;
			}
		}
		return result;
	}
	
}
