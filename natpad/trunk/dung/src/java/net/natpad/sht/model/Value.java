package net.natpad.sht.model;

import java.io.PrintStream;

public abstract class Value {

	public abstract void dump(PrintStream out, String indent);

}
