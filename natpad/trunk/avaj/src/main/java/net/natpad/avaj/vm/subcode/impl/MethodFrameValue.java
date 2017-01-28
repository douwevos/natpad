package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

@SuppressWarnings("serial")
public class MethodFrameValue extends ValueTypeDefinitionList {

	public final int frameIndex;
	protected Identifier identifier;
	
	
	public MethodFrameValue(int frameIndex, String identifier) {
		super();
		this.identifier = identifier==null ? null : new Identifier(identifier);
		this.frameIndex = frameIndex;
	}
	

	public IExpression getExpression(DeclarationType typeDescr) {
		Identifier result = identifier;
		if (result==null) {
			result = new Identifier("lfv"+frameIndex);
		}
		return result;
	}
	
	public boolean setName(String name) {
		boolean result = true;
		if (identifier!=null) {
			result = (identifier.text.equals(name));
		} else {
			identifier = new Identifier(name);
		}
		return result;
	}
	
	@Override
	public int hashCode() {
		return frameIndex;
	}
	
	@Override
	public String toString() {
		String out = "";
		if (!isEmpty()) {
			for(ValueTypeDefinition valTypeDef :this) {
				if (out.length()>0) {
					out+=", ";
				}
				out += valTypeDef.typeDefinition;
				out += ":"+valTypeDef.kind;
			}
		}
		
		return TextUtil.getName(getClass())+"[types="+out+", frameIndex="+frameIndex+", :: "+super.toString()+"]";
	}
	
}
