package net.natpad.avaj.ast;

public enum Modifier { 
		
		
		PUBLIC("public",100, 0x0001), 
		PRIVATE("private", 100, 0x0002),
		PROTECTED("protected",100, 0x0004),
		STATIC("static",90, 0x0008),
		FINAL("final", 95, 0x0010),
		SYNCHRONIZED("synchronized", 60, 0x0020),
		VOLATILE("volatile", 80, 0x0040),
		TRANSIENT("transient",80, 0x0080),
		NATIVE("native",70, 0x100),
		INTERFACE("interface",20, 0x200)
		;
	
		public final String text;
		public final int priority;
		public final int mask;
	
		Modifier(String text, int priority, int mask) {
			this.text = text;
			this.priority = priority;
			this.mask = mask;
		}
	}