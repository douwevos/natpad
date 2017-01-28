package net.avaj.classfile.source;

import net.avaj.classfile.source.extra.LList;

public class ComplexLoopsExample extends LList {

//	public LList Sort_Short_LList(CompareFunction fn) {
//		LList out_list = this;
//		LList l = this;
//		LList in_list = l.next;
//		l.next = null;
//		while (in_list != null) { /*
//								 * scan through the in list, performing an
//								 * insertion sort into the out list
//								 */
//			LList current_list = in_list;
//			Object current_item = current_list.contents;
//			LList scan_list = out_list;
//			LList prev_scan_list = null;
//			in_list = in_list.next;
//			while (scan_list != null
//					&& !fn.InOrder(current_item, scan_list.contents)) {
//				prev_scan_list = scan_list;
//				scan_list = scan_list.next;
//			}
//			current_list.next = scan_list;
//			if (prev_scan_list != null) {
//				prev_scan_list.next = current_list;
//			} else {
//				out_list = current_list;
//			}
//		}
//		return (out_list);
//	}
//	
	
//	public LList Sort_Short_LList2(CompareFunction fn) {
//		ComplexLoopsExample out_list = this;
//		return out_list;
//	}
	
	
	
	public static void main(String[] args) {
		for(String arg : args) {
			System.out.println(arg);
		}
	}
	

//    public static void main2(String[] arg0) {
//        String[] L4 = arg0;
//        int L3 = arg0.length;
//        int L2 = 0;
//        while (L2<L3) {
//            String L1 = L4[++L2];
//            System.out.println(L1);
//        }
//    }
	
}
