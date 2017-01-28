package net.natpad.cup.export.caterpillar;

public class CNameTupple {

	public final String lowerName;
	public final String upperName;

	public final String camelName;

	public CNameTupple(String name) {

		String[] split = name.split(",");
		lowerName = split[0];
		upperName = split[1];
		camelName = split[2];
	}
}
