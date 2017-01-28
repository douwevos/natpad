package net.avaj.classfile.source.extra;

public class LevelTwo extends LevelOne {

	
	
	public LevelTwo() {
	}
	
	
	public LevelTwo(int k, long s) {
		levelTwo = (int) (k+s);
	}
	
	int levelTwo;
	
	public int getLevelTwo() {
		return levelTwo;
	}

	
}
