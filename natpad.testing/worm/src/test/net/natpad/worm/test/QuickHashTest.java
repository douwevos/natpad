package net.natpad.worm.test;

public class QuickHashTest {

	static String TXT = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	static final int TXT_LEN = TXT.length();
	
	static String createCode(long p) {
		StringBuilder buf = new StringBuilder();
		buf.append(TXT.charAt((int) (p%TXT_LEN)));
		p /= TXT_LEN;
		buf.append(TXT.charAt((int) (p%TXT_LEN)));
		p /= TXT_LEN;
		buf.append(TXT.charAt((int) (p%TXT_LEN)));
		p /= TXT_LEN;
		buf.append(TXT.charAt((int) (p%TXT_LEN)));
		p /= TXT_LEN;
		buf.append(TXT.charAt((int) (p%TXT_LEN)));
		return buf.toString();
	}
	
	public static void main(String[] args) {
		
	}
	
	static {

		
		int code = createCode(0).hashCode();

		long rep = TXT_LEN*TXT_LEN*TXT_LEN*TXT_LEN*TXT_LEN;
		
		for(long s=0; s<rep; s++) {
			
			if ((s%1000000)==0) {
				System.out.println("s="+s);
			}
			
			String createdCode = createCode(s);
			if (createdCode.hashCode()==code) {
				System.out.println("s="+s+", createdCode="+createdCode);
			}
		}
		
	}
	
}
