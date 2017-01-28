package net.natpad.worm;

import java.util.ArrayList;

public class WormService implements Runnable, IWorkProvider {

	static long starttime = System.currentTimeMillis();
	
	public static String getTime() {
		return ""+(System.currentTimeMillis()-starttime)+" : ";
	}
	
	final Object consumerLock = new Object();
	final ArrayList<Request> defaultQueue = new ArrayList<Request>();
	final ArrayList<Request> waitList = new ArrayList<Request>();
	

	final Object dispatcherLock = new Object();
	final ArrayList<Request> dispatchQueue = new ArrayList<Request>();
	

	public WormService() {
		new Thread(this).start();
		
		for(int idx=0; idx<10; idx++) {
			Worker worker = new Worker(this);
		}
	}
	
	
	@Override
	public void run() {
		ArrayList<Request> transferList = new ArrayList<Request>();
		long timetowait;
		while(true) {
//			long timea = System.currentTimeMillis();
			synchronized(consumerLock) {
				timetowait = 300000l;
				if (!waitList.isEmpty()) {
					long timeNow = System.currentTimeMillis();
					for(int idx=0; idx<waitList.size(); idx++) {
						Request wrequest = waitList.get(idx);
						if (wrequest.getTimeOut()<=timeNow || wrequest.isNotified) {
							waitList.remove(idx);
							transferList.add(wrequest);
							wrequest.isNotified = false;
							idx--;
						} else if (idx==0) {
							timetowait = wrequest.getTimeOut()-timeNow;
							if (timetowait>300000) {
								timetowait = 300000;
							}
						}
					}
				}

				if (!defaultQueue.isEmpty()) {
					transferList.addAll(defaultQueue);
					defaultQueue.clear();
				}

				if (transferList.isEmpty()) {
					try {
						consumerLock.wait(timetowait);
					} catch (InterruptedException e) {
					}
				}
			}
			
//			long timeb = System.currentTimeMillis();
//			System.out.println(getTime()+"timeab:"+(timeb-timea));
			
			
			if (!transferList.isEmpty()) {
				for(Request req : transferList) {
					req.updateStates();
				}
				
				synchronized (dispatcherLock) {
					dispatchQueue.addAll(transferList);
					transferList.clear();
					dispatcherLock.notifyAll();
				}
			}

//			long timec = System.currentTimeMillis();

//			System.out.println(getTime()+"timebc:"+(timec-timeb));
			
		}
	}
	
	
	
	public void postRequest(Request request) {
		request.addNotify(consumerLock);
		synchronized(consumerLock) {
			if (request.hasTimeOut()) {
				long to = request.getTimeOut();
				for(int idx=0; idx<waitList.size(); idx++) {
					Request req = waitList.get(idx);
					if (req.getTimeOut()>to) {
						waitList.add(idx, request);
						request=null;
						idx = waitList.size();
					}
				}
				if (request!=null) {
					waitList.add(request);
				}
			} else {
				defaultQueue.add(request);
			}
			consumerLock.notifyAll();
		}
	}
	
	
	@Override
	public Request nextRequest() {
		Request result = null;
		synchronized(dispatcherLock) {
			long endTime = System.currentTimeMillis()+50000;
			while(result == null) {
				if (dispatchQueue.isEmpty()) {
					long diff = endTime-System.currentTimeMillis();
					if (diff<=0) {
						break;
					}
					try {
						dispatcherLock.wait(diff);
					} catch (InterruptedException e) {
					}
				} else {
					result = dispatchQueue.remove(0);
				}
			}
		}
		return result;
	}
	
}
