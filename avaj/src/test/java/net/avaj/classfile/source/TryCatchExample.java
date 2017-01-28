package net.avaj.classfile.source;

import java.lang.reflect.InvocationTargetException;

public class TryCatchExample {

	
	public void simpleArrayCreation() throws InvocationTargetException {
		System.out.print("testje");
		int j =5;
		
		try {
			try {
				j = 20;
				simpleArrayCreation();
			} catch(InvocationTargetException e) {
				throw new NullPointerException("");
			}
			j=j+500;
			
		} catch(NullPointerException e) {
			e.printStackTrace();
		} catch(Exception e) {
			if (e.equals("dsf")) {
				System.out.println(e.getMessage());
			} else {
				simpleArrayCreation();
				return;
			}
		}
		System.out.println("ja hoor");
	}


}
