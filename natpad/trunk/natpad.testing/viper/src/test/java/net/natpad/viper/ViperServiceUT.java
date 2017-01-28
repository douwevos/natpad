package net.natpad.viper;

import net.natpad.caterpillar.CatFixedString;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.VipSnapshot;
import net.natpad.viper.model.access.VipNodePath;
import net.natpad.viper.model.access.VipPath;

public class ViperServiceUT {

	
	public static void main(String[] args) {
		
		final ViperService viperService = new ViperService();
		viperService.start();
		
		VipSnapshot snapshot = viperService.getSnapshot();
		snapshot.print(System.out);
		
		
		try {

			VipPath m1path = new VipPath(new CatFixedString("/home/superman/cpp-workspace/trunk/natpad.cup/src/main/java/net/natpad/cup/ParseReduceTable.java"));
			System.out.println("m1path="+m1path);
			VipCreatePathRequest createPathRequest1 = viperService.createPath(m1path);
			VipNodePath path1 = createPathRequest1.wait_for_path();
			VipNode tail1 = path1.getTail();
			tail1.holdUp();
			
			viperService.queue.post(new VipIRequest() {
				
				@Override
				public void run() {
					System.out.println("hiero !!!!!!!!!!!!!");
					VipSnapshot snapshot = viperService.getSnapshot();
					snapshot.print(System.out);
				}
			});

			
			
			VipPath mpath = new VipPath(new CatFixedString("/home/superman/cpp-workspace/trunk/natpad.cup/"));
			System.out.println("mpath="+mpath);
			VipCreatePathRequest createPathRequest = viperService.createPath(mpath);
			VipNodePath path = createPathRequest.wait_for_path();
			VipNode tail = path.getTail();

			tail.holdUp();

			tail.recursiveUp();
			System.out.println("tail="+tail);

			viperService.scan();


			
			viperService.queue.post(new VipIRequest() {
				
				@Override
				public void run() {
					VipSnapshot snapshot = viperService.getSnapshot();
					snapshot.print(System.out);
				}
			});
			
			
			Thread.sleep(500000l);
//
//			
//			
//			VipNodeScanner scanner = new VipNodeScanner(viperService.mapperRegistry);
//			snapshot = viperService.getSnapshot();
//			VipNode newRoot = scanner.scan(snapshot.root);
//			viperService.setSnapshot(snapshot.set_root(newRoot));
//			snapshot = viperService.getSnapshot();
//			snapshot.print(System.out);
//			
//			
//			
//			open.getTail().getObserverInfo().unreference();
//
//			
//			scanner = new VipNodeScanner(viperService.mapperRegistry);
//			snapshot = viperService.getSnapshot();
//			newRoot = scanner.scan(snapshot.root);
//			viperService.setSnapshot(snapshot.set_root(newRoot));
//			snapshot = viperService.getSnapshot();
//			snapshot.print(System.out);
//
//		
		} catch(Exception e) {
			e.printStackTrace();
		}
		
	}
	
	
}
