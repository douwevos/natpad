package net.natpad.dung.module.task;

import net.natpad.dung.run.Session;

public abstract class Task {

	public abstract void runTask(Session session) throws Exception;
}
