package net.avaj.classfile.parser;

import net.natpad.avaj.ast.Name;
import net.natpad.avaj.classfile.ClassfileMethodHeader;

import org.junit.Test;

/**
 * @author dvos
 */
public class ParsedMethodHeaderUT {

	Name className = new Name("TestClassName", false);
	
	
	@Test
	public void testParseMethodHeader() {
		String constructorMethodName = "<init>";
		String description = "()V";
		ClassfileMethodHeader parseMethodHeader = new ClassfileMethodHeader(className, constructorMethodName, description);
		
		System.out.println("parseMethodHeader="+parseMethodHeader);
	}
	
}
