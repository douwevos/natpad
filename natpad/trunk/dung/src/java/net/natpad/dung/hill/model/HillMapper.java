package net.natpad.dung.hill.model;

import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

import net.natpad.dung.DungClassLoader;
import net.natpad.dung.hill.task.Task;
import net.natpad.dung.workspace.Module;
import net.natpad.dung.workspace.ModuleType;
import net.natpad.sht.mapper.Mapper;
import net.natpad.sht.model.BlockValue;
import net.natpad.sht.model.Reference;
import net.natpad.sht.model.SimpleValue;
import net.natpad.sht.model.Value;

public class HillMapper extends Mapper {

	public boolean full;
	
	public List<Reference> unresolvedTargets = new ArrayList<Reference>();
	
	TaskClassResolver resolver;
	
	public HillMapper(DungClassLoader classloader) {
		resolver = new TaskClassResolver(classloader);
	}

	
	
	@Override
	public <T> T map(Iterable<Reference> refs, Class<T> tclass, HashSet<Reference> referencesUsed) throws Exception {
		T result = null;
//		System.out.println("tclass="+tclass);
		if (tclass == WorkspaceDescr.class) {
			result = mapWorkspaceDefinition(refs, tclass, referencesUsed);
		} else {
			result = super.map(refs, tclass, referencesUsed);
		}

		return result;
	}

	private <T> T mapWorkspaceDefinition(Iterable<Reference> refs, Class<T> tclass, HashSet<Reference> referencesUsed)
			throws Exception {
		T result;
		if (referencesUsed==null) {
			referencesUsed = new HashSet<Reference>();
		}
		
		WorkspaceDescr workspaceDefinition = new WorkspaceDescr();
		
		ArrayList<Reference> refsFiltered = new ArrayList<Reference>();
		for(Reference reference : refs) {
			if ("reference".equals(reference.refId)) {
				referencesUsed.add(reference);
				BlockValue refbv = (BlockValue) reference.getValue();
				Reference refidref = refbv.getFirstReferenceById("refid");
				String refname = ((SimpleValue) refidref.getValue()).value;
				refbv.remove(refidref);
//				System.out.println("refbv="+refbv+",refname="+refname);
				workspaceDefinition.references.put(refname, refbv);
			} else if ("template".equals(reference.refId)) {
				referencesUsed.add(reference);
				workspaceDefinition.templates.add(reference);
			} else if ("module".equals(reference.refId)) {
				BlockValue tempRefs = (BlockValue) reference.getValue();
				Reference firstIdRef = tempRefs.getFirstReferenceById("name");
				String templateId = ((SimpleValue) firstIdRef.getValue()).getValue();
				Reference firstTypeRef = tempRefs.getFirstReferenceById("type");
				ModuleType type = (ModuleType) valueToObject(firstTypeRef.getValue(), ModuleType.class);

				Module module = map(tempRefs, type.moduleClass, null);
				
				workspaceDefinition.modules.put(templateId, module);
			} else {
				refsFiltered.add(reference);
			}
		}
		
		result = (T) workspaceDefinition;
		mapFields(refsFiltered, tclass, result, referencesUsed);
		mapAddMethods(refsFiltered, tclass, result, referencesUsed);

		
		
		for (Reference reference : refsFiltered) {
			if (referencesUsed.contains(reference)) {
				continue;
			}
			
//			System.out.println("reference="+reference);
			
			Value value = reference.getValue();
			if (value instanceof Iterable) {
				Iterable<Reference> targetRefs = (Iterable<Reference>) value;
				
				Class<? extends Task> taskClassByName = resolver.taskClassByName(reference.refId);
				if (taskClassByName==null) {
					throw new RuntimeException("no taks with name:"+reference.refId);
				}
				Task task = map(targetRefs, taskClassByName, null);
				workspaceDefinition.addTask(task);
			} else if (value instanceof SimpleValue) {
				SimpleValue sval = (SimpleValue) value;
				String text = sval.getValue();
				Class<? extends Task> taskClassByName = resolver.taskClassByName(reference.refId);
				if (taskClassByName!=null) {
					Constructor<? extends Task> constructor = taskClassByName.getDeclaredConstructor(String.class);
					Task task = (Task) constructor.newInstance(text);
					workspaceDefinition.addTask(task);
				}
			}
			
		}
		return result;
	}

}
