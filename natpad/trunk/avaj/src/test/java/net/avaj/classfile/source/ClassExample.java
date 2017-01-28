package net.avaj.classfile.source;

import java.util.Iterator;
import java.util.Vector;

import net.avaj.classfile.source.extra.INiceInterface;

@SuppressWarnings("serial")
public class ClassExample implements INiceInterface 
//extends Vector<String> implements INiceInterface, Iterable<String> 
{
	
	public ClassExample() {
	}

//	@Override
	public INiceInterface[] beNice() {
		INiceInterface result[][] = new INiceInterface[1][2];
		result[0][2] = this;
//		result[1][1] = result[1][2];
		return result[0];
	}
	
	@Override
	public void beSilent() {
//		super.trimToSize();
	}
//	
//	
//	@Override
//	public synchronized void trimToSize() {
//		super.trimToSize();
//	}
//	
//	@Override
//	public Iterator<String> iterator() {
//		return super.iterator();
//	}
}
