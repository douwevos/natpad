package net.avaj.classfile.source;

import net.natpad.avaj.ast.formater.Writer;

import org.junit.Test;

public class ConstructorExampleUT {



	Writer out = new Writer();

	
	@Test
	public void testConstructurs() {
		try {
			Helper helper = new Helper("ConstructorExample.class");
			
			helper.type.write(out);

			System.out.println("out="+out);
			
			
			
		} catch(Throwable t) {
			t.printStackTrace();
		}
	}
	
}
