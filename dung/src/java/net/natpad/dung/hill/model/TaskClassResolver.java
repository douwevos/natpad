package net.natpad.dung.hill.model;

import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

import net.natpad.dung.DungClassLoader;
import net.natpad.dung.hill.task.Task;
import net.natpad.dung.run.Session;

public class TaskClassResolver {

	DungClassLoader classLoader;
	Map<String, Class<? extends Task>> map = new HashMap<String, Class<? extends Task>>();
	
	
	public TaskClassResolver(DungClassLoader classLoader) {
		this.classLoader = classLoader;
	}
	
	
	public Class<? extends Task> taskClassByName(String name) {
		Class<? extends Task> result = map.get(name);
		if (result==null) {
			result = findTaskClassByName(name);
			if (result!=null) {
				map.put(name, result);
			}
		}
		return result;
	}
	
	
	private Class<? extends Task> findTaskClassByName(String name) {
		char fch = name.charAt(0);
		if (Character.isLowerCase(fch)) {
			name = Character.toUpperCase(fch) + name.substring(1);
		}
		
		try {
			Class<?> forName = classLoader.loadClass("net.natpad.dung.hill.task."+name);
			
//			System.out.println("forName="+forName);
			
//			Class<?> forName = Class.forName("net.natpad.builder.module.task."+name);
			if (forName!=null) {
				if (Task.class.isAssignableFrom(forName)) {
					return (Class<? extends Task> ) forName;
				} else {
					final Method declaredMethod = forName.getDeclaredMethod("runTask", Session.class);
					if (declaredMethod!=null) {
//						return new Task() {
//							
//							@Override
//							public void runTask(Session session) throws Exception {
//								declaredMethod.invoke(session);
//							}
//						};
					}
				}
			}
		} catch (Throwable e) {
		}
		return null;
	}
	
}
