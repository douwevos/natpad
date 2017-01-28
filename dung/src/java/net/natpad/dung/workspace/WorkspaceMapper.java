package net.natpad.dung.workspace;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

import net.natpad.dung.Template;
import net.natpad.sht.mapper.Mapper;
import net.natpad.sht.model.BlockValue;
import net.natpad.sht.model.MapValue;
import net.natpad.sht.model.Reference;
import net.natpad.sht.model.SimpleValue;
import net.natpad.sht.model.Value;

public class WorkspaceMapper extends Mapper {

	
	@Override
	public <T> T map(Iterable<Reference> refs, Class<T> tclass, HashSet<Reference> referencedUsed) throws Exception {
		if (tclass==Workspace.class) {
			return (T) mapWorkspace(refs, tclass, referencedUsed);
		}
		return super.map(refs, tclass, referencedUsed);
	}
	

	public <T> T mapWorkspace(Iterable<Reference> refs, Class<T> tclass, HashSet<Reference> referencedUsed) throws Exception {
		T result = null;
		if (referencedUsed==null) {
			referencedUsed = new HashSet<Reference>();
		}

		
		HashMap<String, BlockValue> refMap = new HashMap<String, BlockValue>();
		HashMap<String, Template> templateMap = new HashMap<String, Template>();
		HashMap<String, Module> moduleMap = new HashMap<String, Module>();
		ArrayList<Reference> refsFiltered = new ArrayList<Reference>();
		for(Reference reference : refs) {
			if ("reference".equals(reference.refId)) {
				referencedUsed.add(reference);
				BlockValue refbv = (BlockValue) reference.getValue();
				Reference refidref = refbv.getFirstReferenceById("refid");
				String refname = ((SimpleValue) refidref.getValue()).value;
				refbv.remove(refidref);
				refMap.put(refname, refbv);

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
			} else if ("module".equals(reference.refId)) {
				BlockValue tempRefs = (BlockValue) reference.getValue();
				Reference firstIdRef = tempRefs.getFirstReferenceById("name");
				String templateId = ((SimpleValue) firstIdRef.getValue()).getValue();
				Reference firstTypeRef = tempRefs.getFirstReferenceById("type");
				ModuleType type = (ModuleType) valueToObject(firstTypeRef.getValue(), ModuleType.class);

				Module module = map(tempRefs, type.moduleClass, null);
				
				moduleMap.put(templateId, module);
			} else {
				refsFiltered.add(reference);
			}
		}
		
		result = super.map(refsFiltered, tclass, referencedUsed);
		Workspace ws = (Workspace) result;
		ws.references.putAll(refMap);
		ws.templates.putAll(templateMap);
		ws.modules.putAll(moduleMap);
		
		for(Reference ref : refs) {
			if (referencedUsed.contains(ref)) {
				continue;
			}
			Value val = ref.getValue();
			if (val instanceof SimpleValue) {
				SimpleValue sv = (SimpleValue) val;
				ws.putProperty(ref.refId, sv.value);
			}
		}
		
		return result;
	}

	
	@Override
	protected void unmapField(MapValue mapValue, Field field, String name, Object object) throws Exception {
//		System.out.println("wsmapper: name="+name);
		if ("references".equals(name)) {
			Map<String, BlockValue> referenceMap = (Map<String, BlockValue>) object;
			for(String refName : referenceMap.keySet()) {
//				System.out.println("wsmapper: refName="+refName);
				Reference referer = new Reference("refid");
				referer.setValue(new SimpleValue(refName));
				BlockValue bv = new BlockValue();
				bv.add(referer);
				BlockValue blockValue = referenceMap.get(refName);
				for(Reference bvk : blockValue) {
					bv.add(bvk);
				}
				
				Reference reference = new Reference("reference");
				reference.setValue(bv);
				mapValue.add(reference);
			}
		} else if ("templates".equals(name)) {
			Map<String, BlockValue> referenceMap = (Map<String, BlockValue>) object;
			for(String refName : referenceMap.keySet()) {
//				System.out.println("wsmapper: refName="+refName);
				Reference referer = new Reference("id");
				referer.setValue(new SimpleValue(refName));
				BlockValue bv = new BlockValue();
				bv.add(referer);
				BlockValue blockValue = referenceMap.get(refName);
				for(Reference bvk : blockValue) {
					bv.add(bvk);
				}
				
				Reference reference = new Reference("template");
				reference.setValue(bv);
				mapValue.add(reference);
			}
		} else if ("modules".equals(name)) {
			Map<String, Module> referenceMap = (Map<String, Module>) object;
			for(String refName : referenceMap.keySet()) {
//				System.out.println("wsmapper: refName="+refName);
				BlockValue bv = new BlockValue();
				Module blockValue = referenceMap.get(refName);
				
				unmap(blockValue, bv);
				
				Reference reference = new Reference("module");
				reference.setValue(bv);
				mapValue.add(reference);
			}
		} else {
			super.unmapField(mapValue, field, name, object);
		}
	}
	
	
}
