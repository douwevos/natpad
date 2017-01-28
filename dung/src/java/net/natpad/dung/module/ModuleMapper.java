package net.natpad.dung.module;

import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;

import net.natpad.dung.DungClassLoader;
import net.natpad.dung.Template;
import net.natpad.dung.module.model.ModuleDescr;
import net.natpad.dung.module.model.ModuleFileDescr;
import net.natpad.dung.module.model.TargetDescr;
import net.natpad.dung.module.model.TaskContainerDescr;
import net.natpad.dung.module.task.Task;
import net.natpad.dung.workspace.Workspace;
import net.natpad.sht.mapper.Mapper;
import net.natpad.sht.model.BlockValue;
import net.natpad.sht.model.ListValue;
import net.natpad.sht.model.MapValue;
import net.natpad.sht.model.Reference;
import net.natpad.sht.model.SimpleValue;
import net.natpad.sht.model.Value;

public class ModuleMapper extends Mapper {

	private final Workspace workspace;
	public boolean full;
	
	public List<Reference> unresolvedTargets = new ArrayList<Reference>();
	
	TaskClassResolver resolver;
	
	
	private Map<String, Template> templateMap = new HashMap<String, Template>();
	
	public ModuleMapper(DungClassLoader classloader, Workspace workspace) {
		resolver = new TaskClassResolver(classloader);
		this.workspace = workspace;
	}

	
	
	@Override
	public <T> T map(Iterable<Reference> refs, Class<T> tclass, HashSet<Reference> referencesUsed) throws Exception {

		if (tclass == ModuleFileDescr.class) {
			templateMap.clear();
			templateMap.putAll(workspace.templates);
			return mapModuleFileDescr(refs, tclass, referencesUsed);
		}

		
		ArrayList<Reference> filteredRefs = new ArrayList<Reference>();
		
		for(Reference ref : refs) {
			if ("reference".equals(ref.refId)) {
				Value refValue = ref.getValue();
				if (refValue instanceof SimpleValue) {
					String refName = ((SimpleValue) refValue).value;
					BlockValue repl = workspace.references.get(refName);
					if (repl!=null) {
						for(Reference r : repl) {
							filteredRefs.add(r);
						}
					}
//				} else if (refValue instanceof BlockValue) {
//					BlockValue bvContent = (BlockValue) refValue;
//					String refName
//					for(Reference cref : bvContent) {
//						String refName = ((SimpleValue) refValue).value;
//					}
				}
				if (referencesUsed!=null) {
					referencesUsed.add(ref);
				}
			} else {
				filteredRefs.add(ref);
			}
		}
		
		
		for(int idx=0; idx<filteredRefs.size(); idx++) {
			Reference ref = filteredRefs.get(idx);
			Template template = templateMap.get(ref.refId);
			if (template!=null) {
//				System.out.println("template found:"+template);
				BlockValue newRefs = (BlockValue) applyTemplate(template, template, ref);
				int oidx = idx;
				idx--;
				filteredRefs.remove(oidx);
				for(Reference repla : newRefs) {
					filteredRefs.add(oidx++, repla);
				}
				if (referencesUsed!=null) {
					referencesUsed.add(ref);
				}
			}
		}
		
		T result = null;
//		System.out.println("tclass="+tclass);
		if (tclass == ModuleDescr.class) {
			result = mapModuleDescr(filteredRefs, tclass, referencesUsed);
		} else if (tclass == TargetDescr.class || TaskContainerDescr.class.isAssignableFrom(tclass)) {
			result = mapTaskContainerDescr(filteredRefs, tclass, referencesUsed);
		} else {
			result = super.map(filteredRefs, tclass, referencesUsed);
		}

		return result;
	}

	private Value applyTemplate(Template template, Value val, Reference contentRef) {
		
		if (val instanceof MapValue) {
			MapValue smv = (MapValue) val;
			BlockValue result = new BlockValue();
			for(Reference sub : smv) {
				if ("content".equals(sub.refId)) {
					Value value = contentRef.getValue();
					if (value instanceof MapValue) {
						MapValue mvv = (MapValue) value;
						for(Reference mvRef : mvv) {
							result.add(mvRef);
						}
					}
				} else {
					Value value = sub.getValue();
					value = applyTemplate(template, value, contentRef);
					Reference repSub = new Reference(sub.refId);
					repSub.setValue(value);
					result.add(repSub);
				}
			}
			return result;
		} else if (val instanceof ListValue) {
			ListValue smv = (ListValue) val;
			ListValue result = new ListValue();
			for(Value sub : smv) {
				Value value = applyTemplate(template, sub, contentRef);
				result.add(value);
			}
			return result;
		}
		
		
		return val;
	}

