package net.natpad.viper;

import java.util.LinkedList;

public class VipQueue implements Runnable {

	private volatile boolean keepRunning = true;
	
	private LinkedList<VipIRequest> queue = new LinkedList<VipIRequest>();
	
	
	public VipQueue() {
		Thread thread = new Thread(this);
		thread.setDaemon(true);
		thread.start();
	}
	
	
	public synchronized void post(VipIRequest request) {
		queue.addLast(request);
		notifyAll();
	}
	
	@Override
	public void run() {
		while(keepRunning) {
			VipIRequest request = null;
			synchronized(this) {
				if (queue.isEmpty()) {
					try {
						wait(5000l);
					} catch (InterruptedException ignore) {
					}
				} else {
					request = queue.poll();
				}
			}
			
			if (request!=null) {
				request.run();
			}
		}
		
	}
}
