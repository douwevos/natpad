package net.natpad.avaj.classfile;

import java.io.PrintStream;
import java.util.Iterator;
import java.util.List;

import net.natpad.avaj.util.TextUtil;

public class TryCatch implements Iterable<ICatchOrFinally> {

	public final int start, end;
	private final ICatchOrFinally catchList[];
	
	
	public TryCatch(int start, int end, ICatchOrFinally ... catchList) {
		this.catchList = catchList;
		this.start = start;
		this.end = end;
	}
	
	public TryCatch(int start, int end, List<ICatchOrFinally> catchList) {
		this.catchList = new ICatchOrFinally[catchList.size()];
		catchList.toArray(this.catchList);
		this.start = start;
		this.end = end;
	}
	
	public int catchCount() {
		return catchList.length;
	}
	
	public ICatchOrFinally catchAt(int index) {
		return catchList[index];
	}


	
	@Override
	public Iterator<ICatchOrFinally> iterator() {
		return new Iterator<ICatchOrFinally>() {
			int index = 0;
			@Override
			public boolean hasNext() {
				return index<catchList.length;
			}
			
			@Override
			public ICatchOrFinally next() {
				return catchList[index++];
			}
			@Override
			public void remove() {
				throw new UnsupportedOperationException();
			}
		};
	}

	
	public void dump(PrintStream out) {
		out.println("TryCatch: start="+TextUtil.hex(start,2)+", end="+TextUtil.hex(end,2));
		for(ICatchOrFinally catchItem : catchList) {
			out.println("    "+catchItem);
		}
	}

	
}
