package net.natpad.jaguar.monkey;

import static net.natpad.caterpillar.CatFixedString.cat_fixed_string_new;
import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import java.util.ArrayList;
import java.util.HashMap;

import net.natpad.monkey.MonkeyService;
import net.natpad.monkey.model.MooEditableContentMap;
import net.natpad.monkey.model.MooEditableNode;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.MooResourceContent;
import net.natpad.monkey.viper.MooINodeWork;
import net.natpad.monkey.viper.MooModuleWork;
import net.natpad.viper.ViperService;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.VipSnapshot;
import net.natpad.viper.model.access.VipInvalidPath;
import net.natpad.viper.model.access.VipNodePath;
import net.natpad.viper.model.access.VipPath;
import net.natpad.viper.model.scanner.VipNodeScanner;
import net.natpad.worm.WormService;

public class JagMooseContext implements JagICompilerMap {


	public final MonkeyService monkeyService;
	private HashMap<String, MooNode> jreMap = new HashMap<String, MooNode>();

	

	
	
	public JagMooseContext(MonkeyService monkeyService) {
		this.monkeyService = monkeyService;

		
//		MooEditableContentMap contentMap = new MooEditableContentMap(null);
//		JagNodeCompiler jagCompilerContent = new JagNodeCompiler(monkeyService.uniqueIdCounter.incrementAndGet());
//		contentMap.set(jagCompilerContent);
//		MooNode jagNodeCompiler = new MooNode(monkeyService.uniqueIdCounter.incrementAndGet(), "java", null, contentMap.finalizeResult(), 180); 
//		MooEditableNode editableCompilerNode = jagNodeCompiler.ensureEditable();
//		
//		
//		ViperService viperService = monkeyService.getViperService();
//		try {
//			VipPath fullpath = new VipPath(cat_fixed_string_new("/opt/jdk1.7.0_17/jre/lib/"));
//			VipNodePath open = viperService.open(fullpath);
//			VipNode tail = open.getTail();
//			tail.setScanRecursive(true);
//			
//
//			VipNodeScanner scanner = new VipNodeScanner(viperService.getMapperRegistry());
//			VipSnapshot snapshot = viperService.getSnapshot();
//			VipNode newRoot = scanner.scan(snapshot.root);
//			viperService.setSnapshot(snapshot.set_root(newRoot));
//			
//			
//			fullpath = new VipPath(cat_fixed_string_new("/opt/jdk1.7.0_17/jre/lib/rt.jar"));
//			open = viperService.open(fullpath);
//			tail = open.getTail();
//			cat_log_debug("tail = "+tail.getClass().getName()+":"+tail.getContent().getClass().getName()+":"+tail.getName());
////			viperService.getSnapshot().print(System.out);
////			System.exit(1);
//
//			
//			MooEditableContentMap subContentMap = new MooEditableContentMap(null);
//			MooResourceContent subResContent = new MooResourceContent(tail);
//			subContentMap.set(subResContent);
//			
//			MooNode subNode = new MooNode(monkeyService.uniqueIdCounter.incrementAndGet(), tail.getName(), null, subContentMap.finalizeResult(), 100);
//			
//			editableCompilerNode.addNewChild(subNode);
//			MooEditableNode editableChild = editableCompilerNode.getEditableChild(0);
//			
//			
//			cat_log_debug("tail.childcount="+tail.childCount());
//			
//			MooModuleWork moduleWork = new MooModuleWork(monkeyService, tail, editableChild);
//			ArrayList<MooINodeWork> worklist = new ArrayList<MooINodeWork>();
//			worklist.add(moduleWork);
//			int idx = 5000;
//			while(!worklist.isEmpty()) {
//				MooINodeWork work = worklist.remove(worklist.size()-1);
//				work.doWork(worklist);
//				cat_log_debug("doing work:"+work);
////				if (idx--<0) {
////					break;
////				}
//			}
////			
//////			System.exit(1);
//			
//		} catch (VipInvalidPath e) {
//			e.printStackTrace();
//		}
//		synchronized (jreMap) {
//			jreMap.put("java", editableCompilerNode.finalizeResult());
//		}
		
	}

	
	
	@Override
	public MooNode getCompilerNode(String name) {
		synchronized (jreMap) {
			return jreMap.get(name);
		}
	}



	public WormService getWormService() {
		return monkeyService.getWormService();
	}

}
