package net.avaj.classfile.source;

import java.lang.annotation.IncompleteAnnotationException;

public class ExceptionExample {

	public void testThrows() throws IncompleteAnnotationException {
		throw new IncompleteAnnotationException(null, null);
	}
	
	
}
