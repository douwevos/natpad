package net.natpad.avaj;

import java.io.FileInputStream;
import java.io.IOException;

import net.natpad.avaj.ast.Type;
import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.classfile.Classfile;
import net.natpad.avaj.classfile.ClassfileParser;

public class Main {

	
	
	public Main() {
	}
	
	
	public static void main(String[] args) {
		if (args.length==0) {
			System.out.println("usage: avaj <class-file>");
			return;
		}
		
		try {
			FileInputStream fis = new FileInputStream(args[0]);
			Classfile parsedClass = new ClassfileParser().doParse(fis);
			Decompiler gen = new Decompiler(parsedClass);
			Type createdType = gen.create();
			
			Writer out = new Writer();
			createdType.write(out);
			
			System.out.println(out);
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}
}
