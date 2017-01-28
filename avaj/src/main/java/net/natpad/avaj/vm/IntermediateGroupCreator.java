package net.natpad.avaj.vm;


import java.util.ArrayList;

import net.natpad.avaj.ast.DeclarationType;
import net.natpad.avaj.ast.IExpression;
import net.natpad.avaj.ast.Identifier;
import net.natpad.avaj.ast.Literal;
import net.natpad.avaj.ast.QuotedString;
import net.natpad.avaj.classfile.Classfile;
import net.natpad.avaj.classfile.ClassfileCatch;
import net.natpad.avaj.classfile.ClassfileMethod;
import net.natpad.avaj.classfile.ClassfileMethodHeader;
import net.natpad.avaj.classfile.ICatchOrFinally;
import net.natpad.avaj.classfile.TryCatch;
import net.natpad.avaj.classfile.TryCatchList;
import net.natpad.avaj.classfile.bytecode.IMnemonic;
import net.natpad.avaj.classfile.bytecode.MnemonicBlock;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.bytecode.Type;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpCheckCast;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpCompareInt;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpCompareReference;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpDuplicate;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpGet;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpIncrement;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpInvoke;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpLoadArray;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpLoadConstant;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpLoadFastIndex;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpLoadIndex;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpMathBasic;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpMathLogic;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpMultiANewArray;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpNew;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpNewArray;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpPush;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpPushIndex;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpPut;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpPut.PutType;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpReturn;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpStoreFastIndex;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpStoreIndex;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpStoreIntoArray;
import net.natpad.avaj.classfile.constant.ConstantClassInfo;
import net.natpad.avaj.classfile.constant.ConstantFieldReference;
import net.natpad.avaj.classfile.constant.ConstantMethodReference;
import net.natpad.avaj.classfile.constant.ConstantPrimString;
import net.natpad.avaj.classfile.constant.ConstantPrimitive;
import net.natpad.avaj.classfile.constant.ConstantUtfText;
import net.natpad.avaj.classfile.constant.IConstant;
import net.natpad.avaj.classfile.label.Label;
import net.natpad.avaj.classfile.label.Label.LabelType;
import net.natpad.avaj.classfile.label.LabelRepository;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;
import net.natpad.avaj.vm.intermediate.IIntermediateNode;
import net.natpad.avaj.vm.intermediate.IntermediateFinallyGroup;
import net.natpad.avaj.vm.intermediate.IntermediateGroup;
import net.natpad.avaj.vm.intermediate.IntermediateNodeScope;
import net.natpad.avaj.vm.intermediate.IntermediateTryCatchGroup;
import net.natpad.avaj.vm.intermediate.ItermediateCatchGroup;
import net.natpad.avaj.vm.intermediate.item.IntermediateArrayGet;
import net.natpad.avaj.vm.intermediate.item.IntermediateArrayLength;
import net.natpad.avaj.vm.intermediate.item.IntermediateArrayPut;
import net.natpad.avaj.vm.intermediate.item.IntermediateBitwiseAndOrXor;
import net.natpad.avaj.vm.intermediate.item.IntermediateCast;
import net.natpad.avaj.vm.intermediate.item.IntermediateCondition;
import net.natpad.avaj.vm.intermediate.item.IntermediateConstant;
import net.natpad.avaj.vm.intermediate.item.IntermediateDuplicate;
import net.natpad.avaj.vm.intermediate.item.IntermediateFieldGet;
import net.natpad.avaj.vm.intermediate.item.IntermediateFieldPut;
import net.natpad.avaj.vm.intermediate.item.IntermediateFrameGet;
import net.natpad.avaj.vm.intermediate.item.IntermediateFramePut;
import net.natpad.avaj.vm.intermediate.item.IntermediateGoto;
import net.natpad.avaj.vm.intermediate.item.IntermediateIncrement;
import net.natpad.avaj.vm.intermediate.item.IntermediateInstanceOf;
import net.natpad.avaj.vm.intermediate.item.IntermediateInvokeSpecial;
import net.natpad.avaj.vm.intermediate.item.IntermediateInvokeStatic;
import net.natpad.avaj.vm.intermediate.item.IntermediateInvokeVirtual;
import net.natpad.avaj.vm.intermediate.item.IntermediateMathBasic;
import net.natpad.avaj.vm.intermediate.item.IntermediateMonitorEnter;
import net.natpad.avaj.vm.intermediate.item.IntermediateMonitorExit;
import net.natpad.avaj.vm.intermediate.item.IntermediateMultiNewArray;
import net.natpad.avaj.vm.intermediate.item.IntermediateNew;
import net.natpad.avaj.vm.intermediate.item.IntermediateNewArray;
import net.natpad.avaj.vm.intermediate.item.IntermediateReturn;
import net.natpad.avaj.vm.intermediate.item.IntermediateThrow;
import net.natpad.avaj.vm.type.ImportRepository;
import net.natpad.avaj.vm.type.ValueTypeDefinition;
import net.natpad.avaj.vm.type.ValueTypeDefinitionKind;
import net.natpad.avaj.vm.type.ValueTypeDefinitionList;

