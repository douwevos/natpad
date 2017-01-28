package net.avaj.classfile.source;

import net.natpad.avaj.ast.Method;
import net.natpad.avaj.ast.MethodHeader;
import net.natpad.avaj.ast.Name;
import net.natpad.avaj.ast.formater.Writer;

import org.junit.Assert;
import org.junit.Test;

public class ExceptionExampleUT {

	Writer out = new Writer();

	
	@Test
	public void testSimpleException() {
		try {
			Helper helper = new Helper("ExceptionExample.class");
			
			helper.type.write(out);

			System.out.println("out="+out);

			Method testThrows = helper.findMethodByName("testThrows");
			MethodHeader methodHeader = testThrows.getMethodHeader();
			Name exception = methodHeader.exceptionAt(0);
			Assert.assertEquals(new Name("IncompleteAnnotationException", false), exception);
			
			
			
		} catch(Throwable t) {
			t.printStackTrace();
		}
	}
	
}
