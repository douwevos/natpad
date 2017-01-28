package net.natpad.monkey.ui.action;

import java.awt.event.ActionEvent;

import net.natpad.monkey.MonkeyService;

public class MooActionDeleteResource extends javax.swing.AbstractAction {

	private static final long serialVersionUID = 4768136L;

	MonkeyService mooService;
	

	public MooActionDeleteResource(MonkeyService mooService) {
		super("Delete resource");
		this.mooService = mooService;
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		
		
	}

}
