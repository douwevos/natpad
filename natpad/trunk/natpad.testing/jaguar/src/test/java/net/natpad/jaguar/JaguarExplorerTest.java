package net.natpad.jaguar;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JScrollPane;

import net.natpad.caterpillar.CatFixedString;
import net.natpad.caterpillar.CatLogLevel;
import net.natpad.caterpillar.CatLogLevels;
import net.natpad.jaguar.monkey.JagMooseConnector;
import net.natpad.jaguar.monkey.ui.jagNodeRendererFactory;
import net.natpad.jaguar.viper.model.archive.VipJarFileMapper;
import net.natpad.monkey.MonkeyService;
import net.natpad.monkey.ui.MonkeyExplorer;
import net.natpad.monkey.ui.NodeRenderRegistry;
import net.natpad.monkey.ui.action.ActionGroup;
import net.natpad.monkey.ui.action.AddModuleHandler;
import net.natpad.viper.VipCreatePathRequest;
import net.natpad.viper.ViperService;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.access.VipNodePath;
import net.natpad.viper.model.access.VipPath;
import net.natpad.viper.model.scanner.VipMapperRegistry;
import net.natpad.worm.WormService;

@CatLogLevel(CatLogLevels.CAT_LOG_TRACE)
public class JaguarExplorerTest extends JFrame {

	
	
	public JaguarExplorerTest() {
	}
	

	
	
	public static void main(String[] args) {
		
		
		System.setProperty("awt.useSystemAAFontSettings","on");
		System.setProperty("swing.aatext", "true");
		  
		ViperService viperService = new ViperService();
		VipMapperRegistry mapperRegistry = viperService.getMapperRegistry();
		mapperRegistry.addMapper(new VipJarFileMapper(viperService));
		
		
		WormService wormService = new WormService();
		MonkeyService monkeyService = new MonkeyService(wormService, viperService);
		
		
		viperService.start();
		
		

		viperService.start();
		VipPath fullpatha = new VipPath(new CatFixedString("/home/superman/cpp-workspace/trunk/natpad.cup/src/main/java/net/natpad/cup"));
		VipCreatePathRequest createPathRequest2 = viperService.createPath(fullpatha);
		VipNodePath nodePath2 = createPathRequest2.wait_for_path();

		
		VipPath fullpath = new VipPath(new CatFixedString("/home/superman/cpp-workspace/trunk/natpad.cup/"));
		VipCreatePathRequest createPathRequest = viperService.createPath(fullpath);
		VipNodePath nodePath = createPathRequest.wait_for_path();
		nodePath.print(System.out);
		
		VipNode tail = nodePath.getTail();
		tail.recursiveUp();
		
		viperService.scan();
		
		
		AddModuleHandler addModuleHandler = new AddModuleHandler(monkeyService, nodePath);
		viperService.post(addModuleHandler);

		addModuleHandler.waitForDone();
		
		
//		VipSnapshot snapshot = viperService.getSnapshot();
//		VipNodePath nodePath = null;;
//		try {
//			VipPath fullpath = new VipPath(new CatFixedString("/home/superman/cpp-workspace/trunk/natpad.cup/"));
//			nodePath = viperService.open(fullpath);
//			nodePath.print(CatLogger.logout);
//			
//			VipNode tail = nodePath.getTail();
//			
//			
//			tail.print("", CatLogger.logout);
//			tail.setScanRecursive(true);
//			
//			
//			VipNodeScanner scanner = new VipNodeScanner(viperService.getMapperRegistry());
//			snapshot = viperService.getSnapshot();
//			VipNode newRoot = scanner.scan(snapshot.root);
//			viperService.setSnapshot(snapshot.set_root(newRoot));
//			snapshot = viperService.getSnapshot();
//		} catch (VipInvalidPath e) {
//			e.printStackTrace();
//		}
//		
//		VipNode tail = nodePath.getTail();
//		
//		
//		MooEditableContentMap contentMap = new MooEditableContentMap(null);
//		MooResourceContent resourceContent = new MooResourceContent(tail);
//		contentMap.set(resourceContent);
//		MooModuleContent moduleContent = new MooModuleContent(tail);
//		contentMap.set(moduleContent);
//		MooNode module = new MooNode(monkeyService.uniqueIdCounter.incrementAndGet(), tail.getName(), null, contentMap.finalizeResult(), 50);
//		boolean didAddModule = monkeyService.addModule(module);
//
//		
//		cat_log_error("didAddModule="+didAddModule);
//		
		JagMooseConnector jagMonkeyNodeService = new JagMooseConnector(monkeyService);
		
		
		JaguarExplorerTest monkeyExplorerTest = new JaguarExplorerTest();
		
		ActionGroup actionGroup = new ActionGroup(monkeyService);

		JMenuBar menuBar = new JMenuBar();
		JMenu menuModule = new JMenu("Module");
		menuBar.add(menuModule);
		
		menuModule.add(actionGroup.actionAddModule);
		
		monkeyExplorerTest.setJMenuBar(menuBar);
		

//		try {
//			Thread.sleep(10000);
//		} catch (InterruptedException e1) {
//			// TODO Auto-generated catch block
//			e1.printStackTrace();
//		}

		NodeRenderRegistry renderRegistry = new NodeRenderRegistry();

		renderRegistry.addNodeRenderer(new jagNodeRendererFactory());
		
		MonkeyExplorer monkeyExplorer = new MonkeyExplorer(monkeyService, renderRegistry, actionGroup);
		
		
		JScrollPane scrollPane = new JScrollPane(monkeyExplorer);
		scrollPane.getVerticalScrollBar().setUnitIncrement(16);
		monkeyExplorerTest.getContentPane().add(scrollPane);
		
		scrollPane.setSize(500,500);
		
		monkeyExplorerTest.setSize(500,500);
		monkeyExplorerTest.setVisible(true);
		
		
		
		while(true) {
		try {
			Thread.sleep(10000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		

//		VipNodeScanner scanner = new VipNodeScanner(viperService.getMapperRegistry());
//		snapshot = viperService.getSnapshot();
//		VipNode newRoot = scanner.scan(snapshot.root);
//		snapshot = snapshot.set_root(newRoot);
//		viperService.setSnapshot(snapshot);
//
//		snapshot.print(CatLogger.logout);
//
//		
		
		cat_log_debug("modifying snapshot");

		}
		
		
	}
	
}
