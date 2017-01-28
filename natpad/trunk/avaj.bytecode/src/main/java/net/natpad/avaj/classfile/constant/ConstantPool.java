package net.natpad.avaj.classfile.constant;

import java.util.ArrayList;
import java.util.Iterator;

import net.natpad.avaj.log.LogLevel;
import net.natpad.avaj.log.Logger;

public class ConstantPool implements Iterable<IConstant>, IConstantProvider {

	static Logger log = Logger.getInstance(IConstant.class);
	
	private final ArrayList<IConstant> constantList = new ArrayList<IConstant>();
	
	
	private int thisClassPoolIndex = -1;
	
	public ConstantPool() {
	}
	
	
	public void setThisClassPoolIndex(int thisClassPoolIndex) {
		this.thisClassPoolIndex = thisClassPoolIndex;
	}
	
	@Override
	public IConstant get(int poolIndex) {
		return getResolved(poolIndex);
	}
	
	@Override
	public String getConstantUtf8Text(int index) {
		ConstantUtfText utf8 = (ConstantUtfText) getResolved(index);
		return utf8.text;
	}
	
	
	public IConstant getUnresolved(int index) {
		return constantList.get(index);
	}

	public IConstant getResolved(int index) {
		IConstant constant = constantList.get(index);
		resolve(constant);
		return constant;
	}


	public void add(IConstant constant) {
		constantList.add(constant);
	}
	
	public boolean resolve(IConstant constant) {
		if (constant.isResolved()) {
			return true;
		}
		
		ArrayList<IConstant> resolveStack = new ArrayList<IConstant>();
		resolveStack.add(constant);
		while(!resolveStack.isEmpty()) {
			IConstant stacktConstant = resolveStack.get(resolveStack.size()-1);
			boolean wasResolved = stacktConstant.isResolved();
			if (!wasResolved) {
				wasResolved = stacktConstant.tryResolve(this, resolveStack);
			}
			if (wasResolved) {
				resolveStack.remove(resolveStack.size()-1);
			}
		}
		return false;
	}
	
	public ConstantClassInfo getThisClassConstant() {
		return (ConstantClassInfo) constantList.get(thisClassPoolIndex-1);
	}
	
	
	@Override
	public Iterator<IConstant> iterator() {
		return new Iterator<IConstant>() {
			
			Object list[] = constantList.toArray();
			int index=0;
			
			@Override
			public boolean hasNext() {
				return index<list.length;
			}
			@Override
			public IConstant next() {
				return (IConstant) list[index++];
			}
			@Override
			public void remove() {
				throw new UnsupportedOperationException();
			}
		};
	}


	public void dump(StringBuilder buf) {
		if (constantList.size()==0) {
			buf.append("constant-pool:EMPTY").append(Logger.ln);
			return;
		}
		int index=1;
		buf.append("constant-pool:size="+constantList.size()).append(Logger.ln);
		buf.append("-----------------------------------------------------------------------").append(Logger.ln);
		for(IConstant constant : constantList) {
			buf.append(index+" :: "+constant).append(Logger.ln);
			index++;
		}
		buf.append("-----------------------------------------------------------------------").append(Logger.ln);
	}


	public void dump() {
		if (log.logEnabbledFor(LogLevel.DEBUG)) {
			StringBuilder buf = new StringBuilder();
			dump(buf);
			log.debug(buf.toString());
		}
	}
	
}
