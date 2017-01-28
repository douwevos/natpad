package net.natpad.monkey.ui.action;

import net.natpad.monkey.MonkeyService;


public class ActionGroup {

	
	private MonkeyService mooService;
	
	public MooActionAddModule actionAddModule;
	public MooActionDeleteResource actionDeleteResource;
	
	public ActionGroup(MonkeyService mooService) {
		this.mooService = mooService;
		actionAddModule = new MooActionAddModule(mooService);
		actionDeleteResource = new MooActionDeleteResource(mooService);
	}
	
}
