package net.avaj.classfile.source;

public class TryFinalExample {

	
	@SuppressWarnings("unused")
	private void simpleTest() {
		String test = null;
		try {
			test = "test me";
		} finally {
			test = "douwe";
		}
		System.out.println(test);
	}
	
}
