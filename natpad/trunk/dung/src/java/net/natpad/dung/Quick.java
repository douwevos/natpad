package net.natpad.dung;

import java.util.regex.Pattern;

public class Quick {

	public static void main(String[] args) {
		boolean matches = Pattern.matches("ashare/glib-2.0/.*", "share/glib-2.0/codegen/config.pyc");
		System.out.println("matches="+matches);
	}
	
}
