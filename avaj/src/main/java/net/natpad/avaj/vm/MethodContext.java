package net.natpad.avaj.vm;

import net.natpad.avaj.ast.Argument;
import net.natpad.avaj.ast.Block;
import net.natpad.avaj.ast.Method;
import net.natpad.avaj.ast.MethodHeader;
import net.natpad.avaj.classfile.Classfile;
import net.natpad.avaj.classfile.ClassfileMethod;
import net.natpad.avaj.classfile.TryCatchList;
import net.natpad.avaj.classfile.attribute.AttributeLocalVariableTable;
import net.natpad.avaj.classfile.attribute.LineNumberTable;
import net.natpad.avaj.classfile.attribute.LocalVariableTable;
import net.natpad.avaj.classfile.bytecode.MnemonicBlock;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.vm.intermediate.IntermediateRunContext;
import net.natpad.avaj.vm.subcode.block.SubCodeBlock;
import net.natpad.avaj.vm.subcode.impl.MethodFrameValue;
import net.natpad.avaj.vm.type.ImportRepository;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;

public class MethodContext {
	
	static Logger log = Logger.getInstance(MethodContext.class);

	
	public final Classfile parsedClass;
	public final ClassfileMethod parsedMethod;
	
	public final MnemonicBlock mnemonicBlock;
	MnemonicLineNumberTable mnemonicLineNumberTable;
	public final IntermediateRunContext connectContext;
	
	public final ImportRepository importRepository;
	
	public MethodContext(ImportRepository importRepository, Classfile parsedClass, ClassfileMethod parsedMethod) {
		this.importRepository = importRepository;
		this.parsedClass = parsedClass;
		this.parsedMethod = parsedMethod;
		
		log.debug(""+parsedMethod.methodHeader);
		
		byte[] bytecode = parsedMethod.getBytecode();
		mnemonicBlock = new MnemonicBlock(bytecode);
		mnemonicBlock.dumpCode(System.out);
		TryCatchList tryCatchList = parsedMethod.getTryCatchList();
		if (tryCatchList!=null) {
			tryCatchList.dump(System.out);
		}
		
		GroupConnector blocker = new GroupConnector(importRepository, parsedClass, parsedMethod, mnemonicBlock, tryCatchList);
		connectContext = blocker.connectContext;
		
		int frameIndex = 0;
		if (!parsedMethod.modifiers.isStatic()) {
			if (parsedClass.getSuperTypeName()!=null) {
				importRepository.add(parsedClass.getSuperTypeName());
			}
			connectContext.setThis(parsedClass.getMainTypeName(), parsedClass.getSuperTypeName());
			frameIndex++;
		}
		
		LineNumberTable lineNumberTable = parsedMethod.getLineNumberTable().getTable();
		mnemonicLineNumberTable = new MnemonicLineNumberTable(mnemonicBlock, lineNumberTable);
		
		
		AttributeLocalVariableTable attributeLocalVariableTable = parsedMethod.getLocalVariableTable();
		LocalVariableTable localVariableTable = null;
		if (attributeLocalVariableTable!=null) {
			localVariableTable = attributeLocalVariableTable.getTable();
		}
		Argument[] args = parsedMethod.methodHeader.getArgs();
		log.debug("args="+args);
		if (args!=null && args.length>0) {
			for(Argument parsedField : args) {
				String name = parsedField.name;
				if (name==null) {
					name = "arg"+frameIndex;
				}
				log.detail("creating argument frame-value:frameIndex="+frameIndex+", name="+name+" localVariableTable="+localVariableTable);
				
				
				
				if (localVariableTable!=null) {
					String tabName = localVariableTable.findName(frameIndex, 0);
					if (tabName!=null) {
						name = tabName;
					}
				}
				
				
				MethodFrameValue frameValue = new MethodFrameValue(frameIndex, name);
				frameValue.add(new ValueTypeDefinition(parsedField.declarationType, ValueTypeDefinitionKind.ARGUMENT));
				connectContext.add(frameIndex, frameValue);
				frameIndex++;
				if (parsedField.declarationType.isCategory2()) {
					connectContext.add(frameIndex, frameValue);
					frameIndex++;
				}
			}
		}

		blocker.modelContainer.connectStageOne(connectContext);
		connectContext.connect(importRepository);
	}

	public Method createMethod() {
		SubCodeBlock subCodeBlock = connectContext.subCodeBlock;
		ModelStageTwoRuntime modelConnector = new ModelStageTwoRuntime(subCodeBlock, importRepository, mnemonicLineNumberTable);
		Block block = modelConnector.connect();
		MethodHeader methodHeader = modelConnector.createFinalHeader(parsedMethod);
		Method method = new Method(parsedMethod.modifiers, methodHeader, block);
		if (mnemonicLineNumberTable!=null) {
			int lnr = mnemonicLineNumberTable.getLeastLineNumber();
			if (lnr>=0) {
				method.setAtLeastLineNumber(lnr-1);
			}
		}
		return method;
	}
	
}