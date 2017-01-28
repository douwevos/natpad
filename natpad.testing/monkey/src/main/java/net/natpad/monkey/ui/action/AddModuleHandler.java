package net.natpad.monkey.ui.action;

import net.natpad.caterpillar.versionedtree.CatReadableTreeNode;
import net.natpad.monkey.MonkeyService;
import net.natpad.monkey.model.MooEditableContentMap;
import net.natpad.monkey.model.MooModuleContent;
import net.natpad.monkey.model.MooNode;
import net.natpad.monkey.model.MooResourceContent;
import net.natpad.viper.VipIRequest;
import net.natpad.viper.model.VipNode;
import net.natpad.viper.model.access.VipNodePath;
import net.natpad.worm.Condition;
import net.natpad.worm.IConditionObserver;
import net.natpad.worm.Request;
import net.natpad.worm.WormService;

public class AddModuleHandler extends Request implements VipIRequest, IConditionObserver {

	private MonkeyService monkeyService;
	private VipNodePath nodePath;
	private MooNode module;
	
	private Condition added_condition;
	
	public AddModuleHandler(MonkeyService monkeyService, VipNodePath nodePath) {
		this.monkeyService = monkeyService;
		this.nodePath = nodePath;
		added_condition = new Condition();
		added_condition.registerSelector(this);
	}

	@Override
	public void run() {
		if (module==null) {
			VipNode tail = nodePath.getTail();
			CatReadableTreeNode<VipNode> tail_node = nodePath.getTailNode();
			
			tail_node = monkeyService.getViperService().getSnapshot().refresh(tail_node);
			
			
			
			MooEditableContentMap contentMap = new MooEditableContentMap(null);
			MooResourceContent resourceContent = new MooResourceContent(tail_node);
			contentMap.set(resourceContent);
			MooModuleContent moduleContent = new MooModuleContent(tail_node);
			contentMap.set(moduleContent);
			module = new MooNode(monkeyService.uniqueIdCounter.incrementAndGet(), tail.getName(), null, contentMap.finalizeResult(), 50);
			WormService worm_service = monkeyService.getWormService();
			worm_service.postRequest(this);
		}
	}
	
	@Override
	public void runRequest() {
//		boolean didAddModule = 
				monkeyService.add_node(module);
		added_condition.up();
	}
	
	
	
	volatile boolean done = false;
	
	@Override
	public synchronized void conditionReporting() {
		done = true;
		notifyAll();
	}
	
	
	public synchronized void waitForDone() {
		while(!done) {
			try {
				wait(2000l);
			} catch (InterruptedException e) {
			}
		}

	}
}
