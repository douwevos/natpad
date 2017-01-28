package net.natpad.dung.module.model;

import java.util.ArrayList;
import java.util.List;

import net.natpad.dung.module.task.Task;

public class TargetDescr implements TaskContainerDescr {

	private List<Task> taskList = new ArrayList<Task>();

	@Override
	public void addTask(Task task) {
//		System.out.println("addint task:"+task);
		taskList.add(task);
	}

	public List<Task> getTasks() {
		return taskList;
	}

}
