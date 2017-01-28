package net.natpad.worm;

public class Worker implements Runnable {

	static int nr = 1000;
	
	private final IWorkProvider provider;
	private final int workerId;
	
	public Worker(IWorkProvider provider) {
		this.provider = provider;
		workerId = nr++;
		Thread thread = new Thread(this);
		thread.start();
//		thread.setPriority(Thread.MAX_PRIORITY);
		
	}
	
	@Override
	public void run() {
		while(true) {
			Request request = provider.nextRequest();
			if (request!=null) {
//				System.out.println(WormService.getTime()+"worker["+workerId+"] request="+request);
				request.runRequest();
				
//				synchronized (this) {
//					try {
//						this.wait(400);
//					} catch (InterruptedException e) {
//						// TODO Auto-generated catch block
//						e.printStackTrace();
//					}
//				}
			}
		}
	}
	
}
