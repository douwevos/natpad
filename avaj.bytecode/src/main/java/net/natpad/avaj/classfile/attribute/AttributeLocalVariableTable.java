package net.natpad.avaj.classfile.attribute;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.classfile.ClassfileField;
import net.natpad.avaj.classfile.constant.IConstantProvider;
import net.natpad.avaj.log.Logger;

public class AttributeLocalVariableTable implements IAttribute {

	LocalVariableTable table;
	
	public AttributeLocalVariableTable(AttributeInfo info, IConstantProvider constantProvider) throws IOException {
		ByteArrayInputStream stream = info.getStream();
		DataInputStream dis = new DataInputStream(stream);
		int tableLength = dis.readUnsignedShort();
		table = new LocalVariableTable();
		for(int idx=0; idx<tableLength; idx++) {
			int startPc = dis.readUnsignedShort();
			int length = dis.readUnsignedShort();
			int nameIndex = dis.readUnsignedShort();
			int descriptorIndex = dis.readUnsignedShort();
			int index = dis.readUnsignedShort();

			
			String nameText = constantProvider.getConstantUtf8Text(nameIndex-1);

			
			String descriptorText = constantProvider.getConstantUtf8Text(descriptorIndex-1);
			DeclarationType declarationType = ClassfileField.parseDescriptor(descriptorText);
			
			LocalVariableTableEntry entry = new LocalVariableTableEntry(startPc, length, nameIndex, nameText, descriptorIndex, declarationType, index);
			table.add(entry);
		}
	}

	
	public LocalVariableTable getTable() {
		return table;
	}
	
	@Override
	public void dump(String prefix, StringBuilder buf) {
		buf.append(prefix).append("LocalVariableTableEntry:table-length="+table.count()).append(Logger.ln);
		int idx = 0;
		for(LocalVariableTableEntry entry : table) {
			buf.append(prefix).append(" ["+idx+"] ").append(entry).append(Logger.ln);
//			buf.append(prefix).append(" ["+idx+"] startPc="+TextUtil.hex(entry.startPc, 4)+", length="+entry.length+", nameIndex="+entry.nameIndex+", descIdx="+entry.descriptorIndex+", index="+entry.index).append(Logger.ln);
			idx++;
		}
	}
	
}
