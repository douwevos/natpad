package net.avaj.classfile.source;

import net.natpad.avaj.ast.formater.Writer;

import org.junit.Test;

public class ClassExampleUT {

	
	Writer out = new Writer();

	
	@Test
	public void test() {
		try {
			Helper helper = new Helper("ClassExample.class");
//			Helper helper = new Helper("/home/superman/work/cpp-workspace/natpad/natpad.cup/build/classes/net/natpad/cup/Main.class");
			
			helper.type.write(out);
			System.out.println("out="+out);
			
		} catch(Throwable t) {
			t.printStackTrace();
		}
	}
	

}
