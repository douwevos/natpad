package net.natpad.sht.mapper;

import java.lang.annotation.Annotation;
import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;

import net.natpad.sht.model.BlockValue;
import net.natpad.sht.model.ListValue;
import net.natpad.sht.model.MapValue;
import net.natpad.sht.model.Model;
import net.natpad.sht.model.Reference;
import net.natpad.sht.model.SimpleValue;
import net.natpad.sht.model.Value;

public class Mapper {

	
	private Reference firstRefByName(Iterable<Reference> references, String name) {
		for(Reference ref : references) {
//			System.out.println("ref.refId="+ref.refId);
			if (ref.refId.equalsIgnoreCase(name)) {
				return ref;
			}
		}
		return null;
	}

	
	public <T> T map(Iterable<Reference> refs, Class<T> tclass, HashSet<Reference> referencedUsed) throws Exception {
		if (refs==null) {
			return null;
		}
		
		T newInstance = tclass.newInstance();
		
		mapFields(refs, tclass, newInstance, referencedUsed);
		mapAddMethods(refs, tclass, newInstance, referencedUsed);
		return newInstance;
	}

	protected<T> void mapAddMethods(Iterable<Reference> refs, Class<T> tclass, T newInstance, HashSet<Reference> referencedUsed) throws Exception {
		Method[] declaredMethods = tclass.getDeclaredMethods();
		for (Method method : declaredMethods) {
			String name = method.getName();
			if (name.startsWith("add")) {
				name = name.substring(3);
				Class<?>[] parameterTypes = method.getParameterTypes();
				if (parameterTypes.length!=1) {
					continue;
				}
				Class<?> ftype = parameterTypes[0];
				boolean isSimple = ftype.isPrimitive() || ftype==String.class;
				
				for(Reference ref : refs) {
					if (ref.refId.equalsIgnoreCase(name)) {
						Value value = ref.getValue();
						boolean valIsSimple = (value instanceof SimpleValue);
						if (valIsSimple!=isSimple) {
							continue;
						}
						
						Object rval = valueToObject(value, ftype);
						method.invoke(newInstance, rval);
						if (referencedUsed!=null) {
							referencedUsed.add(ref);
						}
					}
				}
			}

			
		}
		
	}

	
	protected Object valueToObject(Value value, Class<?> ftype) throws Exception {
		if (value==null) {
			return null;
		}

		if (ftype.isEnum()) {
			if (value instanceof SimpleValue) {
				String iv = ((SimpleValue) value).getValue();
				Object[] enumConstants = (Object[]) ftype.getEnumConstants();
				for(Object os : enumConstants) {
					Class<? extends Object> ec = os.getClass();
					Method declaredMethod = ec.getSuperclass().getDeclaredMethod("name");
					String ecName = (String) declaredMethod.invoke(os);
					if (iv.equalsIgnoreCase(ecName)) {
						return os;
					}
				}
				return null;
			} else if (value!=null) {
				throw new RuntimeException("Not a simple value");
			}
		} else if (ftype==int.class) {
			if (value instanceof SimpleValue) {
				Integer iv = Integer.valueOf(((SimpleValue) value).getValue());
				return iv;
			} else if (value!=null) {
				throw new RuntimeException("Not a simple value");
			}
		} else if (ftype==float.class) {
			if (value instanceof SimpleValue) {
				Float iv = Float.valueOf(((SimpleValue) value).getValue());
				return iv;
			} else if (value!=null) {
				throw new RuntimeException("Not a simple value");
			}
		} else if (ftype==boolean.class) {
			if (value instanceof SimpleValue) {
				Boolean iv = Boolean.valueOf(((SimpleValue) value).getValue());
				return iv;
			} else if (value!=null) {
				throw new RuntimeException("Not a simple value");
			}
		} else if (ftype==String.class) {
			if (value instanceof SimpleValue) {
				return ((SimpleValue) value).getValue();
			} else if (value!=null) {
				throw new RuntimeException("Not a simple value");
			}
		} else {
			if (value instanceof BlockValue) {
				Object n = map((BlockValue) value, ftype, null);
				return n;
			} else if (value instanceof ListValue) {
				Object n = map((ListValue) value, ftype, null);
				return n;

			} else if (value!=null) {
//				System.out.println("value="+value);
				throw new RuntimeException("Not a block value");
			}
			
		}

		return null;
	}

	private Object map(ListValue listValue, Class<?> ftype, Object referencedUsed) throws Exception {
		
		if (ftype.isArray()) {
			Class<?> componentType = ftype.getComponentType();
			Object result = Array.newInstance(componentType, listValue.count());
			int index = 0;
			for(Value value : listValue) {
				Object arrayObject = valueToObject(value, componentType);
				Array.set(result, index++, arrayObject);
			}
			return result;
//		} else if (List.class.isAssignableFrom(ftype)) {
//			List result = null;
//			if (ftype.equals(List.class)) {
//				result = (List) new ArrayList();
//			} else {
//				result = (List) ftype.newInstance();
//			}
//
//			
//			
//			Class<?> componentType = ftype.getComponentType();
//			
//			System.out.println("componentType="+ftype.);
//			
//			for(Value value : listValue) {
//				Object arrayObject = valueToObject(value, componentType);
//				result.add(arrayObject);
//			}
//			return result;
		}

		throw new RuntimeException("can not map array to non array field");
	}

	
	protected List<Field> enlistFields(Class<?> tclass, HashSet<String> addedNames) {
		if (addedNames==null) {
			addedNames = new HashSet<String>();
		}
		ArrayList<Field> result = new ArrayList<Field>();
		Field[] declaredFields = tclass.getDeclaredFields();
		for (Field field : declaredFields) {
			String fname = field.getName();
			if (addedNames.add(fname)) {
				result.add(field);
			}
		}
		MapParent annotation = tclass.getAnnotation(MapParent.class);
		if (annotation!=null) {
			List<Field> enlistFields = enlistFields(tclass.getSuperclass(), addedNames);
			result.addAll(enlistFields);
		}
		return result;
	}
	
