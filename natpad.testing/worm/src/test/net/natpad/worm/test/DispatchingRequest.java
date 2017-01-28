package net.natpad.worm.test;

import net.natpad.worm.Condition;
import net.natpad.worm.Request;
import net.natpad.worm.WormService;

public class DispatchingRequest extends Request {

	private final WormService service;
	
	RequestCreator requestCreator;
	
	private DispatcherRequest pendingRequests[];
	
	
	
	public DispatchingRequest(WormService service) {
		this.service = service;
		int procs = (int) (Runtime.getRuntime().availableProcessors()*1.0d);
		if (procs<1) {
			procs = 1;
		}
		
		procs = 18;
		
		pendingRequests = new DispatcherRequest[procs];
		
		setTimeOut(0);
	}
	
	@Override
	public void runRequest() {
//		cat_log_debug(WormService.getTime()+"running dispatch");
		if (requestCreator==null) {
			requestCreator = new RequestCreator();
		}
		
		boolean hasActive = false;
		for(int idx=0; idx<pendingRequests.length; idx++) {
			DispatcherRequest prequest = pendingRequests[idx];
			if (prequest!=null) {
				if (testCondition(prequest.condition)) {
//					cat_log_debug("["+id+"] condition was up");
//					
//				}
//				if (prequest.hasFinished) {
					removeCondition(prequest.condition);
					prequest = null;
					pendingRequests[idx] = null;
				} else {
					hasActive = true;					
				}
			}
			
			if (prequest==null) {
				Request newreq = requestCreator.nextRequest();
				if (newreq!=null) {
					pendingRequests[idx] = new DispatcherRequest(newreq);
					addCondition(pendingRequests[idx].condition);
//					cat_log_debug(WormService.getTime()+"added pending");
					service.postRequest(pendingRequests[idx]);
					hasActive = true;					
				}
			}
		}
		
		if (hasActive) {
//			cat_log_debug(WormService.getTime()+"dispatcher still active:"+System.currentTimeMillis());
			setTimeOut(System.currentTimeMillis()+30000);
			service.postRequest(this);
		} else {
			System.out.println(WormService.getTime()+"dispatcher has finished");
		}
	}

	
	
	static class DispatcherRequest extends Request {
		
		Request delegate;
//		public volatile boolean hasFinished;
		public final Condition condition;
		
		public DispatcherRequest(Request request) {
			delegate = request;
//			hasFinished = false;
			condition = new Condition();
//			addCondition(condition);
		}
		
		
		@Override
		public void runRequest() {
			delegate.runRequest();
//			hasFinished = true;
			condition.up();
//			cat_log_debug("condition set up:"+condition);
		}
		
		@Override
		public long getTimeOut() {
//			if (delegate.hasTimeOut()) {
				return delegate.getTimeOut();
//			}
//			return Long.MAX_VALUE;
		}
		
		
		@Override
		public boolean hasTimeOut() {
			return delegate.hasTimeOut();
//			return true;
		}
	}
	
	
	static class RequestCreator {
		
		long creation;
		
		public RequestCreator() {
		}
		
		Request nextRequest() {
			if (creation<3000000) {
				creation++;
				return new PlainRequest("creation:"+creation+", num="+(int) ((2+Math.random())*100));
//				return new TestRequest((int) ((2+Math.random())*100));
			}
			return null;
		}
		
	}
	
}
