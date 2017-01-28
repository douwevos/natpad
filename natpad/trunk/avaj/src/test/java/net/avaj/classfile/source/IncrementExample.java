package net.avaj.classfile.source;

public class IncrementExample {
//
//
////    0 | 0000 | 1B                  iload_1
////    1 | 0001 | 84 01 01            iinc                1,1
////    2 | 0004 | 3D                  istore_2
////    3 | 0005 | 1C                  iload_2
////    4 | 0006 | 1B                  iload_1
////    5 | 0007 | 60                  iadd
////    6 | 0008 | AC                  ireturn	
//	public int postfixIncrement(int i) {
//		int k = i++;
//		return k+i;
//	}
//	
//	
//	
////    0 | 0000 | 84 01 01            iinc                1,1
////    1 | 0003 | 1B                  iload_1
////    2 | 0004 | 3D                  istore_2
////    3 | 0005 | 1C                  iload_2
////    4 | 0006 | 1B                  iload_1
////    5 | 0007 | 60                  iadd
////    6 | 0008 | AC                  ireturn
//	public int prefixIncrement(int i) {
//		int k = ++i;
//		return k+i;
//	}

	
	public long postfixDecrement(long i) {
		long k = i--;
		return k+i;
	}
	
	

	public int prefixDecrement(int i) {
		int k = --i;
		return k+i;
	}

	
	public short forLoopIncrement(short s) {
		for(short t=0; t<s; t++) {
			s = (short) (s - 3);
		}
		return s;
	}

	
	public void arrayIncrement(int myarray[]) {
		myarray[3]++;
	}

	
//	idx = idx++ - ++idx;
//	idx =  ++idx - idx++;

}
