package net.avaj.classfile.source;

import java.lang.reflect.InvocationTargetException;

public class SynchronizedExample {

	
	public void simpleArrayCreation() throws InvocationTargetException {
		synchronized(SynchronizedExample.class) {
//			try {
				simpleArrayCreation();
//			} finally {
//				System.out.println("hallo");
//			}
		}
	}

	
	public void simpleArrayCreationNoException() {
		synchronized(SynchronizedExample.class) {
			simpleArrayCreationNoException();
		}

	}

}
