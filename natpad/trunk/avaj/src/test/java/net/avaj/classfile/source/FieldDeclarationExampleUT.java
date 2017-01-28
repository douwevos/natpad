package net.avaj.classfile.source;

import java.io.IOException;

import junit.framework.Assert;
import net.natpad.avaj.ast.FieldDeclaration;
import net.natpad.avaj.ast.Type;
import net.natpad.avaj.ast.formater.Writer;

import org.junit.Test;

public class FieldDeclarationExampleUT {

	Writer out = new Writer();

	@Test
	public void testFields() {
		try {
			Helper helper = new Helper("FieldDeclarationExample.class");
			Type type = helper.type;
			
			
			checkField(type, "refToThisClass", "public FieldDeclarationExample refToThisClass");
			checkField(type, "simpleIntValue", "public int simpleIntValue");
			checkField(type, "privateIntValue", "private int privateIntValue");
			checkField(type, "protectedIntValue", "protected int protectedIntValue");
			checkField(type, "defaultIntValue", "int defaultIntValue");
			checkField(type, "publicStaticIntValue", "public static int publicStaticIntValue");
			checkField(type, "publicVolatileIntValue", "public volatile int publicVolatileIntValue");
			checkField(type, "publicTransientIntValue", "public transient int publicTransientIntValue");
			
			type.write(out);
			System.out.println(""+out);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void checkField(Type type, String fieldName, String expected) {
	    out.clear();
	    FieldDeclaration fieldDecl = type.findFieldByName(fieldName);
	    fieldDecl.write(out);
	    System.out.println("out="+out.toString());
	    Assert.assertEquals(expected, out.toString());
    }
	
	
	
}
