package net.natpad.dung.hill.model;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import net.natpad.dung.hill.task.Task;
import net.natpad.dung.workspace.Module;
import net.natpad.sht.model.BlockValue;
import net.natpad.sht.model.Reference;

public class WorkspaceDescr implements Iterable<Task> {

	public String name;

	private ArrayList<Task> tasks = new ArrayList<Task>();

	
	public Map<String, BlockValue> references = new HashMap<String, BlockValue>();

	public List<Reference> templates = new ArrayList<Reference>();

	public Map<String, Module>  modules = new HashMap<String, Module>();
	
	@Override
	public String toString() {
		return "Workspace [name=" + name + "]";
	}

	public void addTask(Task task) {
		tasks.add(task);
	}
	
	
	@Override
	public Iterator<Task> iterator() {
		return tasks.iterator();
	}
	
	
}
