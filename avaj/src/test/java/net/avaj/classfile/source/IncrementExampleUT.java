package net.avaj.classfile.source;

import java.io.IOException;

import net.natpad.avaj.ast.Type;
import net.natpad.avaj.ast.formater.Writer;

import org.junit.Test;

public class IncrementExampleUT {


	Writer out = new Writer();

	@Test
	public void testIncrement() {
		try {
			Helper helper = new Helper("IncrementExample.class");
			Type type = helper.type;
			type.write(out);
			System.out.println(""+out);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
