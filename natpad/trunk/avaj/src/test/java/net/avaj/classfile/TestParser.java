package net.avaj.classfile;

import java.io.FileInputStream;
import java.io.IOException;

import net.natpad.avaj.classfile.ClassfileParser;

public class TestParser {

	public static void main(String[] args) {
		new TestParser();
	}
	
	public TestParser() {
		FileInputStream fis;
		try {
			fis = new FileInputStream("bin/net/avaj/classfile/parser/ClassFileParser.class");
			new ClassfileParser().doParse(fis);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}
