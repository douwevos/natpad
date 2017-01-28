package net.natpad.avaj.classfile.label;

import net.natpad.avaj.util.TextUtil;

public class Label {

	public enum LabelType { HARD, SOFT }
	
	public final int mnemonicIndex;
	public final int bytecodeOffset;
	private String name;
	private LabelType type;
	
	Label(LabelType type, final int mnemonicIndex, final int bytecodeOffset, final String name) {
		this.mnemonicIndex = mnemonicIndex;
		this.bytecodeOffset = bytecodeOffset;
		this.name = name;
		if (name==null) {
			this.name = "Label("+mnemonicIndex+":"+TextUtil.hex(bytecodeOffset, 4)+")";
		}
		this.type = type;
	}


	public void setName(String newName) {
		name = newName;
	}

	public String getName() {
		return name;
	}

	public void setType(LabelType type) {
		this.type = type;
	}

	public LabelType getType() {
		return type;
	}
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(name).append(": ").append(mnemonicIndex).append(", ").append(TextUtil.hex(bytecodeOffset, 4));
		
//		if (callers!=null) {
//			buf.append("     [::");
//			boolean isFirst = true;
//			for(Label ext : callers) {
//				if (isFirst) {
//					isFirst = false;
//				} else {
//					buf.append(", ");
//				}
//				buf.append(ext.mnemonicIndex);
//			}
//			buf.append("::]");
//		}
		
		return buf.toString();
	}
	
	
	@Override
	public boolean equals(Object obj) {
		if (obj==this) {
			return true;
		}
		if (obj instanceof Label) {
			Label other = (Label) obj;
			return mnemonicIndex==other.mnemonicIndex && bytecodeOffset==other.bytecodeOffset;
		}
		return false;
	}

	@Override
	public int hashCode() {
		return mnemonicIndex*15+bytecodeOffset;
	}
	
}
