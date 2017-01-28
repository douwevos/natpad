package net.avaj.classfile.source;

public class FieldDeclarationExample {

	public FieldDeclarationExample refToThisClass;
	
	public int simpleIntValue;
	
	@SuppressWarnings("unused")
	private int privateIntValue;

	protected int protectedIntValue;
	
	int defaultIntValue;

	public static int publicStaticIntValue;
	
	public volatile int publicVolatileIntValue;

	public transient int publicTransientIntValue;

	
	public final int publicFinalIntValue = 40;
	
	public final boolean publicFinalBoolValue = true;

	
	public final long publicFinalLongValue = 50;
	
	public final String publicFinalStringValue = "help";
	
	@SuppressWarnings("rawtypes")
	public final Class publicFinalClassValue = FieldDeclarationExample.class;
}
