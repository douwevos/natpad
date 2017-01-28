package net.natpad.jaguar;


import net.natpad.jaguar.viper.model.archive.VipJarFileMapper;
import net.natpad.viper.ViperService;
import net.natpad.viper.model.VipSnapshot;
import net.natpad.viper.model.scanner.VipMapperRegistry;

public class ViperServiceUT {

	
	public static void main(String[] args) {
		
		
		
		ViperService viperService = new ViperService();
		VipMapperRegistry mapperRegistry = viperService.getMapperRegistry();
		mapperRegistry.addMapper(new VipJarFileMapper(viperService));
		viperService.start();
		
		VipSnapshot snapshot = viperService.getSnapshot();
		snapshot.print(System.out);
		
//		
//		VipNodePath nodePath;
//		try {
//			VipPath fullpath = new VipPath(new CatFixedString("/home/superman/cpp-workspace/natpad/natpad.cup/"));
//			nodePath = viperService.open(fullpath);
//			CatLoggerPrintStream out = new CatLoggerPrintStream(CatLogger.default_instance, CatLogLevels.CAT_LOG_ERROR);
//			nodePath.print(out);
//			
////			
////			VipNode tail = nodePath.getTail();
////			
////			
////			tail.print("", System.out);
////			
////			tail.setScanRecursive(true);
////			
////			
////			VipNodeScanner scanner = new VipNodeScanner(viperService.getMapperRegistry());
////			snapshot = viperService.getSnapshot();
////			VipNode newRoot = scanner.scan(snapshot.root);
////			viperService.setSnapshot(snapshot.set_root(newRoot));
////
////			snapshot = viperService.getSnapshot();
////			snapshot.print(System.out);
////
////			
////			
////			
////			scanner = new VipNodeScanner(viperService.getMapperRegistry());
////			snapshot = viperService.getSnapshot();
////			newRoot = scanner.scan(snapshot.root);
////			viperService.setSnapshot(snapshot.set_root(newRoot));
////
////			snapshot = viperService.getSnapshot();
////			snapshot.print(System.out);
//
//			
//		} catch (VipInvalidPath e) {
//			e.printStackTrace();
//		}
		
		
//		ViperNodeScanner nodeScanner = new ViperNodeScanner(viperService);
//		List<ViperNode> path = nodeScanner.createPath(snapshot.root, "src/main/java/");
//		ViperNode javanode = path.get(path.size()-1);
//		cat_log_debug("srcNode="+javanode);
//		
//		javanode.print("", System.out);
	}
	
	
	
	/* use case 1:
	 *   indexer runs over files while user modifies content of file
	 *   
	 *   
	 * use case 2:
	 *   indexer runs over files while user deletes file
	 *   
	 *     the structural content changed
	 *   
	 */
	
}
