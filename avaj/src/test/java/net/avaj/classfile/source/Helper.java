package net.avaj.classfile.source;

import java.io.FileInputStream;
import java.io.IOException;

import net.natpad.avaj.Decompiler;
import net.natpad.avaj.ast.Method;
import net.natpad.avaj.ast.Type;
import net.natpad.avaj.classfile.Classfile;
import net.natpad.avaj.classfile.ClassfileParser;

public class Helper {

	public final Type type;
	
	public Helper(String className) throws IOException {
		
		FileInputStream fis = null;
		if (className.indexOf('/')>=0) {
			fis = new FileInputStream(className);
		} else {
			fis = new FileInputStream("bin/net/avaj/classfile/source/"+className);
		}
//		FileInputStream fis = new FileInputStream(className);
//		Logger.defaultLogLevel = LogLevel.ERROR;
		Classfile classfile = new ClassfileParser().doParse(fis);
		Decompiler gen = new Decompiler(classfile);
		type = gen.create();
    }
	
	public Method findMethodByName(String methodName) {
		int methodCount = type.methodCount();
		for(int idx=0; idx<methodCount; idx++) {
			Method method = type.methodAt(idx);
//			System.out.println("method="+method+", method.methodHeader.methodName="+method.methodHeader.methodName.text);
			if (method.methodHeader.methodName.text.equals(methodName)) {
				return method;
			}
		}
		return null;
	}

}