public class IntermediateGroupCreator {

	static Logger log = Logger.getInstance(IIntermediateNode.class);
	
	public final MnemonicBlock mnemonicBlock;
	public final Classfile typeDefinition;
	public final ClassfileMethod parsedMethod;
	
	public final IntermediateGroup modelContainer;
	public final ImportRepository importRepository;
	public final TryCatchList tryCatchList;
	
	public IntermediateGroupCreator(ImportRepository importRepository, Classfile typeDefinition, ClassfileMethod parsedMethod, MnemonicBlock mnemonicBlock, TryCatchList tryCatchList) {
		this.mnemonicBlock = mnemonicBlock;
		this.typeDefinition = typeDefinition;
		this.importRepository = importRepository;
		this.parsedMethod = parsedMethod;
		this.tryCatchList = tryCatchList;
		if (tryCatchList!=null) {
			tryCatchList.dump(System.out);
		}
		modelContainer = createInitialBlock();
	}
	
	


	private IntermediateGroup createInitialBlock() {
		int oppIndex = 0;
		
		LabelRepository labelRepository = mnemonicBlock.getLabelRepository();
		
		IntermediateGroup result = new IntermediateGroup();
		
		
		IntermediateGroup main = result;
		
		IntermediateGroup group = null;

		
		ArrayList<IntermediateTryCatchGroup> pendingTryCatches = new ArrayList<IntermediateTryCatchGroup>();
		
		int tryCatchListIndex = 0;
		for(IMnemonic mnemonic : mnemonicBlock) {
			int offset = mnemonic.getOffset();

			if (tryCatchList!=null) {
				main = null;
				while(!pendingTryCatches.isEmpty()) {
					IntermediateTryCatchGroup tryCatchGroup = pendingTryCatches.get(pendingTryCatches.size()-1);
					if (tryCatchGroup.tryCatch.end<=offset) {
						pendingTryCatches.remove(pendingTryCatches.size()-1);
						group = null;
					} else {
						main = tryCatchGroup;
						break;
					}
				}
				if (main == null) {
					main = result;
				}
				
				
				while(tryCatchListIndex<tryCatchList.count()) {
					TryCatch tryCatch = tryCatchList.get(tryCatchListIndex);
					
					if (tryCatch.start<=offset) {
						IntermediateTryCatchGroup tryCatchGroup = new IntermediateTryCatchGroup(tryCatch);
						for(ICatchOrFinally catchItem : tryCatch) {
							if (catchItem instanceof ClassfileCatch) {
								importRepository.attach(((ClassfileCatch) catchItem).exceptionType);
								tryCatchGroup.add(new ItermediateCatchGroup(((ClassfileCatch) catchItem).exceptionType));
							} else {
								tryCatchGroup.add(new IntermediateFinallyGroup());
							}
						}
						pendingTryCatches.add(tryCatchGroup);
						main.add(tryCatchGroup);
						group = null;
						main = tryCatchGroup;
						tryCatchListIndex++;
					} else  {
						break;
					}
				}
			}
			
			int branchOffset = mnemonic.getBranchOffset();
			IIntermediateNode intermediateNode = run(oppIndex, mnemonic);
			
			if (intermediateNode!=null) {
				if (group==null) {
					group = new IntermediateGroup();
					main.add(group);
				}
				group.add(intermediateNode);
//				log.detail("adding:"+oppIndex+" >> "+subCode);
			} else {
				log.warn(""+mnemonic+", class="+TextUtil.getName(mnemonic.getClass()));
			}
			
			
			Label nextOppLabel = labelRepository.get(oppIndex+1);
			if (intermediateNode!=null) {
				IntermediateNodeScope scope = intermediateNode.getScope();
				if (scope.getExitIndex()<0 && scope.getJumpIndex()<0) {
					group = null;
				}
			}
			if (branchOffset>=0 || (nextOppLabel!=null && nextOppLabel.getType()==LabelType.HARD) || oppIndex==mnemonicBlock.count()-1) {
				group = null;
			}
			oppIndex++;
		}
		return result;
	}


