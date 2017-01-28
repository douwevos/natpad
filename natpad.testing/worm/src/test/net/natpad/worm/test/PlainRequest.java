package net.natpad.worm.test;

import net.natpad.worm.Request;
import net.natpad.worm.WormService;

public class PlainRequest extends Request {

	String msg;
	long s;
	
	public PlainRequest(String msg) {
		this.msg = msg;
	}
	
	
	@Override
	public void runRequest() {
		for(long i=0; i<10000000; i++) {
			s = s+i*(1+s/10);
		}
//		try {
//			Thread.sleep(50+(long) (Math.random()*200));
//		} catch (InterruptedException e) {
//		}

//		System.out.println(WormService.getTime()+"pr:"+msg);
	}
}
