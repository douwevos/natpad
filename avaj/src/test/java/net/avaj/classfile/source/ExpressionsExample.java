package net.avaj.classfile.source;

public class ExpressionsExample {

//	
//	public int doAIntegerAnd(int a, int b) {
//		return a & b;
//		
//	}
//
//	public int doAIntegerOr(int a, int b) {
//		return a | b;
//		
//	}
//	

	
//               0 | 0000 | 1B                  iload_1
//               1 | 0001 | 06                  iconst_3
//               2 | 0002 | A0 00 09            if_icmpne           lab0
//               3 | 0005 | 1C                  iload_2
//               4 | 0006 | 10 06               bipush              6
//               5 | 0008 | 9F 00 09            if_icmpeq           lab1
//       lab0: 6, 000B     [::5, 2::]
//               6 | 000B | 1B                  iload_1
//               7 | 000C | 10 06               bipush              6
//               8 | 000E | A0 00 05            if_icmpne           lab2
//       lab1: 9, 0011     [::5, 8::]
//               9 | 0011 | 04                  iconst_1
//              10 | 0012 | AC                  ireturn
//       lab2: 11, 0013     [::8::]
//              11 | 0013 | 03                  iconst_0
//              12 | 0014 | AC                  ireturn
	
	public boolean doAInteger2Or(int a, int b) {
		if ((a==3 && b==6) || (a==6)) {
			return true;
		}
		return false;
		
	}


//              0 | 0000 | 1B                  iload_1
//              1 | 0001 | 06                  iconst_3
//              2 | 0002 | 9F 00 09            if_icmpeq           lab0
//              3 | 0005 | 1C                  iload_2
//              4 | 0006 | 10 06               bipush              6
//              5 | 0008 | A0 00 0B            if_icmpne           lab1
//      lab0: 6, 000B     [::5, 2::]
//              6 | 000B | 1B                  iload_1
//              7 | 000C | 10 06               bipush              6
//              8 | 000E | A0 00 05            if_icmpne           lab1
//              9 | 0011 | 04                  iconst_1
//             10 | 0012 | AC                  ireturn
//      lab1: 11, 0013     [::5, 8::]
//             11 | 0013 | 03                  iconst_0
//             12 | 0014 | AC                  ireturn
	public boolean doAInteger3Or(int a, int b) {
		if ((a==3 || b==6) && (a==6)) {
			return true;
		}
		return false;
		
	}

	
	
	public boolean testBooleanBitwiseOr(boolean a, boolean b) {
		return (a | b);
	}

	public int testIntBitwiseOr(int a, int b) {
		return (a | b);
	}


	public long testLongBitwiseOr(long a, long b, long c) {
		return (a | b | c);
	}

	

	public boolean testBooleanBitwiseAnd(boolean a, boolean b) {
		return (a & b);
	}	

	
	
	public boolean testBooleanLogicalAnd(boolean a, boolean b) {
		return (a && b);
	}	

	
	public boolean testBooleanLogicalOr(boolean a, boolean b) {
		return (a || b);
	}	


	public boolean testBooleanMixedLogical(boolean a, boolean b, boolean c, boolean d) {
		return (a || b) && (c || d);
	}	

	public boolean testBooleanMixedLogical2(boolean a, boolean b, boolean c, boolean d) {
		return (a && b) || (c && d);
	}	

	
}
