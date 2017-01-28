package net.natpad.dung.module;

import java.util.List;

import net.natpad.dung.module.model.TargetDescr;
import net.natpad.dung.module.task.Task;
import net.natpad.dung.run.Session;

public class Target {

	public final TargetDescr targetDescr;
	
	public Target(TargetDescr targetDescr) {
		this.targetDescr = targetDescr;
	}
	
	public void run(Session session) throws Exception {
		List<Task> taskList = targetDescr.getTasks();
//		System.out.println("taskList="+taskList);
		for (Task task : taskList) {
			task.invokeTask(session);
		}
	}
	
}