	private <T> T mapModuleFileDescr(Iterable<Reference> refs, Class<T> tclass, HashSet<Reference> referencesUsed)
			throws Exception {
		ModuleFileDescr result = new ModuleFileDescr();
		for (Reference reference : refs) {
			if (referencesUsed!=null && referencesUsed.contains(reference)) {
				continue;
			}
			
			
			if ("module".equals(reference.refId)) {
				BlockValue modRefs = (BlockValue) reference.getValue();
				ModuleDescr module = mapModuleDescr(modRefs, ModuleDescr.class, referencesUsed);
				result.module = module; 
			} else if ("template".equals(reference.refId)) {
				BlockValue tempRefs = (BlockValue) reference.getValue();
				Reference firstIdRef = tempRefs.getFirstReferenceById("id");
				String templateId = ((SimpleValue) firstIdRef.getValue()).getValue();
				Template template = new Template(templateId);
				for(Reference ref : tempRefs) {
					if (ref!=firstIdRef) {
						template.add(ref);
					}
				}
				templateMap.put(templateId, template);
			}
		}
		
		
		return (T) result;
	}


	private <T> T mapModuleDescr(Iterable<Reference> refs, Class<T> tclass, HashSet<Reference> referencesUsed)
			throws Exception {
		T result;
		if (referencesUsed==null) {
			referencesUsed = new HashSet<Reference>();
		}
		result = super.map(refs, tclass, referencesUsed);
		
		ModuleDescr moduleDescr = (ModuleDescr) result;
		
		for (Reference reference : refs) {
			if (referencesUsed.contains(reference)) {
				continue;
			}
			
//			System.out.println("reference="+reference);
			
			Value value = reference.getValue();
			if (value instanceof Iterable) {
				
				unresolvedTargets.add(reference);
			} else if (value instanceof SimpleValue) {
				SimpleValue sv = (SimpleValue) value;
				moduleDescr.setProperty(reference.getId(), sv.value);
			}
			
		}
		return result;
	}
	

	private <T> T mapTaskContainerDescr(Iterable<Reference> refs, Class<T> tclass, HashSet<Reference> referencesUsed)
			throws Exception {
		T result;
		if (referencesUsed==null) {
			referencesUsed = new HashSet<Reference>();
		}
		result = super.map(refs, tclass, referencesUsed);
		TaskContainerDescr taskContainerDescr = (TaskContainerDescr) result;
		for (Reference reference : refs) {
			if (referencesUsed.contains(reference)) {
				continue;
			}
			
//			System.out.println("reference="+reference);
			
			Value value = reference.getValue();
			if (value instanceof Iterable) {
				Iterable<Reference> targetRefs = (Iterable<Reference>) value;
				
				Class<? extends Task> taskClassByName = resolver.taskClassByName(reference.refId);
				if (taskClassByName==null) {
					throw new RuntimeException("no tasks with name:"+reference.refId);
				}
				Task task = map(targetRefs, taskClassByName, null);
				taskContainerDescr.addTask(task);
			} else if (value instanceof SimpleValue) {
				SimpleValue sval = (SimpleValue) value;
				String text = sval.getValue();
				Class<? extends Task> taskClassByName = resolver.taskClassByName(reference.refId);
				if (taskClassByName==null) {
					throw new RuntimeException("no tasks with name:"+reference.refId);
				}
				Constructor<? extends Task> constructor = taskClassByName.getDeclaredConstructor(String.class);
				Task task = (Task) constructor.newInstance(text);
				taskContainerDescr.addTask(task);
			}
			
		}
		return result;
	}

	
	public void resolveTargets(ModuleDescr moduleDescr) throws Exception {
		for (Reference reference : unresolvedTargets) {
			Value value = reference.getValue();
			Iterable<Reference> targetRefs = (Iterable<Reference>) value;
			TargetDescr target = map(targetRefs, TargetDescr.class, null);
			moduleDescr.addTarget(reference.getId(), target);
		}
	}
	
	static class UnresolvedTarget {
		public final Iterable<Reference> refs;
		public final Class<?> tclass;
		
		public UnresolvedTarget(Iterable<Reference> refs, Class<?> tclass) {
			this.refs = refs;
			this.tclass = tclass;
		}
	}
	
}