	private IIntermediateNode run(int mnemonicIndex, IMnemonic mnemonic) {
		switch(mnemonic.getOperation()) {
//			case OP_NOP : break;
			case OP_ACONST_NULL : {
				return new IntermediateConstant(mnemonicIndex, ValueTypeDefinitionList.NULL_TYPE_SET, new Identifier("null"));				
			}
			case OP_ICONST_M1 :
			case OP_ICONST_0 :
			case OP_ICONST_1 :
			case OP_ICONST_2 :
			case OP_ICONST_3 :
			case OP_ICONST_4 :
			case OP_ICONST_5 :
			case OP_LCONST_0 :
			case OP_LCONST_1 :
			case OP_FCONST_0 :
			case OP_FCONST_1 :
			case OP_FCONST_2 :
			case OP_DCONST_0 :
			case OP_DCONST_1 : {
				OpLoadConstant oppLoadConstant = (OpLoadConstant) mnemonic;
				DeclarationType declarationDescriptionPrimitive = new DeclarationType(oppLoadConstant.type,0);
				ValueTypeDefinitionList typedSet = new ValueTypeDefinitionList(declarationDescriptionPrimitive, ValueTypeDefinitionKind.CONSTANT);
				return new IntermediateConstant(mnemonicIndex, typedSet, new Literal(oppLoadConstant.object));
			}
			case OP_BIPUSH :
			case OP_SIPUSH : {
				OpPush oppPush = (OpPush) mnemonic;
				ValueTypeDefinitionList typedSet = new ValueTypeDefinitionList();
				typedSet.add(new ValueTypeDefinition(DeclarationType.DEFAULT_INT, ValueTypeDefinitionKind.CONSTANT));
				typedSet.add(new ValueTypeDefinition(DeclarationType.DEFAULT_CHAR, ValueTypeDefinitionKind.CONSTANT));
				IExpression defaultExpression = new Literal(oppPush.value);
				
				if (oppPush.value==0 || oppPush.value==1) {
					typedSet.add(new ValueTypeDefinition(DeclarationType.DEFAULT_BOOLEAN, ValueTypeDefinitionKind.CONSTANT));
				}
				
				return new IntermediateConstant(mnemonicIndex, typedSet, defaultExpression);
			}
			case OP_LDC : 
			case OP_LDC_W : 
			case OP_LDC2_W : {
				OpPushIndex oppPushIndex = (OpPushIndex) mnemonic;
				IConstant poolConstant = typeDefinition.getConstantPool().getResolved(oppPushIndex.index-1);
				log.detail("poolConstant.class="+TextUtil.getName(poolConstant.getClass()));
				if (poolConstant instanceof ConstantUtfText) {
					ConstantUtfText utfText = (ConstantUtfText) poolConstant;
					ValueTypeDefinitionList expr = new ValueTypeDefinitionList(DeclarationType.DEFAULT_STRING, ValueTypeDefinitionKind.CONSTANT);
					IntermediateConstant result = new IntermediateConstant(mnemonicIndex, expr, new QuotedString(utfText.text));
					return result;
				} else if (poolConstant instanceof ConstantPrimString) {
					ConstantPrimString primString = (ConstantPrimString) poolConstant;
					ValueTypeDefinitionList expr = new ValueTypeDefinitionList(DeclarationType.DEFAULT_STRING, ValueTypeDefinitionKind.CONSTANT);
					IntermediateConstant result = new IntermediateConstant(mnemonicIndex, expr, new QuotedString(primString.getText()));
					return result;
				} else if (poolConstant instanceof ConstantClassInfo) {
					ConstantClassInfo primString = (ConstantClassInfo) poolConstant;
					ValueTypeDefinitionList expr = new ValueTypeDefinitionList(DeclarationType.DEFAULT_CLASS, ValueTypeDefinitionKind.CONSTANT);
					IntermediateConstant result = new IntermediateConstant(mnemonicIndex, expr, new Literal(primString.getRefrenceTypeName().createFQN()+".class"));
					return result;
				} else if (poolConstant instanceof ConstantPrimitive) {
					ConstantPrimitive<?> prim = (ConstantPrimitive<?>) poolConstant;
					Object value = prim.value;
					ValueTypeDefinitionList expr = null;
					if (value instanceof Integer) {
						expr = new ValueTypeDefinitionList();
						expr.add(new ValueTypeDefinition(DeclarationType.DEFAULT_INT, ValueTypeDefinitionKind.CONSTANT));
						expr.add(new ValueTypeDefinition(DeclarationType.DEFAULT_CHAR, ValueTypeDefinitionKind.CONSTANT));
					} else if (value instanceof Long) {
						expr = new ValueTypeDefinitionList(new DeclarationType(Type.LONG,0), ValueTypeDefinitionKind.CONSTANT);
					} else if (value instanceof Float) {
						expr = new ValueTypeDefinitionList(new DeclarationType(Type.FLOAT,0), ValueTypeDefinitionKind.CONSTANT);
					} else if (value instanceof Double) {
						expr = new ValueTypeDefinitionList(new DeclarationType(Type.DOUBLE,0), ValueTypeDefinitionKind.CONSTANT);
					}
					IntermediateConstant result = new IntermediateConstant(mnemonicIndex, expr, new Literal(value));
					return result;
					
				}				
			}
			case OP_ILOAD :
			case OP_LLOAD :
			case OP_FLOAD :
			case OP_DLOAD :
			case OP_ALOAD : {
				return new IntermediateFrameGet(mnemonicIndex, ((OpLoadIndex) mnemonic).index);
			}
			case OP_ILOAD_0 :
			case OP_ILOAD_1 :
			case OP_ILOAD_2 :
			case OP_ILOAD_3 :
			case OP_LLOAD_0 :
			case OP_LLOAD_1 :
			case OP_LLOAD_2 :
			case OP_LLOAD_3 :
			case OP_FLOAD_0 :
			case OP_FLOAD_1 :
			case OP_FLOAD_2 :
			case OP_FLOAD_3 :
			case OP_DLOAD_0 :
			case OP_DLOAD_1 :
			case OP_DLOAD_2 :
			case OP_DLOAD_3 :
			case OP_ALOAD_0 :
			case OP_ALOAD_1 :
			case OP_ALOAD_2 :
			case OP_ALOAD_3 : {
				return new IntermediateFrameGet(mnemonicIndex, ((OpLoadFastIndex) mnemonic).index);
			}
			case OP_IALOAD :
			case OP_LALOAD :
			case OP_FALOAD :
			case OP_DALOAD :
			case OP_AALOAD :
			case OP_BALOAD :
			case OP_CALOAD :
			case OP_SALOAD : {
				OpLoadArray oppLoadArray = (OpLoadArray) mnemonic;
				IntermediateArrayGet result = new IntermediateArrayGet(mnemonicIndex, oppLoadArray.loadType);
				return result;
			}
			case OP_ISTORE :
			case OP_LSTORE :
			case OP_FSTORE :
			case OP_DSTORE :
			case OP_ASTORE : {
				OpStoreIndex oppStoreIndex = (OpStoreIndex) mnemonic;
				return new IntermediateFramePut(mnemonicIndex, oppStoreIndex.index);
			}
			case OP_ISTORE_0 :
			case OP_ISTORE_1 :
			case OP_ISTORE_2 :
			case OP_ISTORE_3 :
			case OP_LSTORE_0 :
			case OP_LSTORE_1 :
			case OP_LSTORE_2 :
			case OP_LSTORE_3 :
			case OP_FSTORE_0 :
			case OP_FSTORE_1 :
			case OP_FSTORE_2 :
			case OP_FSTORE_3 :
			case OP_DSTORE_0 :
			case OP_DSTORE_1 :
			case OP_DSTORE_2 :
			case OP_DSTORE_3 :
			case OP_ASTORE_0 :
			case OP_ASTORE_1 :
			case OP_ASTORE_2 :
			case OP_ASTORE_3 : {
				OpStoreFastIndex oppStoreFastIndex = (OpStoreFastIndex) mnemonic;
				return new IntermediateFramePut(mnemonicIndex, oppStoreFastIndex.index);
			}
			case OP_IASTORE :
			case OP_LASTORE :
			case OP_FASTORE :
			case OP_DASTORE :
			case OP_AASTORE :
			case OP_BASTORE :
			case OP_CASTORE :
			case OP_SASTORE : {
				OpStoreIntoArray oppStoreIntoArray = (OpStoreIntoArray) mnemonic;
				IntermediateArrayPut result = new IntermediateArrayPut(mnemonicIndex, oppStoreIntoArray.storeType);
				return result;
			}
//			case OP_POP : break;
//			case OP_POP2 : break;
			case OP_DUP :
			case OP_DUP_X1 :
			case OP_DUP_X2 :
			case OP_DUP2 :
			case OP_DUP2_X1 :
			case OP_DUP2_X2 : {
				OpDuplicate oppDuplicate = (OpDuplicate) mnemonic;
				return new IntermediateDuplicate(mnemonicIndex, oppDuplicate.dupType);
			}
//			case OP_SWAP : break;
			case OP_IADD :
			case OP_LADD :
			case OP_FADD :
			case OP_DADD :
			case OP_ISUB :
			case OP_LSUB :
			case OP_FSUB :
			case OP_DSUB :
			case OP_IMUL :
			case OP_LMUL :
			case OP_FMUL :
			case OP_DMUL :
			case OP_IDIV :
			case OP_LDIV :
			case OP_FDIV :
			case OP_DDIV :
			case OP_IREM :
			case OP_LREM :
			case OP_FREM :
			case OP_DREM :
			case OP_INEG :
			case OP_LNEG :
			case OP_FNEG :
			case OP_DNEG : {
				OpMathBasic oppMathBasic = (OpMathBasic) mnemonic;
				return new IntermediateMathBasic(mnemonicIndex, oppMathBasic.operator);
			}
			case OP_ISHL :
			case OP_LSHL :
			case OP_ISHR :
			case OP_LSHR :
			case OP_IUSHR :
			case OP_LUSHR :
			case OP_IAND :
			case OP_LAND :
			case OP_IOR :
			case OP_LOR :
			case OP_IXOR :
			case OP_LXOR :
			{
				OpMathLogic oppMathLogic = (OpMathLogic) mnemonic;
				return new IntermediateBitwiseAndOrXor(mnemonicIndex, oppMathLogic.operator);
			}
			case OP_IINC : {
				OpIncrement oppIncrement = (OpIncrement) mnemonic;
				return new IntermediateIncrement(mnemonicIndex, oppIncrement.index, oppIncrement.constant);
			}
//			case OP_I2L : break;
//			case OP_I2F : break;
//			case OP_I2D : break;
//			case OP_L2I : break;
//			case OP_L2F : break;
//			case OP_L2D : break;
//			case OP_F2I : break;
//			case OP_F2L : break;
//			case OP_F2D : break;
//			case OP_D2I : break;
//			case OP_D2L : break;
//			case OP_D2F : break;
//			case OP_I2B : break;
//			case OP_I2C : break;
//			case OP_I2S : break;
//			case OP_LCMP : break;
//			case OP_FCMPL : break;
//			case OP_FCMPG : break;
//			case OP_DCMPL : break;
//			case OP_DCMPG : break;
			case OP_IFEQ :
			case OP_IFNE :
			case OP_IFLT :
			case OP_IFGE :
			case OP_IFGT :
			case OP_IFLE :
			case OP_IF_ICMPEQ :
			case OP_IF_ICMPNE :
			case OP_IF_ICMPLT :
			case OP_IF_ICMPGE :
			case OP_IF_ICMPGT :
			case OP_IF_ICMPLE : {
				OpCompareInt oppCompareInt = (OpCompareInt) mnemonic;
				Label branchLab = mnemonicBlock.findOrCreatLabelAt(mnemonicBlock.findByBytecodeOffset(mnemonic.getBranchOffset()));
				return new IntermediateCondition(mnemonicIndex, oppCompareInt.getConditionType(), branchLab.mnemonicIndex, oppCompareInt.isWithZero(), true);
			}
			case OP_IF_ACMPEQ :
			case OP_IF_ACMPNE :
			case OP_IFNULL : 
			case OP_IFNONNULL : {
				OpCompareReference oppCompareReference = (OpCompareReference) mnemonic;
				Label branchLab = mnemonicBlock.findOrCreatLabelAt(mnemonicBlock.findByBytecodeOffset(mnemonic.getBranchOffset()));
				return new IntermediateCondition(mnemonicIndex, oppCompareReference.getConditionType(), branchLab.mnemonicIndex, oppCompareReference.withZero, false);
			}
			case OP_GOTO :
			case OP_GOTO_W : {
				Label branchLab = mnemonicBlock.findOrCreatLabelAt(mnemonicBlock.findByBytecodeOffset(mnemonic.getBranchOffset()));
				return new IntermediateGoto(mnemonicIndex, branchLab.mnemonicIndex);
			}
//			case OP_JSR : break;
//			case OP_RET : break;
//			case OP_TABLESWITCH : break;
//			case OP_LOOKUPSWITCH : break;
			case OP_IRETURN :
			case OP_LRETURN :
			case OP_FRETURN :
			case OP_DRETURN :
			case OP_ARETURN :
			case OP_RETURN : {
				DeclarationType returnType = null;
				OpReturn oppReturn = (OpReturn) mnemonic;
				if (parsedMethod!=null) {
					returnType = parsedMethod.methodHeader.returnType;
					if (returnType!=null && returnType.isVoid()) {
						returnType = null;
					}
				} else {
					if (oppReturn.returnType!=null) {
						if (oppReturn.returnType==Type.REFERENCE) {
							returnType = parsedMethod.methodHeader.returnType;
						} else {
							returnType = new DeclarationType(oppReturn.returnType, 0);
						}
					}
				}
				return new IntermediateReturn(mnemonicIndex, returnType);
			}
			case OP_GETSTATIC :
			case OP_GETFIELD : {
				OpGet oppPushIndex = (OpGet) mnemonic;
				IConstant poolConstant = typeDefinition.getConstantPool().getResolved(oppPushIndex.poolIndex-1);
				if (poolConstant instanceof ConstantFieldReference) {
					ConstantFieldReference fieldRef = (ConstantFieldReference) poolConstant;
					DeclarationType fieldDeclType = fieldRef.getDeclarationType();
					
					DeclarationType fieldSourceDescription = fieldRef.getFieldSourceDescription();
					importRepository.attach(fieldSourceDescription);

					
					IntermediateFieldGet result = new IntermediateFieldGet(mnemonicIndex, fieldDeclType, fieldSourceDescription, new Identifier(fieldRef.getFieldName()), mnemonic.getOperation()==Operation.OP_GETFIELD);
					return result;
				}
			} break;
			case OP_PUTSTATIC :
			case OP_PUTFIELD : {
				OpPut oppPut = (OpPut) mnemonic;
				IConstant poolConstant = typeDefinition.getConstantPool().getResolved(oppPut.poolIndex-1);
				if (poolConstant instanceof ConstantFieldReference) {
					ConstantFieldReference fieldRef = (ConstantFieldReference) poolConstant;
					DeclarationType fieldDeclType = fieldRef.getDeclarationType();
//					importRepository.attach(declarationType);
					
					DeclarationType fieldSourceDescription = fieldRef.getFieldSourceDescription();
					importRepository.attach(fieldSourceDescription);
					
					IntermediateFieldPut result = new IntermediateFieldPut(mnemonicIndex, fieldDeclType, fieldSourceDescription, new Identifier(fieldRef.getFieldName()), oppPut.putType==PutType.FIELD);
					return result;
				}
			} break;
			case OP_INVOKEVIRTUAL : {
				OpInvoke oppInvoke = (OpInvoke) mnemonic;
				
				IConstant constant = typeDefinition.getConstantPool().getResolved(oppInvoke.index-1);
				ConstantMethodReference poolConstant = (ConstantMethodReference) constant;
				
				ClassfileMethodHeader parsedMethodHeader = poolConstant.getMethodHeader();
				return new IntermediateInvokeVirtual(mnemonicIndex, parsedMethodHeader);
			}
			case OP_INVOKESPECIAL : {
				OpInvoke oppInvoke = (OpInvoke) mnemonic;
				
				IConstant constant = typeDefinition.getConstantPool().getResolved(oppInvoke.index-1);
				ConstantMethodReference poolConstant = (ConstantMethodReference) constant;
				DeclarationType descriptor = poolConstant.getConstantClassInfo().getParsedDescriptor();
				
				ClassfileMethodHeader parsedMethodHeader = poolConstant.getMethodHeader();
				return new IntermediateInvokeSpecial(mnemonicIndex, descriptor, parsedMethodHeader);
			}
			case OP_INVOKESTATIC : {
				OpInvoke oppInvoke = (OpInvoke) mnemonic;
				
				IConstant constant = typeDefinition.getConstantPool().getResolved(oppInvoke.index-1);
				ConstantMethodReference poolConstant = (ConstantMethodReference) constant;
				DeclarationType descriptor = poolConstant.getConstantClassInfo().getParsedDescriptor();
				
				ClassfileMethodHeader parsedMethodHeader = poolConstant.getMethodHeader();
				return new IntermediateInvokeStatic(mnemonicIndex, descriptor, parsedMethodHeader);
			}
			case OP_INVOKEINTERFACE : {
				OpInvoke oppInvoke = (OpInvoke) mnemonic;
				
				IConstant constant = typeDefinition.getConstantPool().getResolved(oppInvoke.index-1);
				ConstantMethodReference poolConstant = (ConstantMethodReference) constant;
				
				ClassfileMethodHeader parsedMethodHeader = poolConstant.getMethodHeader();
				return new IntermediateInvokeVirtual(mnemonicIndex, parsedMethodHeader);
			}
//			case OP_INVOKEDYNAMIC : break;
			case OP_NEW : {
				OpNew oppNew = (OpNew) mnemonic;
				ConstantClassInfo constantClassInfo = (ConstantClassInfo) typeDefinition.getConstantPool().getResolved(oppNew.index-1);
				DeclarationType declarationType = constantClassInfo.getParsedDescriptor();
				importRepository.attach(declarationType);
				return new IntermediateNew(mnemonicIndex, declarationType);
			}
			case OP_NEWARRAY :
			case OP_ANEWARRAY : {
				OpNewArray oppNewArray = (OpNewArray) mnemonic;
				switch(oppNewArray.arrayType) {
					case BOOLEAN : 
					case BYTE : 
					case CHAR : 
					case DOUBLE : 
					case FLOAT : 
					case INT : 
					case LONG : 
					case SHORT : {
						return new IntermediateNewArray(mnemonicIndex, new DeclarationType(oppNewArray.arrayType, 1));
					}
					case REFERENCE : {
						ConstantClassInfo constantClassInfo = (ConstantClassInfo) typeDefinition.getConstantPool().getResolved(oppNewArray.refTypeIndex-1);
						DeclarationType descriptor = constantClassInfo.getParsedDescriptor();
						return new IntermediateNewArray(mnemonicIndex, descriptor.setDimCount(1));
					}
				}
			} break;
			case OP_ARRAYLENGTH : {
				return new IntermediateArrayLength(mnemonicIndex);
			}
			case OP_ATHROW : {
				return new IntermediateThrow(mnemonicIndex);				
			}
			case OP_CHECKCAST : {
				OpCheckCast checkCast = (OpCheckCast) mnemonic;
				ConstantClassInfo constantClass = (ConstantClassInfo) typeDefinition.getConstantPool().getResolved(checkCast.poolIndex-1);
				DeclarationType declDescRef = constantClass.getParsedDescriptor();
				importRepository.attach(declDescRef);
				return new IntermediateCast(mnemonicIndex, declDescRef);
			}
			case OP_INSTANCEOF : {
				OpCheckCast checkCast = (OpCheckCast) mnemonic;
				ConstantClassInfo constantClass = (ConstantClassInfo) typeDefinition.getConstantPool().getResolved(checkCast.poolIndex-1);
				DeclarationType declDescRef = constantClass.getParsedDescriptor();
				importRepository.attach(declDescRef);
				return new IntermediateInstanceOf(mnemonicIndex, declDescRef);
			}
			case OP_MONITORENTER : {
				return new IntermediateMonitorEnter();
			}
			case OP_MONITOREXIT : {
				return new IntermediateMonitorExit();
			}
//			case OP_WIDE : break;
			case OP_MULTIANEWARRAY : {
				OpMultiANewArray oppMultiANewArray = (OpMultiANewArray) mnemonic;
				ConstantClassInfo constantClassInfo = (ConstantClassInfo) typeDefinition.getConstantPool().getResolved(oppMultiANewArray.poolIndex-1);
				DeclarationType descriptor = constantClassInfo.getParsedDescriptor();
				
				return new IntermediateMultiNewArray(mnemonicIndex, oppMultiANewArray.dimensions, descriptor);
			}
//			case OP_JSR_W : break;
//			case OP_BREAKPOINT : break;
		}
		return null;
	}

}
