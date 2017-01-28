package net.natpad.cup.export.purec;

import java.io.File;

public class PureCConfiguration {

	protected File destDir = new File("./src/generated/caterpillar");

	
	
	
	/**
	 * User option -- do we compact tables by making most common reduce the
	 * default action
	 */
//	public boolean opt_compact_red = false;
	public boolean compactReduceDefault = false;

	
	
	public String namespacePrefix = "NatpadCup";
	
	
	public boolean outputNonTerminals = true;




	public boolean extraDebugInfo = true;




	public String debugLevel = "WARN";
	
}
