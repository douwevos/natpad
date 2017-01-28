package net.natpad.avaj;

import java.util.ArrayList;
import java.util.List;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.FieldDeclaration;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.ast.ImportStatement;
import net.natpad.avaj.ast.Literal;
import net.natpad.avaj.ast.Method;
import net.natpad.avaj.ast.Name;
import net.natpad.avaj.ast.PackageStatement;
import net.natpad.avaj.ast.QuotedString;
import net.natpad.avaj.ast.Type;
import net.natpad.avaj.ast.TypeHeader;
import net.natpad.avaj.classfile.Classfile;
import net.natpad.avaj.classfile.ClassfileField;
import net.natpad.avaj.classfile.ClassfileMethod;
import net.natpad.avaj.classfile.bytecode.MnemonicBlock;
import net.natpad.avaj.classfile.constant.ConstantClassInfo;
import net.natpad.avaj.classfile.constant.ConstantPrimString;
import net.natpad.avaj.classfile.constant.ConstantPrimitive;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.vm.GroupConnector;
import net.natpad.avaj.vm.MethodContext;
import net.natpad.avaj.vm.ModelStageTwoRuntime;
import net.natpad.avaj.vm.subcode.block.SubCodeBlock;
import net.natpad.avaj.vm.type.ImportRepository;

public class Decompiler {

	static Logger log = Logger.getInstance(Decompiler.class);
	
	public final Classfile parsedClass;
	public final ImportRepository importRepository;
	
	public Decompiler(Classfile parsedClass) {
		this.parsedClass = parsedClass;
		importRepository = new ImportRepository(parsedClass.getMainTypeName());
	}
	
	

	

	public FieldDeclaration createFinal(ClassfileField parsedField, String localVarName) {
		IExpression fieldValue = null;
		if (parsedField.value!=null) {
			if (parsedField.value instanceof ConstantPrimitive) {
				ConstantPrimitive<?> cprim = (ConstantPrimitive<?>) parsedField.value;
				Object value = cprim.value;
				fieldValue = new Literal(value);
			} else if (parsedField.value instanceof ConstantPrimString) {
				ConstantPrimString cs = (ConstantPrimString) parsedField.value;
				fieldValue = new QuotedString(cs.getText());
			} else if (parsedField.value instanceof ConstantClassInfo) {
				ConstantClassInfo cci = (ConstantClassInfo) parsedField.value;
				String fqn = cci.getRefrenceTypeName().createFQN()+".class";
				fieldValue = new Identifier(fqn);
			} else {
				System.out.println("parsedField.value="+parsedField.value);
				RuntimeException exception = new RuntimeException();
				throw exception;
			}

		}
		if (localVarName==null) {
			localVarName = parsedField.name;
		}
		return new FieldDeclaration(parsedField.modifiers, importRepository.createFinalDeclarationType(parsedField.declarationType), new Identifier(localVarName), fieldValue);
	}
	
	
	public Type create() {
		
		PackageStatement packageStatement = null;
		Name packageName = parsedClass.getMainTypeName().createPackageName();
		if (packageName!=null) {
			packageStatement = new PackageStatement(new Identifier(packageName));
		}
		
		
		ClassfileField fieldList[] = parsedClass.getFields();
		for(int fieldIdx=0; fieldIdx<fieldList.length; fieldIdx++) {
			ClassfileField parsedField = fieldList[fieldIdx];
			importRepository.attach(parsedField.declarationType);
		}
		
		DeclarationType interfaceList[] = parsedClass.getInterfaces();
		for(int ifaceIdx=0; ifaceIdx<interfaceList.length; ifaceIdx++) {
			DeclarationType interfaceDeclType = interfaceList[ifaceIdx];
			importRepository.attach(interfaceDeclType);
		}

		
		ClassfileMethod methods[] = parsedClass.getMethods();
		
		
		byte[] bytecode = parsedClass.getByteCode();
		if (bytecode!=null) {
			MnemonicBlock opperationBlock = new MnemonicBlock(bytecode);
			GroupConnector blocker = new GroupConnector(importRepository, parsedClass, null, opperationBlock, parsedClass.getTryCatchList());

			SubCodeBlock subCodeBlock = blocker.connectContext.subCodeBlock;
			
			ModelStageTwoRuntime modelConnector = new ModelStageTwoRuntime(subCodeBlock, importRepository, null); /* is there a linenumbertable for a class if so then implement*/
			modelConnector.connect();
		}
		
		
		
		List<MethodContext> methodContextList = new ArrayList<MethodContext>();
		for(ClassfileMethod methodInfo : methods) {
			log.debug("--------------------------------------------------------------------");
			methodContextList.add(new MethodContext(importRepository, parsedClass, methodInfo));
			importRepository.attach(methodInfo);
		}
		
		ImportStatement imports[] = importRepository.createStatements();

		
		
		Method cmethods[] = new Method[methodContextList.size()];
		int index = 0;
		for(MethodContext context : methodContextList) {
			cmethods[index++] = context.createMethod();
		}

		
		FieldDeclaration fields[] = new FieldDeclaration[fieldList.length];
		for(int fieldIdx=0; fieldIdx<fieldList.length; fieldIdx++) {
			ClassfileField parsedField = fieldList[fieldIdx];
			fields[fieldIdx] = createFinal(parsedField, null); // TODO second argument should be set
		}
		
		Name interfaces[] = new Name[interfaceList.length];
		for(int ifaceIdx=0; ifaceIdx<interfaceList.length; ifaceIdx++) {
			DeclarationType interfaceDeclType = interfaceList[ifaceIdx];
			Name resolvedTypename = importRepository.createResolvedTypename(interfaceDeclType.referenceTypeName);
			interfaces[ifaceIdx] = resolvedTypename;
		}
		
		TypeHeader typeHeader = new TypeHeader(parsedClass.getMainTypeName(), parsedClass.getModifiers(), parsedClass.getSuperTypeName(), interfaces);
		
		return new Type(packageStatement, imports, typeHeader, fields, cmethods);
	}


//	private Method createMethod(ParsedClass classDef, ParsedMethod method) {
//		MethodHeader methodHeader = method.methodHeader;
//		String methodName = methodHeader.methodName;
//		if (methodName.equals("<init>")) {
//			methodName = classDef.getClassNameFQN();
//		}
//		
//		
//		byte[] bytecode = method.getBytecode();
//		MnemonicBlock opperationBlock = new MnemonicBlock(bytecode);
//
//		VirtualRuntime virtualRuntime = new VirtualRuntime(classDef, method, opperationBlock);
//		Block methodBlock = virtualRuntime.run();
//
//		return new Method(method.modifiers, method.methodHeader, methodBlock);
//	}
//
//	

}
