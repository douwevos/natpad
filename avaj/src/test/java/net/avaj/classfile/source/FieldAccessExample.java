package net.avaj.classfile.source;

public class FieldAccessExample {

	private static final String testValue = "welkom";
	
	public int plainIntField;

	public Object plainRefField;
	

	//    0 | 0000 | 2A                  aload_0
	//    1 | 0001 | B7 00 11            invokespecial       17
	//    2 | 0004 | B1                  return
	/* public FieldAccessExample() {} */
	
	
	/*    0 | 0000 | 2A                  aload_0
	      1 | 0001 | 10 0D               bipush              13
	      2 | 0003 | B5 00 19            putfield            25
	      3 | 0006 | 2A                  aload_0
	      4 | 0007 | B4 00 19            getfield            25
	      5 | 000A | AC                  ireturn
	 */
	public int testPrimitiveAccess() {
		plainIntField = 13;
		return plainIntField;
		
	}
	
	/*    0 | 0000 | 2A                  aload_0
	      1 | 0001 | 12 1D               ldc_w               29
	      2 | 0003 | B5 00 1F            putfield            31
	      3 | 0006 | 2A                  aload_0
	      4 | 0007 | B4 00 1F            getfield            31
	      5 | 000A | B0                  rreturn
	 */
	public Object testReferenceAccess() {
		plainRefField = "hey hallo";
		return plainRefField;
	}

	/*    0 | 0000 | 2A                  aload_0
	      1 | 0001 | 12 1D               ldc_w               29
	      2 | 0003 | B5 00 1F            putfield            31
	      3 | 0006 | 2A                  aload_0
	      4 | 0007 | 12 08               ldc_w               8
	      5 | 0009 | B5 00 1F            putfield            31
	      6 | 000C | 2A                  aload_0
	      7 | 000D | B4 00 1F            getfield            31
	      8 | 0010 | B0                  rreturn
	 */
	public Object testReferenceAccessRead() {
		plainRefField = "hey hallo";
		plainRefField = testValue;
		return plainRefField;
	}

}
