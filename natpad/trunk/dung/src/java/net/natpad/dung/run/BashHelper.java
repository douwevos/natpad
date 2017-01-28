package net.natpad.dung.run;


public class BashHelper {

	static boolean isHtml = false;
	
	public static void printLine(String rawline) {
		int idx = rawline.indexOf("warning");
		if (idx>=0) {
			System.err.print(rawline.substring(0, idx));
			System.err.print((char) 27);
			if (isHtml) {
				System.err.print("<font color=\"003377\">");
			} else {
				System.err.print("[1;94m");
			}
			System.err.print(rawline.substring(idx));
			System.err.print((char) 27);
			if (isHtml) {
				System.err.print("</font>");
			} else { 
				System.err.println("[0m");
			}
		} else { 
			idx = rawline.indexOf("error");
			if (idx>=0) {
				System.err.print(rawline.substring(0, idx));
				System.err.print((char) 27);
				System.err.print("[1;31m");
				System.err.print(rawline.substring(idx));
				System.err.print((char) 27);
				System.err.println("[0m");
			} else {
				System.err.println(rawline);
			}
		}
		
	}

	
	
	
}
