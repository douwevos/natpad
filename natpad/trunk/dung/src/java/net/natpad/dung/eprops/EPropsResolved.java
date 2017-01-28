package net.natpad.dung.eprops;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class EPropsResolved {

	String START = "[%";
	String END = "%]";

	public final EPropsContext context;
	public final IEPropertyResolver propertyResolver;
	
	public int[] lastContextVersion;
	
	private HashMap<String, EPropertyItem> resolved = new HashMap<String, EPropertyItem>();
	
	public EPropsResolved(EPropsContext context, IEPropertyResolver propertyResolver) {
		this.context = context;
		this.propertyResolver = propertyResolver;
	}

	
	public String resolveProperty(String key) {
		ensureResolved();
		EPropertyItem propertyItem = resolved.get(key);
		String value = propertyItem==null ? null : propertyItem.getValue();
//		log.debug("resolveProperty:key="+key+" value="+value);
		return value;
	}


	private void ensureResolved() {
		int[] version = context.getVersion();
		if (lastContextVersion==version) {
			return;
		}
		resolveAll();
		lastContextVersion = version;
	}


	public String replaceProperties(String text) {
		ensureResolved();
		Map<String, EPropertyItem> dependencyMapping = new HashMap<String, EPropertyItem>();
		EPropertyItem propertyItem = new EPropertyItem("[%this%]", text);
		propertyItem.reset();
		if (resolveItem(propertyResolver, dependencyMapping, propertyItem)) {
			return propertyItem.getValue();
		}
		
		return propertyItem.getValue();
	}

	

	private void resolveAll() {
		resolved.clear();
		EPropsContext rcontext = context;
		while(rcontext!=null) {
//			System.out.println("rc="+rcontext);
			for(String key : rcontext.keys()) {
				if (resolved.containsKey(key)) {
					continue;
				}
				String uval = rcontext.get(key);
//				System.out.println("rc="+rcontext+", key="+key+", uval="+uval);
				EPropertyItem ePropertyItem = new EPropertyItem(key, uval);
				ePropertyItem.reset();
				resolved.put(key, ePropertyItem);
			}
			rcontext = rcontext.parent;
		}
		Map<String, EPropertyItem> dependencyMapping = new HashMap<String, EPropertyItem>();
		Set<String> runSet = new HashSet<String>();
		runSet.addAll(resolved.keySet());
		
		while(!runSet.isEmpty()) {
			
			boolean resolvedSomething = false;
			for(String key : runSet) {
				EPropertyItem dependItem = dependencyMapping.get(key);
				if (dependItem!=null && !dependItem.isResolved()) {
					continue;
				}
				EPropertyItem propertyItem = resolved.get(key);
				if (resolveItem(propertyResolver, dependencyMapping, propertyItem)) {
//					System.out.println("resolved:"+propertyItem);
					resolvedSomething = true;
				}
			}
			
			if (resolvedSomething) {
				runSet.clear();
				runSet.addAll(dependencyMapping.keySet());
			} else {
				runSet.clear();
			}
		}
		
		
		if (!dependencyMapping.isEmpty()) {
//			log("EProps: WARN not all dependencies resolved", Project.MSG_DEBUG);
			for(String key : dependencyMapping.keySet()) {
//				log("key:"+key+" depends on "+dependencyMapping.get(key), Project.MSG_DEBUG);
			}
		}
	}
	
	
	
	
	private boolean resolveItem(IEPropertyResolver propertyResolver, Map<String, EPropertyItem> dependencyMapping, EPropertyItem propertyItem) {
		String text = propertyItem.getValue();
		boolean keepSubRunning = true;
		int idxStart = 0;
		
//		System.out.println("text="+text);
		if (text == null) {
			propertyItem.setValue(null, true);
			return true;
		}
//		log.debug("being:text="+text);
		
		while(keepSubRunning) {
			keepSubRunning = false;
			idxStart = text.indexOf(START, idxStart);
			if (idxStart>=0) {
				int idxEnd = text.indexOf(END, idxStart);
				if (idxEnd>=0) {
					idxStart = text.lastIndexOf(START, idxEnd);
					String propName =  text.substring(idxStart+START.length(), idxEnd);
					if (propName.charAt(0)=='=') {
						String rtext = propertyResolver.resolveExpression(propName.substring(1));
						text = text.substring(0, idxStart) + rtext + text.substring(idxEnd+END.length());
//		System.err.println("new text="+text);
						keepSubRunning = true;		
						idxStart = 0;
						
					} else {
						
						EPropertyItem child = resolved.get(propName);
//						System.out.println("propName="+propName+", child="+child);
						if (child==null) {
							idxStart = idxEnd+END.length();
	//						log.debug("EProps: WARN possible missing property:"+propName);
							keepSubRunning = true;
						} else if (child.isResolved()) {
							text = text.substring(0, idxStart) + child.getValue() + text.substring(idxEnd+END.length());
	//		System.err.println("new text="+text);
							keepSubRunning = true;		
							idxStart = 0;
						} else {
							dependencyMapping.put(propertyItem.getKey(), child);
							propertyItem.setValue(text, false);
							return false;
						}
					}
				}
			}
		}
//		log.debug("end:text="+text);
		
		dependencyMapping.remove(propertyItem.getKey());
		propertyItem.setValue(text, true);
		return true;
	}
	
}
