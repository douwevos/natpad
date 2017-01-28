package net.natpad.monkey;

import static net.natpad.caterpillar.CatLogger.cat_log_debug;

import javax.swing.JFrame;
import javax.swing.JScrollPane;

import net.natpad.caterpillar.CatFixedString;
import net.natpad.monkey.model.MooEditableContentMap;
import net.natpad.monkey.model.MooModuleContent;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.MooResourceContent;
import net.natpad.monkey.ui.MonkeyExplorer;
import net.natpad.monkey.ui.NodeRenderRegistry;
import net.natpad.monkey.ui.action.ActionGroup;
import net.natpad.monkey.ui.action.AddModuleHandler;
import net.natpad.viper.VipCreatePathRequest;
import net.natpad.viper.ViperService;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.VipSnapshot;
import net.natpad.viper.model.access.VipInvalidPath;
import net.natpad.viper.model.access.VipNodePath;
import net.natpad.viper.model.access.VipPath;
import net.natpad.viper.model.scanner.VipNodeScanner;
import net.natpad.worm.WormService;

public class MonkeyExplorerTest extends JFrame {

	
	
	public MonkeyExplorerTest() {
	}
	

	
	
	public static void main(String[] args) {
		
		
		System.setProperty("awt.useSystemAAFontSettings","on");
		System.setProperty("swing.aatext", "true");
		  
		ViperService viperService = new ViperService();

		WormService wormService = new WormService();
		MonkeyService monkeyService = new MonkeyService(wormService, viperService);

		
		viperService.start();
		VipPath fullpath = new VipPath(new CatFixedString("/home/superman/cpp-workspace/trunk/avaj"));
		VipCreatePathRequest createPathRequest = viperService.createPath(fullpath);
		VipNodePath nodePath = createPathRequest.wait_for_path();
		nodePath.print(System.out);
		
		VipNode tail = nodePath.getTail();
		tail.recursiveUp();
		
		viperService.scan();
		

		
		AddModuleHandler addModuleHandler = new AddModuleHandler(monkeyService, nodePath);
		viperService.post(addModuleHandler);

		addModuleHandler.waitForDone();
		
//		try {
//			Thread.sleep(2000);
//		} catch (InterruptedException e1) {
//			// TODO Auto-generated catch block
//			e1.printStackTrace();
//		}
		

		
//		
//		
//		MooEditableContentMap contentMap = new MooEditableContentMap(null);
//		MooResourceContent resourceContent = new MooResourceContent(tail);
//		contentMap.set(resourceContent);
//		MooModuleContent moduleContent = new MooModuleContent(tail);
//		contentMap.set(moduleContent);
//		MooNode module = new MooNode(monkeyService.uniqueIdCounter.incrementAndGet(), tail.getName(), null, contentMap.finalizeResult(), 50);
//		monkeyService.addModule(module);

		
		MonkeyExplorerTest monkeyExplorerTest = new MonkeyExplorerTest();
		
		
		ActionGroup actionGroup = new ActionGroup(monkeyService);


//		try {
//			Thread.sleep(10000);
//		} catch (InterruptedException e1) {
//			// TODO Auto-generated catch block
//			e1.printStackTrace();
//		}

		NodeRenderRegistry renderRegistry = new NodeRenderRegistry();
		
		MonkeyExplorer monkeyExplorer = new MonkeyExplorer(monkeyService, renderRegistry, actionGroup);
		
		
		JScrollPane scrollPane = new JScrollPane(monkeyExplorer);
		scrollPane.getVerticalScrollBar().setUnitIncrement(16);
		monkeyExplorerTest.getContentPane().add(scrollPane);
		
		scrollPane.setSize(500,500);
		
		monkeyExplorerTest.setSize(500,500);
		monkeyExplorerTest.setVisible(true);
		
		
		try {
			Thread.sleep(10000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		cat_log_debug("modifying snapshot");

		
		
		
	}
	
}
