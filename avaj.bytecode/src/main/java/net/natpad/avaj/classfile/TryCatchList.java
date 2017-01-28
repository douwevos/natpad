package net.natpad.avaj.classfile;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.classfile.attribute.AttributeCode.AttributeCodeException;
import net.natpad.avaj.classfile.constant.ConstantClassInfo;
import net.natpad.avaj.classfile.constant.IConstantProvider;

public class TryCatchList implements Iterable<TryCatch> {

	private List<TryCatch> internalList;
	
	
	public TryCatchList(AttributeCodeException exceptionTable[], IConstantProvider constantProvider) {
		ArrayList<AttributeCodeException> todoList = new ArrayList<AttributeCodeException>();
		todoList.addAll(Arrays.asList(exceptionTable));

		internalList = new ArrayList<TryCatch>();
		while(!todoList.isEmpty()) {
			AttributeCodeException main = null;
			List<ICatchOrFinally> catchList = new ArrayList<ICatchOrFinally>();
			for(int idx=0; idx<todoList.size(); idx++) {
				AttributeCodeException exception = todoList.get(idx);
				if (exception.handlerPc==exception.startPc) {
					todoList.remove(idx);
					idx--;
					continue;
				}
				if (main==null) {
					main = todoList.remove(idx);
					idx--;
				} else if (main.startPc==exception.startPc && main.endPc==exception.endPc) {
					todoList.remove(idx--);
				} else {
					continue;
				}
				
				if (exception.catchType<=0) {
					catchList.add(new ClassfileFinally(exception.handlerPc));
				} else {
					ConstantClassInfo constantClass = (ConstantClassInfo) constantProvider.get(exception.catchType-1);
					DeclarationType descriptor = constantClass.getParsedDescriptor();
					catchList.add(new ClassfileCatch(descriptor, exception.handlerPc));
				}
			}
			
			internalList.add(new TryCatch(main.startPc, main.endPc, catchList));
		}
		
		
		Collections.sort(internalList, new Comparator<TryCatch>() {
			@Override
			public int compare(TryCatch o1, TryCatch o2) {
				if (o1.start<o2.start) {
					return -1;
				}
				
				if (o1.start>o2.start) {
					return 1;
				}
				
				if (o1.end<o2.end) {
					return 1;
				}

				if (o1.end>o2.end) {
					return -1;
				}

				return 0;
			}
		});
	}

	public void dump(PrintStream out) {
		int idx=0;
		for(TryCatch tryCatch : internalList) {
			out.print(idx++);
			out.print(" :: ");
			tryCatch.dump(out);
		}
	}

	
	public int count() {
		return internalList.size();
	}
	
	public TryCatch get(int index) {
		return internalList.get(index);
	}
	
	@Override
	public Iterator<TryCatch> iterator() {
		return new Iterator<TryCatch>() {
			int index = 0;
			@Override
			public boolean hasNext() {
				return index<internalList.size();
			}
			@Override
			public TryCatch next() {
				return internalList.get(index);
			}
			@Override
			public void remove(){
				throw new UnsupportedOperationException();
			}
		};
	}

	
}
