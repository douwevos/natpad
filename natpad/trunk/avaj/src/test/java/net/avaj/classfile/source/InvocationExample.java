package net.avaj.classfile.source;

import java.util.Properties;

public class InvocationExample {

	Properties properties;
	
	static boolean test;

	public InvocationExample() {

	}
	
	
//	public StringBuilder createBuilder() {
//		return new StringBuilder().append("hallo");
//	}
//	
//	public void testJustAPrint(int a, boolean c) {
//		
//		System.out.print("hallo"+a);
//		
//	}
//	
	
	public void run() {
		for(long idx=0; idx<30; idx++) {
			System.out.println("idx="+idx);
		}
	}
	
}
