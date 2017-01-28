package net.natpad.concord.ui;

import net.natpad.concord.Configuration;

public abstract class ConPanel {

	public void addNotify() {
	}
	
	public void removeNotify() {
	}
	
	public abstract void reconfigure(Configuration new_configuration);

	
}
