package net.natpad.avaj.vm.type;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;

import net.natpad.avaj.ast.Argument;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.ImportStatement;
import net.natpad.avaj.ast.Name;
import net.natpad.avaj.classfile.ClassfileMethod;
import net.natpad.avaj.classfile.ClassfileMethodHeader;
import net.natpad.avaj.classfile.ParsedExcpetions;
import net.natpad.avaj.log.Logger;

public class ImportRepository {

	static Logger log = Logger.getInstance(ImportRepository.class);
	
	private final Map<String, ImportNameEntry> map = new HashMap<String, ImportNameEntry>();
	
	public final Name mainTypeName;
	
	public ImportRepository(Name mainTypeName) {
		this.mainTypeName = mainTypeName;
	}

	
	
	public Name createResolvedTypename(Name referenceTypeName) {
		Name result = referenceTypeName;
		String shortTypeName = referenceTypeName.getShortTypeName();
		ImportNameEntry importNameEntry = map.get(shortTypeName);
		if (importNameEntry!=null) {
			if (importNameEntry.allowShortVersion()) {
				result = new Name(shortTypeName, false);
			}
		}
		return result;
	}

	
	public Name add(Name fullName) {
		String shortTypeName = fullName.getShortTypeName();
		ImportNameEntry importNameEntry = map.get(shortTypeName);
		log.debug("shortTypeName="+shortTypeName);
		if (importNameEntry==null) {
			importNameEntry = new ImportNameEntry(shortTypeName);
			map.put(shortTypeName, importNameEntry);
		}
		return importNameEntry.getOrAdd(fullName);
	}

	
	
	public ImportStatement[] createStatements() {
		Name javaLangPackage = new Name("java.lang", false);
		ArrayList<ImportStatement> result = new ArrayList<ImportStatement>();
		for(String key : map.keySet()) {
			if (key.equals(mainTypeName)) {
				continue;
			}
			ImportNameEntry importNameEntry = map.get(key);
			if (importNameEntry.allowShortVersion()) {
				Name importName = importNameEntry.getFirstName();
				if (importName.startsWith(javaLangPackage)) {
					continue;
				}
				result.add(new ImportStatement(importName));
			}
			
//			Identifier name[] = typeHolder.getFullName();
//			result.add(new ImportStatement(name));
		}
		Collections.sort(result, new ImportStatementComparator());
		
		ImportStatement output[] = new ImportStatement[result.size()];
		result.toArray(output);
		return output;
	}


	public void attach(ClassfileMethod parsedMethod) {
		attach(parsedMethod.methodHeader);
	}

	
	public void attach(ClassfileMethodHeader methodHeader) {
		if (methodHeader.returnType!=null) {
			attach(methodHeader.returnType);
		}
		for(Argument argument : methodHeader.getArgs()) {
			attach(argument.declarationType);
		}
		
		if (methodHeader.getParsedExcpetions()!=null) {
			attach(methodHeader.getParsedExcpetions());
		}
	}

	public void attach(ParsedExcpetions parsedExcpetions) {
		for(Name name : parsedExcpetions) {
			add(name);
		}
		
	}

	public DeclarationType attach(DeclarationType declarationType) {
		if (declarationType.isReference()) {
			Name importedName = add(declarationType.referenceTypeName);
			declarationType = new DeclarationType(importedName, declarationType.dimCount);
		}
		return declarationType;
	}

	
	
//	public static class TypeImport {
//		
//		public final TypeReference typeReference;
//		
//		public TypeImport(TypeReference typeReference) {
//			this.typeReference = typeReference;
//		}
//		
//		public Identifier createUsage() {
//			return new Identifier(typeReference.typeName);
//		}
//	}
	
	

	public DeclarationType createFinalDeclarationType(DeclarationType bestType) {
		DeclarationType result = bestType;
		if (bestType.isReference()) {
			Name resolvedTypename = createResolvedTypename(bestType.referenceTypeName);
			if (resolvedTypename != bestType.referenceTypeName) {
				result = new DeclarationType(resolvedTypename, bestType.dimCount);
			}
		}
		return result;
	}
	
	
	static class ImportStatementComparator implements Comparator<ImportStatement> {
		@Override
		public int compare(ImportStatement o1, ImportStatement o2) {
			return o1.compare(o2);
		}
		
	}
}
