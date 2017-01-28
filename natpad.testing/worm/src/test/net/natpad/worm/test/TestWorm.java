package net.natpad.worm.test;

import net.natpad.worm.WormService;

public class TestWorm {

	
	public TestWorm() {
		
		WormService service = new WormService();
		service.postRequest(new DispatchingRequest(service));
//		service.postRequest(new TestRequest(600l));
//		service.postRequest(new TestRequest(500l));
//		service.postRequest(new TestRequest(1300l));
//		service.postRequest(new TestRequest(1100l));
//		service.postRequest(new TestRequest(200l));
//		service.postRequest(new TestRequest(700l));
//		service.postRequest(new TestRequest(400l));
//		service.postRequest(new TestRequest(1000l));
//		service.postRequest(new TestRequest(1200l));
//		service.postRequest(new TestRequest(800l));
//		service.postRequest(new TestRequest(900l));
//		
//		
		
	}
	
	public static void main(String[] args) {
		new TestWorm();
	}
	
}
