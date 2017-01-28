package net.avaj.classfile.source;

import java.io.FileInputStream;
import java.io.IOException;

import net.natpad.avaj.Decompiler;
import net.natpad.avaj.ast.Type;
import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.classfile.Classfile;
import net.natpad.avaj.classfile.ClassfileParser;

public class TestGenerator {

	
	public static void main(String[] args) {
		new TestGenerator();
	}
	
	public TestGenerator() {
		FileInputStream fis;
		try {
			fis = new FileInputStream("bin/net/avaj/classfile/parser/TestClass.class");
//			fis = new FileInputStream("bin/net/avaj/classfile/parser/ParsedModifiers.class");
			Classfile classfile = new ClassfileParser().doParse(fis);
			Decompiler gen = new Decompiler(classfile);
			
			Type type = gen.create();
			
			Writer out = new Writer();
			type.write(out);
			System.out.println(""+out);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}
