package net.natpad.avaj.vm;

import net.natpad.avaj.ast.Argument;
import net.natpad.avaj.ast.Block;
import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.FieldDeclaration;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.IStatement;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.ast.MethodHeader;
import net.natpad.avaj.ast.Name;
import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.classfile.ClassfileMethod;
import net.natpad.avaj.classfile.ClassfileMethodHeader;
import net.natpad.avaj.classfile.ParsedExcpetions;
import net.natpad.avaj.classfile.attribute.AttributeLocalVariableTable;
import net.natpad.avaj.classfile.attribute.LineNumberTableEntry;
import net.natpad.avaj.classfile.attribute.LocalVariableTable;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.vm.subcode.ISubCode;
import net.natpad.avaj.vm.type.ImportRepository;

public class ModelStageTwoRuntime {

	static Logger log = Logger.getInstance(ISubCode.class);

	
	public final ISubCode subCodeBlock;
	public final ImportRepository importRepository;
	public final MnemonicLineNumberTable lineNumberTable;
	
	private Block currentBlock = new Block();
	public final int level;
	private int mnemonicIndex;
	private int statementLineNr;
	
	
	public ModelStageTwoRuntime(ISubCode subCodeBlock, ImportRepository importRepository, MnemonicLineNumberTable lineNumberTable) {
		this.subCodeBlock = subCodeBlock;
		this.importRepository = importRepository;
		this.lineNumberTable = lineNumberTable;
		this.level = 0;
		this.mnemonicIndex = -1;
		this.statementLineNr = -1;
	}

	private ModelStageTwoRuntime(ISubCode subCodeBlock, ImportRepository importRepository, MnemonicLineNumberTable lineNumberTable, int level, int mnemonicIndex, int statementLineNr) {
		this.subCodeBlock = subCodeBlock;
		this.importRepository = importRepository;
		this.lineNumberTable = lineNumberTable;
		this.level = level;
		this.mnemonicIndex = mnemonicIndex;
		this.statementLineNr = statementLineNr;
	}

	
	public ModelStageTwoRuntime createBranch(Block currentBlock) {
		ModelStageTwoRuntime result = new ModelStageTwoRuntime(subCodeBlock, importRepository, lineNumberTable, level+1, mnemonicIndex, statementLineNr);
		result.currentBlock = currentBlock;
		return result;
	}
	
	
	public Block connect() {
		log.debug("runtime:"+hashCode()+", level="+level);
		log.indent();
		subCodeBlock.connectStageTwo(this);
		currentBlock.removeLastReturn();
		log.dedent();
		return currentBlock;
	}

	public void writeSource(Writer out) {
		currentBlock.writeStatement(out);
	}
	
	
	public void setMnemonicIndex(int mnemonicIndex) {
		this.mnemonicIndex = mnemonicIndex;
		if (lineNumberTable!=null) {
			for(LineNumberTableEntry entry : lineNumberTable) {
				if (entry.startPc==mnemonicIndex) {
					statementLineNr = entry.lineNumber;
					break;
				}
			}
		}
	}


	public void appendStatement(IStatement statement) {
		currentBlock.append(statement);
		if (statementLineNr!=-1) {
			statement.setAtLeastLineNumber(statementLineNr);
		}
	}
		
	
	
	public Identifier createPlainName(DeclarationType declarationType, Identifier fieldSourceClass) {
		if (declarationType.isReference()) {
			Name resolvedTypename = importRepository.createResolvedTypename(declarationType.referenceTypeName);
			if (fieldSourceClass!=null) {
				if (importRepository.mainTypeName.equals(declarationType.referenceTypeName)) {
					return fieldSourceClass;
				}
				return new Identifier(resolvedTypename.createFQN()+"."+fieldSourceClass.text);
			}
			return new Identifier(resolvedTypename.createFQN());
		} else {
			return new Identifier(declarationType.type.toString().toLowerCase());
		}
	}

	

	public MethodHeader createFinalHeader(ClassfileMethod parsedMethod) {
		ClassfileMethodHeader methodHeader = parsedMethod.methodHeader;
		
		DeclarationType finalDeclType = null;
		if (methodHeader.returnType!=null) {
			finalDeclType = importRepository.createFinalDeclarationType(methodHeader.returnType);
		}
		
		int frameIndex = 0;
		if (!parsedMethod.modifiers.isStatic()) {
			frameIndex++;
		}


		AttributeLocalVariableTable attributeLocalVariableTable = parsedMethod.getLocalVariableTable();
		LocalVariableTable localVariableTable = null;
		if (attributeLocalVariableTable!=null) {
			localVariableTable = attributeLocalVariableTable.getTable();
		}

		FieldDeclaration arguments[] = new FieldDeclaration[methodHeader.argumentCount()];
		for(int argIdx=0; argIdx<arguments.length; argIdx++, frameIndex++) {
			Argument parsedField = methodHeader.argumentAt(argIdx);
			String localVarName = null;
			
			
			if (localVariableTable!=null) {
				localVarName = localVariableTable.findName(frameIndex, 0);
			}
			
			arguments[argIdx] = createFinal(parsedField, localVarName);
			if (parsedField.declarationType.isCategory2()) {
				frameIndex++;
			}
		}
		
		ParsedExcpetions parsedExcpetions = methodHeader.getParsedExcpetions();
		Name exceptions[] = null;
		if (parsedExcpetions!=null && parsedExcpetions.count()>0) {
			int excCount = parsedExcpetions.count();
			exceptions = new Name[excCount];
			for(int idx=0; idx<excCount; idx++) {
				// TODO can not just call getShortTypeName need to test import-repository 
				String resolvedName = parsedExcpetions.get(idx).getShortTypeName();
				exceptions[idx] = new Name(resolvedName, false);
			}
		}
		
		return new MethodHeader(new Identifier(methodHeader.methodName), methodHeader.isConstructor, finalDeclType, arguments, exceptions);
	}

	
	

	public FieldDeclaration createFinal(Argument parsedField, String localVarName) {
		IExpression fieldValue = null;
//		if (parsedField.valueExpression!=null) {
//			if (parsedField.valueExpression instanceof ConstantPrimitive) {
//				ConstantPrimitive<?> cprim = (ConstantPrimitive<?>) parsedField.valueExpression;
//				Object value = cprim.value;
//				fieldValue = new Literal(value);
//			}
//		}
		if (localVarName==null) {
			localVarName = parsedField.name;
		}
		// TODO return typ should be ast.Argument
		return new FieldDeclaration(parsedField.modifiers, importRepository.createFinalDeclarationType(parsedField.declarationType), new Identifier(localVarName), fieldValue);
	}


	
	
}
