package net.avaj.classfile.source;

import net.natpad.avaj.ast.formater.Writer;

import org.junit.Test;

public class ComplexMethodsExampleUT {


	Writer out = new Writer();

	
	@Test
	public void testSort_Short_LList() {
		try {
			Helper helper = new Helper("ComplexMethodsExample.class");
			
			helper.type.write(out);

			System.out.println("out="+out);
			
			
			
		} catch(Throwable t) {
			t.printStackTrace();
		}
	}
	
}
