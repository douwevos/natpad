package net.natpad.worm.test;

import net.natpad.worm.IRequestService;
import net.natpad.worm.Request;

public class TestRequest extends Request {

	public long s;
	
	public TestRequest(long timeOut) {
		setTimeOut(timeOut+System.currentTimeMillis());
	}
	
	@Override
	public void runRequest() {
//		System.out.println("run request");
		for(long i=0; i<1000000; i++) {
			s = s+i*(1+s/10);
		}
	}
	
}
