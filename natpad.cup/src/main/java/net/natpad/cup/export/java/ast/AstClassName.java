package net.natpad.cup.export.java.ast;

public class AstClassName {

	public final AstQualifiedName astPackage;
	public final String className;
	
	public AstClassName(AstQualifiedName astPackage, String className) {
		this.astPackage = astPackage;
		this.className = className;
	}

	public AstClassName(String importName) {
		int idx = importName.lastIndexOf('.');
		if (idx>=0) {
			astPackage = new AstQualifiedName(importName.substring(0,idx));
			className = importName.substring(idx+1);
		} else {
			astPackage = null;
			className = importName;
		}
	}

	public String getFullQualifiedName() {
		return astPackage==null ? className : astPackage.getFullQualifiedName()+"."+className;
	}
	
}
