package net.avaj.classfile.source;

import net.avaj.classfile.source.extra.LevelOne;
import net.avaj.classfile.source.extra.LevelTwo;

public class ConstructorExample {

	
	int v;
	
	public LevelOne createAObject() {
		
		LevelTwo levelTwo = new LevelTwo(24, 12);
		return levelTwo;
	}

	
	public LevelOne createBObject() {
		
		int s = new LevelTwo(24, 12).getLevelTwo();
		v = s;
		return null;
	}

	
}
