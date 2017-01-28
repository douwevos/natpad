package net.natpad.avaj.ast;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import net.natpad.avaj.ast.formater.Writer;



public class Type {

	public final PackageStatement packageStatement;
	public final TypeHeader typeHeader;
	private final FieldDeclaration fields[];
	private final Method methods[];
	private final ImportStatement imports[];
	
	public Type(PackageStatement packageStatement, ImportStatement imports[], TypeHeader typeHeader, FieldDeclaration fields[], Method methods[]) {
		this.packageStatement = packageStatement;
		this.typeHeader = typeHeader;
		this.fields = fields;
		this.methods = methods;
		this.imports = imports;
	}
	
	public int fieldCount() {
		return fields==null ? 0 : fields.length;
	}
	
	public FieldDeclaration getField(int fieldIndex) {
		return fields[fieldIndex];
	}
	
	
	public List<ImportStatement> getImportStatements() {
		ArrayList<ImportStatement> result = new ArrayList<ImportStatement>();
		result.addAll(Arrays.asList(imports));
		return result;
	}
	
	public FieldDeclaration findFieldByName(String fieldName) {
		for(FieldDeclaration field : fields) {
			Identifier identifier = field.getVariable();
			if (identifier.text.equals(fieldName)) {
				return field;
			}
		}
		return null;
	}
	
	
	
	public int methodCount() {
		return methods.length;
	}
	
	public Method methodAt(int methodIndex) {
		return methods[methodIndex];
	}
	
	
	public void write(Writer out) {
		if (packageStatement!=null) {
			packageStatement.writeStatement(out);
			out.print("\n");
		}
		
		if (imports!=null && imports.length>0) {
			for(ImportStatement importStatement : imports) {
				importStatement.writeStatement(out);
			}
			out.print("\n");
		}
		
		typeHeader.write(out);
		out.print(" {\n");
		out.print("\n");
		out.increaseIndent();
		if (fields!=null && fields.length>0) {
			for(FieldDeclaration field : fields) {
				field.write(out);
				out.print(";\n");
			}
			out.print("\n");
		}
		
		for(Method method : methods) {
			method.writeStatement(out);
			out.print("\n\n");
		}
		out.decreaseIndent();
		out.print("}\n");
	}
	
	
}