	protected <T> void mapFields(Iterable<Reference> refs, Class<T> tclass, T newInstance, HashSet<Reference> referencedUsed) throws Exception {
		
		List<Field> declaredFields = enlistFields(tclass, null);

		for (Field field : declaredFields) {
			String name = field.getName();
			field.setAccessible(true);
//			System.out.println("field.name="+name);
			Class<?> ftype = field.getType();
			
			
			Reference refByName = firstRefByName(refs, name);
			if (refByName==null && "defaultValue".equals(name)) {
				refByName = firstRefByName(refs, "default");
//				System.out.println("default="+refByName);
			}
			
			if (refByName!=null) {
				Object val = valueToObject(refByName.getValue(), ftype);
				field.set(newInstance, val);
				if (referencedUsed!=null) {
					referencedUsed.add(refByName);
				}
			}
		}
	}


	public Model unmap(Object object) throws Exception {
		Model model = new Model();
		unmap(object, model);
		return model;
	}

	
	public void unmap(Object object, MapValue mapValue) throws Exception {
		Class<? extends Object> tclass = object.getClass();
		
		List<Field> declaredFields = enlistFields(tclass, null);
		for (Field field : declaredFields) {
			String name = field.getName();
			field.setAccessible(true);
			Class<?> ftype = field.getType();
			Object object1 = field.get(object);
//			System.out.println("name="+name+", object1="+object1);
			unmapField(mapValue, field, name, object1);
		}
	}


	protected void unmapField(MapValue mapValue, Field field, String name, Object object) throws Exception {
		if (object instanceof List) {
			Annotation[] declaredAnnotations = field.getDeclaredAnnotations();
			for (Annotation annotation : declaredAnnotations) {
//					System.out.println("annotation="+annotation);
			}
			MapFlat mapFlat = field.getAnnotation(MapFlat.class);
//				System.out.println("mapFlat="+mapFlat);
			if (mapFlat!=null) {
				String mapAsName = mapFlat.value();
				List<Object> ll = (List<Object>) object;
				for (Object object2 : ll) {
					Value unmapedChild = unmapObjectToValue(object2);
					Reference ref = new Reference(mapAsName);
					ref.setValue(unmapedChild);
					mapValue.add(ref);

				}
				return;
			}
		}
		if (object instanceof Map) {
			MapFlat mapFlat = field.getAnnotation(MapFlat.class);
			if (mapFlat!=null) {
				String mapAsName = mapFlat.value();
				Map<Object, Object> map = (Map<Object, Object>) object;
				for(Object ko : map.keySet()) {
					String key = ko.toString();
					Object value = map.get(ko);
					Value unmapedChild = unmapObjectToValue(value);
					Reference ref = new Reference(key);
					ref.setValue(unmapedChild);
					mapValue.add(ref);
				}
				return;
			}
		}
		Value value = unmapObjectToValue(object);
		if (value!=null) {
			Reference ref = new Reference(name);
			ref.setValue(value);
			mapValue.add(ref);
		}
	}


	private Value unmapObjectToValue(Object object) throws Exception {
		if (object == null) {
			return null;
		}
		Class<? extends Object> oclass = object.getClass();
//		System.out.println("oclass="+oclass);
		if (oclass.isEnum()) {

//			System.out.println("sc---"+oclass.getSuperclass().getName());
//
//			Method[] declaredMethods = oclass.getSuperclass().getDeclaredMethods();
//			for (Method method : declaredMethods) {
//				System.out.println("---"+method.getName());
//			}
			Method declaredMethod = oclass.getSuperclass().getDeclaredMethod("name");
			String name = (String) declaredMethod.invoke(object);
			return new SimpleValue(name);
		} else if (oclass.isPrimitive()) {
			return new SimpleValue(""+object); 
		} else if (oclass==String.class) {
			return new SimpleValue(object.toString());
		} else if (object instanceof List) {
			List<Object> ll = (List<Object>) object;
			ListValue lv = new ListValue();
			for (Object object2 : ll) {
				Value unmapedChild = unmapObjectToValue(object2);
				lv.add(unmapedChild);
			}
			return lv;
		} else if (object instanceof Map) {
			Map<Object, Object> ll = (Map<Object, Object>) object;
			BlockValue bv = new BlockValue();
			for (Object key : ll.keySet()) {
				Object kval = ll.get(key);
				Value unmapedChild = unmapObjectToValue(kval);
				Reference mref = new Reference(key.toString());
				mref.setValue(unmapedChild);
				bv.add(mref);
			}
			return bv;
		} else {
			BlockValue blockValue = new BlockValue();
			unmap(object, blockValue);
			return blockValue;
		}
	}
	
	
}
