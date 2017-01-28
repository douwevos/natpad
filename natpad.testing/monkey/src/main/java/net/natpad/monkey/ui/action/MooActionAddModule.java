package net.natpad.monkey.ui.action;

import java.awt.event.ActionEvent;

import net.natpad.monkey.MonkeyService;

public class MooActionAddModule extends javax.swing.AbstractAction {

	private static final long serialVersionUID = 4768136L;

	MonkeyService mooService;
	
	public MooActionAddModule(MonkeyService mooService) {
		super("Add module");
		this.mooService = mooService;
		
	}
	
	
	@Override
	public void actionPerformed(ActionEvent e) {
		AddModuleDialog addModuleDialog = new AddModuleDialog(mooService);
		addModuleDialog.setVisible(true);
	}
}
