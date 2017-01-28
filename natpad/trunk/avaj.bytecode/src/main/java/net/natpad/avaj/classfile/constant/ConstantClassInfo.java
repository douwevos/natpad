package net.natpad.avaj.classfile.constant;

import java.util.ArrayList;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.Name;
import net.natpad.avaj.classfile.bytecode.Type;
import net.natpad.avaj.util.TextUtil;

public class ConstantClassInfo implements IConstant {

	public final int nameIndex;
	private ConstantUtfText constantUtfText;
	private DeclarationType parsedDescriptor;
	
	public ConstantClassInfo(int nameIndex) {
		this.nameIndex = nameIndex;
	}
	
	
	private boolean isResolved;
	
	@Override
	public boolean isResolved() {
		return isResolved;
	}
	
	
	@Override
	public boolean tryResolve(ConstantPool constantPool, ArrayList<IConstant> resolveStack) throws RecursiveConstantsException {
		constantUtfText = (ConstantUtfText) constantPool.getUnresolved(nameIndex-1);
		parsedDescriptor = parseDescriptor(constantUtfText.text);
		if (parsedDescriptor==null) {
			throw new RuntimeException("text="+constantUtfText.text);
		}
		isResolved = true;
		return true;
	}

	public Name getRefrenceTypeName() {
		return parsedDescriptor.referenceTypeName;
	}
	
	public DeclarationType getParsedDescriptor() {
		return parsedDescriptor;
	}
	
	
	private final DeclarationType parseDescriptor(String text) {
		int arrayCnt = 0;
		DeclarationType result = null;
		if (text.startsWith("[")) {
			int idx = 0;
			boolean parsingName = false;
			StringBuilder buf = new StringBuilder();
			while(result==null && idx<text.length()) {
				char ch = text.charAt(idx++);
				
				if (parsingName) {
					if (ch==';') {
						Name name = new Name(buf.toString(), true);
						result = new DeclarationType(name, arrayCnt);
					} else {
						buf.append(ch);
					}
				} else {
					switch(ch) {
						case '[' : {
							arrayCnt++;
						} break;
						case 'L' : {
							parsingName = true;
						} break;
						case 'B' : result = new DeclarationType(Type.BYTE, arrayCnt); break;
						case 'C' : result = new DeclarationType(Type.CHAR, arrayCnt); break;
						case 'D' : result = new DeclarationType(Type.DOUBLE, arrayCnt); break;
						case 'F' : result = new DeclarationType(Type.FLOAT, arrayCnt); break;
						case 'I' : result = new DeclarationType(Type.INT, arrayCnt); break;
						case 'J' : result = new DeclarationType(Type.LONG, arrayCnt);  break;
						case 'S' : result = new DeclarationType(Type.SHORT, arrayCnt);  break;
						case 'Z' : result = new DeclarationType(Type.BOOLEAN, arrayCnt);  break;
						default : {
							// TODO: handle invalid descrptior
						}
					}
				}
			}
		} else {
			Name name = new Name(text, true);
			result = new DeclarationType(name, arrayCnt);
		}
		return result;
	}
	
	
	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder(TextUtil.getName(getClass())).append("[");
		buf.append("constantUtfText=").append(constantUtfText);
		buf.append(", nameIndex=").append(nameIndex);
		buf.append(", parsedDescriptor=").append(parsedDescriptor);
		return buf.append("]").toString();
	}
}
