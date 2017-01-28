package net.natpad.avaj.vm.subcode.impl;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;

@SuppressWarnings("serial")
public class ThisOrSuperValue extends MethodFrameValue {

	
	public final DeclarationType thisImportType;
	Identifier superIdentifier;
	
	public ThisOrSuperValue(int frameIndex, DeclarationType thisImportType) {
		super(frameIndex, "this");
		this.thisImportType = thisImportType;
		superIdentifier = new Identifier("super");
	}
	
	
	
	@Override
	public IExpression getExpression(DeclarationType typeDescr) {
		if (typeDescr!=null && typeDescr.isReference()) {
			if (!typeDescr.equals(thisImportType)) {
				return superIdentifier;
			}
		}
		return identifier;
	}
	
}
