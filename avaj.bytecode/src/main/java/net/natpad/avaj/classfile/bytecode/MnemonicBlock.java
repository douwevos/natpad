package net.natpad.avaj.classfile.bytecode;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import net.natpad.avaj.classfile.bytecode.mnemonic.OpArrayLength;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpCheckCast;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpCompareInt;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpCompareReference;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpDuplicate;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpGet;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpGoto;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpIncrement;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpInvoke;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpLoadArray;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpLoadConstant;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpLoadFastIndex;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpLoadIndex;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpLoadNull;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpMathBasic;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpMathLogic;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpMonitor;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpMultiANewArray;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpNew;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpNewArray;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpPush;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpPushIndex;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpPut;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpReturn;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpStoreFastIndex;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpStoreIndex;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpStoreIntoArray;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpTableSwitch;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpThrow;
import net.natpad.avaj.classfile.bytecode.mnemonic.OpValueConvert;
import net.natpad.avaj.classfile.label.Label;
import net.natpad.avaj.classfile.label.Label.LabelType;
import net.natpad.avaj.classfile.label.LabelRepository;
import net.natpad.avaj.log.LogLevel;
import net.natpad.avaj.log.Logger;
import net.natpad.avaj.util.TextUtil;

public class MnemonicBlock implements IMnemonicBlock {

	private static Logger log = Logger.getInstance(IMnemonic.class);
	
	private final byte bytecode[];
	private final List<IMnemonic> opperationList;
	private LabelRepository labelRepository;
	
	public MnemonicBlock(byte bytecode[]) {
		if (bytecode == null) {
			this.bytecode = new byte[0];
		} else {
			this.bytecode = new byte[bytecode.length];
			System.arraycopy(bytecode, 0, this.bytecode, 0, bytecode.length);
		}
		opperationList = run();
		
		resolveLabels();
	}

	
	public LabelRepository getLabelRepository() {
		return labelRepository;
	}
	
	private void resolveLabels() {
		labelRepository = new LabelRepository();
		int oppIndex = 0;
		for(IMnemonic opp : opperationList) {
			int bytecodeOffset = opp.getBranchOffset();
			if (bytecodeOffset>=0) {
				labelRepository.createSoft(oppIndex, opp.getOffset());
				int oppOffset = findByBytecodeOffset(bytecodeOffset);
				log.error("bytecodeoffset="+bytecodeOffset+", oppOffset="+oppOffset);
				labelRepository.createHard(oppOffset, bytecodeOffset);
				
				int contByteCodeOffset = opp.getContinuesOffset();
				if (contByteCodeOffset>=0) {
					int oppContOffset = findByBytecodeOffset(contByteCodeOffset);
					log.error("contByteCodeOffset="+contByteCodeOffset+", oppContOffset="+oppContOffset);
					labelRepository.createSoft(oppContOffset, contByteCodeOffset);
				}
			}
			oppIndex++;
		}
		
		
//		HashMap<Integer, Label> newLabels = new HashMap<Integer, Label>();
//		for(Label label : labelRepository) {
//			switch(label.getType()) {
//				case HARD : {
//					int prevIndex = label.blockIndex-1;
//					if (prevIndex<0) {
//						continue;
//					}
//					IMnemonic prevMnemonic = opperationList.get(prevIndex);
//					IMnemonic mnemonic = opperationList.get(prevIndex+1);
//					int continuesOffset = prevMnemonic.getContinuesOffset();
//					if (continuesOffset == mnemonic.getOffset()) {
//						Label contLabel = labelSet.get(prevIndex);
//						if (contLabel==null) {
//							contLabel = new Label(LabelType.SOFT, prevIndex, prevMnemonic.getOffset(), null);
//							newLabels.put(prevIndex, contLabel);
//						}
//						label.appendCaller(contLabel);
//					}
//				} break;
//				case SOFT : {
//					
//				} break;
//			} 
//		}
//		labelSet.putAll(newLabels);
	}


	protected List<IMnemonic> run() {
		ArrayList<String> lines = new ArrayList<String>();
		ArrayList<IMnemonic> operations = new ArrayList<IMnemonic>();
		boolean isWide = false;
		int startAdres = 0;
		int bytecodePtr = 0;
		while(bytecodePtr<bytecode.length) {
			startAdres = bytecodePtr;
			short opcode = (short) (0xff & bytecode[bytecodePtr++]);
			log.trace(":startAdres="+startAdres+", bytecodePtr="+bytecodePtr+", opcode="+TextUtil.hex(opcode, 2));
			
			IMnemonic opperation = null;
			switch(opcode) {
				case 0x00 : {
					lines.add("nop");
				} break;
				case 0x01 : {
					opperation = new OpLoadNull(startAdres);
				} break;
				case 0x02 : opperation = new OpLoadConstant(Operation.OP_ICONST_M1, Type.INT, new Integer(-1), startAdres); break;
				case 0x03 : opperation = new OpLoadConstant(Operation.OP_ICONST_0, Type.INT, new Integer(0), startAdres); break;
				case 0x04 : opperation = new OpLoadConstant(Operation.OP_ICONST_1, Type.INT, new Integer(1), startAdres); break;
				case 0x05 : opperation = new OpLoadConstant(Operation.OP_ICONST_2, Type.INT, new Integer(2), startAdres); break;
				case 0x06 : opperation = new OpLoadConstant(Operation.OP_ICONST_3, Type.INT, new Integer(3), startAdres); break;
				case 0x07 : opperation = new OpLoadConstant(Operation.OP_ICONST_4, Type.INT, new Integer(4), startAdres); break;
				case 0x08 : opperation = new OpLoadConstant(Operation.OP_ICONST_5, Type.INT, new Integer(5), startAdres); break;
				case 0x09 : opperation = new OpLoadConstant(Operation.OP_LCONST_0, Type.LONG, new Long(0), startAdres); break;
				case 0x0a : opperation = new OpLoadConstant(Operation.OP_LCONST_1, Type.LONG, new Long(1), startAdres); break;
				case 0x0b : opperation = new OpLoadConstant(Operation.OP_FCONST_0, Type.FLOAT, new Float(0), startAdres); break;
				case 0x0c : opperation = new OpLoadConstant(Operation.OP_FCONST_1, Type.FLOAT, new Float(1), startAdres); break;
				case 0x0d : opperation = new OpLoadConstant(Operation.OP_FCONST_2, Type.FLOAT, new Float(2), startAdres); break;
				case 0x0e : opperation = new OpLoadConstant(Operation.OP_DCONST_0, Type.DOUBLE, new Double(0), startAdres); break;
				case 0x0f : opperation = new OpLoadConstant(Operation.OP_DCONST_1, Type.DOUBLE, new Double(1), startAdres); break;
				case 0x10 : {
					byte value = bytecode[bytecodePtr++];
					opperation = new OpPush(startAdres, 2, value);
				} break;
				case 0x11 : {
					short value = (short) ((bytecode[bytecodePtr++]<<8)|bytecode[bytecodePtr++]);
					opperation = new OpPush(startAdres, 3, value);
				} break;
				case 0x12 : {
					int index = bytecode[bytecodePtr++]&0xff;
					opperation = new OpPushIndex(Operation.OP_LDC, startAdres, 2, false, index, false);
				} break;
				case 0x13 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpPushIndex(Operation.OP_LDC_W, startAdres, 3, true, index, false);
				} break;
				case 0x14 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpPushIndex(Operation.OP_LDC2_W, startAdres, 3, true, index, true);
				} break;
				case 0x15 : {
					int index = isWide ? (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]) : 0xff & bytecode[bytecodePtr++];
					opperation = new OpLoadIndex(Operation.OP_ILOAD, startAdres, index, isWide);
				} break;
				case 0x16 : {
					int index = isWide ? (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]) : 0xff & bytecode[bytecodePtr++];
					opperation = new OpLoadIndex(Operation.OP_LLOAD, startAdres, index, isWide);
				} break;
				case 0x17 : {
					int index = isWide ? (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]) : 0xff & bytecode[bytecodePtr++];
					opperation = new OpLoadIndex(Operation.OP_FLOAD, startAdres, index, isWide);
				} break;
				case 0x18 : {
					int index = isWide ? (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]) : 0xff & bytecode[bytecodePtr++];
					opperation = new OpLoadIndex(Operation.OP_DLOAD, startAdres, index, isWide);
				} break;
				case 0x19 : {
					int index = isWide ? (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]) : 0xff & bytecode[bytecodePtr++];
					opperation = new OpLoadIndex(Operation.OP_ALOAD, startAdres, index, isWide);
				} break;
				case 0x1a : opperation = new OpLoadFastIndex(Operation.OP_ILOAD_0, startAdres, 1 ); break;
				case 0x1b : opperation = new OpLoadFastIndex(Operation.OP_ILOAD_1, startAdres, 1 ); break;
				case 0x1c : opperation = new OpLoadFastIndex(Operation.OP_ILOAD_2, startAdres, 1 ); break;
				case 0x1d : opperation = new OpLoadFastIndex(Operation.OP_ILOAD_3, startAdres, 1 ); break;
				case 0x1e : opperation = new OpLoadFastIndex(Operation.OP_LLOAD_0, startAdres, 1 ); break;
				case 0x1f : opperation = new OpLoadFastIndex(Operation.OP_LLOAD_1, startAdres, 1 ); break;
				case 0x20 : opperation = new OpLoadFastIndex(Operation.OP_LLOAD_2, startAdres, 1 ); break;
				case 0x21 : opperation = new OpLoadFastIndex(Operation.OP_LLOAD_3, startAdres, 1 ); break;
				case 0x22 : opperation = new OpLoadFastIndex(Operation.OP_FLOAD_0, startAdres, 1 ); break;
				case 0x23 : opperation = new OpLoadFastIndex(Operation.OP_FLOAD_1, startAdres, 1 ); break;
				case 0x24 : opperation = new OpLoadFastIndex(Operation.OP_FLOAD_2, startAdres, 1 ); break;
				case 0x25 : opperation = new OpLoadFastIndex(Operation.OP_FLOAD_3, startAdres, 1 ); break;
				case 0x26 : opperation = new OpLoadFastIndex(Operation.OP_DLOAD_0, startAdres, 1 ); break;
				case 0x27 : opperation = new OpLoadFastIndex(Operation.OP_DLOAD_1, startAdres, 1 ); break;
				case 0x28 : opperation = new OpLoadFastIndex(Operation.OP_DLOAD_2, startAdres, 1 ); break;
				case 0x29 : opperation = new OpLoadFastIndex(Operation.OP_DLOAD_3, startAdres, 1 ); break;
				case 0x2a : opperation = new OpLoadFastIndex(Operation.OP_ALOAD_0, startAdres, 1 ); break;
				case 0x2b : opperation = new OpLoadFastIndex(Operation.OP_ALOAD_1, startAdres, 1 ); break;
				case 0x2c : opperation = new OpLoadFastIndex(Operation.OP_ALOAD_2, startAdres, 1 ); break;
				case 0x2d : opperation = new OpLoadFastIndex(Operation.OP_ALOAD_3, startAdres, 1 ); break;
				case 0x2e : {
					opperation = new OpLoadArray(Operation.OP_IALOAD, startAdres, 1);
				} break; 
				case 0x2f :  {
					opperation = new OpLoadArray(Operation.OP_LALOAD, startAdres, 1);
				} break;
				case 0x30 :  {
					opperation = new OpLoadArray(Operation.OP_FALOAD, startAdres, 1);
				} break;
				case 0x31 :  {
					opperation = new OpLoadArray(Operation.OP_DALOAD, startAdres, 1);
				} break;
				case 0x32 :  {
					opperation = new OpLoadArray(Operation.OP_AALOAD, startAdres, 1);
				} break;
				case 0x33 : {
					opperation = new OpLoadArray(Operation.OP_BALOAD, startAdres, 1);
				} break; 
				case 0x34 : {
					opperation = new OpLoadArray(Operation.OP_CALOAD, startAdres, 1);
				} break; 
				case 0x35 : {
					opperation = new OpLoadArray(Operation.OP_SALOAD, startAdres, 1);
				} break;
				case 0x36 : {
					int index = 0xff & bytecode[bytecodePtr++];
					opperation = new OpStoreIndex(Operation.OP_ISTORE, startAdres, 2, index);
				} break; 
				case 0x37 : {
					int index = 0xff & bytecode[bytecodePtr++];
					opperation = new OpStoreIndex(Operation.OP_LSTORE, startAdres, 2, index);
				} break; 
				case 0x38 : {
					int index = 0xff & bytecode[bytecodePtr++];
					opperation = new OpStoreIndex(Operation.OP_FSTORE, startAdres, 2, index);
				} break; 
				case 0x39 : {
					int index = 0xff & bytecode[bytecodePtr++];
					opperation = new OpStoreIndex(Operation.OP_DSTORE, startAdres, 2, index);
				} break; 
				case 0x3a : {
					int index = 0xff & bytecode[bytecodePtr++];
					opperation = new OpStoreIndex(Operation.OP_ASTORE, startAdres, 2, index);
				} break;
				case 0x3b : opperation = new OpStoreFastIndex(Operation.OP_ISTORE_0, startAdres, 1); break;
				case 0x3c : opperation = new OpStoreFastIndex(Operation.OP_ISTORE_1, startAdres, 1); break;
				case 0x3d : opperation = new OpStoreFastIndex(Operation.OP_ISTORE_2, startAdres, 1); break;
				case 0x3e : opperation = new OpStoreFastIndex(Operation.OP_ISTORE_3, startAdres, 1); break;
				case 0x3f : opperation = new OpStoreFastIndex(Operation.OP_LSTORE_0, startAdres, 1); break;
				case 0x40 : opperation = new OpStoreFastIndex(Operation.OP_LSTORE_1, startAdres, 1); break;
				case 0x41 : opperation = new OpStoreFastIndex(Operation.OP_LSTORE_2, startAdres, 1); break;
				case 0x42 : opperation = new OpStoreFastIndex(Operation.OP_LSTORE_3, startAdres, 1); break;
				case 0x43 : opperation = new OpStoreFastIndex(Operation.OP_FSTORE_0, startAdres, 1); break;
				case 0x44 : opperation = new OpStoreFastIndex(Operation.OP_FSTORE_1, startAdres, 1); break;
				case 0x45 : opperation = new OpStoreFastIndex(Operation.OP_FSTORE_2, startAdres, 1); break;
				case 0x46 : opperation = new OpStoreFastIndex(Operation.OP_FSTORE_3, startAdres, 1); break;
				case 0x47 : opperation = new OpStoreFastIndex(Operation.OP_DSTORE_0, startAdres, 1); break;
				case 0x48 : opperation = new OpStoreFastIndex(Operation.OP_DSTORE_1, startAdres, 1); break;
				case 0x49 : opperation = new OpStoreFastIndex(Operation.OP_DSTORE_2, startAdres, 1); break;
				case 0x4a : opperation = new OpStoreFastIndex(Operation.OP_DSTORE_3, startAdres, 1); break;
				case 0x4b : opperation = new OpStoreFastIndex(Operation.OP_ASTORE_0, startAdres, 1); break;
				case 0x4c : opperation = new OpStoreFastIndex(Operation.OP_ASTORE_1, startAdres, 1); break;
				case 0x4d : opperation = new OpStoreFastIndex(Operation.OP_ASTORE_2, startAdres, 1); break;
				case 0x4e : opperation = new OpStoreFastIndex(Operation.OP_ASTORE_3, startAdres, 1); break;
				case 0x4f : opperation = new OpStoreIntoArray(Operation.OP_IASTORE, startAdres, 1); break;
				case 0x50 : opperation = new OpStoreIntoArray(Operation.OP_LASTORE, startAdres, 1); break;
				case 0x51 : opperation = new OpStoreIntoArray(Operation.OP_FASTORE, startAdres, 1); break;
				case 0x52 : opperation = new OpStoreIntoArray(Operation.OP_DASTORE, startAdres, 1); break;
				case 0x53 : opperation = new OpStoreIntoArray(Operation.OP_AASTORE, startAdres, 1); break;
				case 0x54 : opperation = new OpStoreIntoArray(Operation.OP_BASTORE, startAdres, 1); break;
				case 0x55 : opperation = new OpStoreIntoArray(Operation.OP_CASTORE, startAdres, 1); break;
				case 0x56 : opperation = new OpStoreIntoArray(Operation.OP_SASTORE, startAdres, 1); break;
				case 0x57 : {
					lines.add("pop");
				} break;
				case 0x58 : {
					lines.add("pop2");
				} break;
				case 0x59 : opperation = new OpDuplicate(Operation.OP_DUP, startAdres, 1); break; 
				case 0x5a : opperation = new OpDuplicate(Operation.OP_DUP_X1, startAdres,1); break; 
				case 0x5b : opperation = new OpDuplicate(Operation.OP_DUP_X2, startAdres,1); break; 
				case 0x5c : opperation = new OpDuplicate(Operation.OP_DUP2, startAdres,1); break; 
				case 0x5d : opperation = new OpDuplicate(Operation.OP_DUP2_X1, startAdres,1); break; 
				case 0x5e : opperation = new OpDuplicate(Operation.OP_DUP2_X2, startAdres,1); break;
				case 0x5f : {
					lines.add("swap");
				} break;
				case 0x60 : opperation = new OpMathBasic(Operation.OP_IADD, startAdres, Type.INT, Operator.ADD); break;
				case 0x61 : opperation = new OpMathBasic(Operation.OP_LADD, startAdres, Type.LONG, Operator.ADD); break;
				case 0x62 : opperation = new OpMathBasic(Operation.OP_FADD, startAdres, Type.FLOAT, Operator.ADD); break;
				case 0x63 : opperation = new OpMathBasic(Operation.OP_DADD, startAdres, Type.DOUBLE, Operator.ADD); break;
				case 0x64 : opperation = new OpMathBasic(Operation.OP_ISUB, startAdres, Type.INT, Operator.SUB); break;
				case 0x65 : opperation = new OpMathBasic(Operation.OP_LSUB, startAdres, Type.LONG, Operator.SUB); break;
				case 0x66 : opperation = new OpMathBasic(Operation.OP_FSUB, startAdres, Type.FLOAT, Operator.SUB); break;
				case 0x67 : opperation = new OpMathBasic(Operation.OP_DSUB, startAdres, Type.DOUBLE, Operator.SUB); break;
				case 0x68 : opperation = new OpMathBasic(Operation.OP_IMUL, startAdres, Type.INT, Operator.MULTIPLY); break;
				case 0x69 : opperation = new OpMathBasic(Operation.OP_LMUL, startAdres, Type.LONG, Operator.MULTIPLY); break;
				case 0x6a : opperation = new OpMathBasic(Operation.OP_FMUL, startAdres, Type.FLOAT, Operator.MULTIPLY); break;
				case 0x6b : opperation = new OpMathBasic(Operation.OP_DMUL, startAdres, Type.DOUBLE, Operator.MULTIPLY); break;
				case 0x6c : opperation = new OpMathBasic(Operation.OP_IDIV, startAdres, Type.INT, Operator.DIVIDE); break;
				case 0x6d : opperation = new OpMathBasic(Operation.OP_LDIV, startAdres, Type.LONG, Operator.DIVIDE); break;
				case 0x6e : opperation = new OpMathBasic(Operation.OP_FDIV, startAdres, Type.FLOAT, Operator.DIVIDE); break;
				case 0x6f : opperation = new OpMathBasic(Operation.OP_DDIV, startAdres, Type.DOUBLE, Operator.DIVIDE); break;
				case 0x70 : opperation = new OpMathBasic(Operation.OP_IREM, startAdres, Type.INT, Operator.REMAINDER); break;
				case 0x71 : opperation = new OpMathBasic(Operation.OP_LREM, startAdres, Type.LONG, Operator.REMAINDER); break;
				case 0x72 : opperation = new OpMathBasic(Operation.OP_FREM, startAdres, Type.FLOAT, Operator.REMAINDER); break;
				case 0x73 : opperation = new OpMathBasic(Operation.OP_DREM, startAdres, Type.DOUBLE, Operator.REMAINDER); break;
				case 0x74 : opperation = new OpMathBasic(Operation.OP_INEG, startAdres, Type.INT, Operator.UNARY_MINUS); break;
				case 0x75 : opperation = new OpMathBasic(Operation.OP_LNEG, startAdres, Type.LONG, Operator.UNARY_MINUS); break;
				case 0x76 : opperation = new OpMathBasic(Operation.OP_FNEG, startAdres, Type.FLOAT, Operator.UNARY_MINUS); break;
				case 0x77 : opperation = new OpMathBasic(Operation.OP_DNEG, startAdres, Type.DOUBLE, Operator.UNARY_MINUS); break;
				case 0x78 : opperation = new OpMathLogic(Operation.OP_ISHL, Type.INT, Operator.SHIFT_LEFT, startAdres, 1); break;
				case 0x79 : opperation = new OpMathLogic(Operation.OP_LSHL, Type.LONG, Operator.SHIFT_LEFT, startAdres, 1); break;
				case 0x7a : opperation = new OpMathLogic(Operation.OP_ISHR, Type.INT, Operator.SHIFT_RIGHT, startAdres, 1); break;
				case 0x7b : opperation = new OpMathLogic(Operation.OP_LSHR, Type.LONG, Operator.SHIFT_RIGHT, startAdres, 1); break;
				case 0x7c : opperation = new OpMathLogic(Operation.OP_IUSHR, Type.INT, Operator.UNSIGNED_SHIFT_RIGHT, startAdres, 1); break;
				case 0x7d : opperation = new OpMathLogic(Operation.OP_LUSHR, Type.LONG, Operator.UNSIGNED_SHIFT_RIGHT, startAdres, 1); break;
				case 0x7e : opperation = new OpMathLogic(Operation.OP_IAND, Type.INT, Operator.BITWISE_AND, startAdres, 1); break;
				case 0x7f : opperation = new OpMathLogic(Operation.OP_LAND, Type.LONG, Operator.BITWISE_AND, startAdres, 1); break;
				case 0x80 : opperation = new OpMathLogic(Operation.OP_IOR, Type.INT, Operator.BITWISE_OR, startAdres, 1); break;
				case 0x81 : opperation = new OpMathLogic(Operation.OP_LOR, Type.LONG, Operator.BITWISE_OR, startAdres, 1); break;
				case 0x82 : opperation = new OpMathLogic(Operation.OP_IXOR, Type.INT, Operator.BITWISE_XOR, startAdres, 1); break;
				case 0x83 : opperation = new OpMathLogic(Operation.OP_LXOR, Type.LONG, Operator.BITWISE_XOR, startAdres, 1); break;
				case 0x84 : {
					int index = 0;
					int constant = 0;
					if (isWide) {
						short val = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]));
						index = val;
						val = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]));
						constant = val;
					} else {
						index = bytecode[bytecodePtr++];
						constant = bytecode[bytecodePtr++];
					}
					int wideCnt = isWide ? 1 : 0;
					opperation = new OpIncrement(startAdres-wideCnt, bytecodePtr+wideCnt-startAdres, index, constant);
				} break;
				case 0x85 : opperation = new OpValueConvert(Operation.OP_I2L, Type.INT, Type.LONG, startAdres, 1); break;
				case 0x86 : opperation = new OpValueConvert(Operation.OP_I2F, Type.INT, Type.FLOAT, startAdres, 1); break;
				case 0x87 : opperation = new OpValueConvert(Operation.OP_I2D, Type.INT, Type.DOUBLE, startAdres, 1); break;
				case 0x88 : opperation = new OpValueConvert(Operation.OP_L2I, Type.LONG, Type.INT, startAdres, 1); break;
				case 0x89 : opperation = new OpValueConvert(Operation.OP_L2F, Type.LONG, Type.FLOAT, startAdres, 1); break;
				case 0x8a : opperation = new OpValueConvert(Operation.OP_L2D, Type.LONG, Type.DOUBLE, startAdres, 1); break;
				case 0x8b : opperation = new OpValueConvert(Operation.OP_F2I, Type.FLOAT, Type.INT, startAdres, 1); break;
				case 0x8c : opperation = new OpValueConvert(Operation.OP_F2L, Type.FLOAT, Type.LONG, startAdres, 1); break;
				case 0x8d : opperation = new OpValueConvert(Operation.OP_F2D, Type.FLOAT, Type.DOUBLE, startAdres, 1); break;
				case 0x8e : opperation = new OpValueConvert(Operation.OP_D2I, Type.DOUBLE, Type.INT, startAdres, 1); break;
				case 0x8f : opperation = new OpValueConvert(Operation.OP_D2L, Type.DOUBLE, Type.LONG, startAdres, 1); break;
				case 0x90 : opperation = new OpValueConvert(Operation.OP_D2F, Type.DOUBLE, Type.FLOAT, startAdres, 1); break;
				case 0x91 : opperation = new OpValueConvert(Operation.OP_I2B, Type.INT, Type.BYTE, startAdres, 1); break;
				case 0x92 : opperation = new OpValueConvert(Operation.OP_I2C, Type.INT, Type.CHAR, startAdres, 1); break;
				case 0x93 : opperation = new OpValueConvert(Operation.OP_I2S, Type.INT, Type.SHORT, startAdres, 1); break;
				case 0x94 : {
					lines.add("lcmp");
				} break;
				case 0x95 : {
					lines.add("fcmpl");
				} break;
				case 0x96 : {
					lines.add("fcmpg");
				} break;
				case 0x97 : {
					lines.add("dcmpl");
				} break;
				case 0x98 : {
					lines.add("dcmpg");
				} break;
				case 0x99 : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareInt(Operation.OP_IFEQ, startAdres, offset, ConditionType.EQUAL, true);
				} break; 
				case 0x9a : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareInt(Operation.OP_IFNE, startAdres, offset, ConditionType.NOT_EQUAL, true);
				} break; 
				case 0x9b : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareInt(Operation.OP_IFLT, startAdres, offset, ConditionType.LESS_THEN, true);
				} break; 
				case 0x9c : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareInt(Operation.OP_IFGE, startAdres, offset, ConditionType.GREATER_OR_EQUAL, true);
				} break; 
				case 0x9d : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareInt(Operation.OP_IFGT, startAdres, offset, ConditionType.GREATER_THEN, true);
				} break; 
				case 0x9e : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareInt(Operation.OP_IFLE, startAdres, offset, ConditionType.LESS_OR_EQUAL, true);
				} break; 
				case 0x9f : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareInt(Operation.OP_IF_ICMPEQ, startAdres, offset, ConditionType.EQUAL, false);
				} break; 
				case 0xa0 : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareInt(Operation.OP_IF_ICMPNE, startAdres, offset, ConditionType.NOT_EQUAL, false);
				} break; 
				case 0xa1 : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareInt(Operation.OP_IF_ICMPLT, startAdres, offset, ConditionType.LESS_THEN, false);
				} break; 
				case 0xa2 : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareInt(Operation.OP_IF_ICMPGE, startAdres, offset, ConditionType.GREATER_OR_EQUAL, false);
				} break; 
				case 0xa3 : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareInt(Operation.OP_IF_ICMPGT, startAdres, offset, ConditionType.GREATER_THEN, false);
				} break; 
				case 0xa4 : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareInt(Operation.OP_IF_ICMPLE, startAdres, offset, ConditionType.LESS_OR_EQUAL, false);
				} break;
				case 0xa5 : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareReference(Operation.OP_IF_ACMPEQ, startAdres, offset, ConditionType.EQUAL, false);
				} break;
				case 0xa6 : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareReference(Operation.OP_IF_ACMPNE, startAdres, offset, ConditionType.NOT_EQUAL, false);
				} break;
				case 0xa7 : {
					short offset = (short) ((0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpGoto(Operation.OP_GOTO, startAdres, 3, offset);
				} break;
				case 0xa8 : {
					lines.add("jsr");
				} break;
				case 0xa9 : {
					lines.add("ret");
				} break;
				case 0xaa : {
					while(bytecodePtr%4!=0) {
						bytecodePtr++;
					}
					int defaultByte = 0xFF & bytecode[bytecodePtr++];
					defaultByte = (defaultByte<<8) | (0xFF & bytecode[bytecodePtr++]);
					defaultByte = (defaultByte<<8) | (0xFF & bytecode[bytecodePtr++]);
					defaultByte = (defaultByte<<8) | (0xFF & bytecode[bytecodePtr++]);

					int lowByte = 0xFF & bytecode[bytecodePtr++];
					lowByte = (lowByte<<8) | (0xFF & bytecode[bytecodePtr++]);
					lowByte = (lowByte<<8) | (0xFF & bytecode[bytecodePtr++]);
					lowByte = (lowByte<<8) | (0xFF & bytecode[bytecodePtr++]);

					int highByte = 0xFF & bytecode[bytecodePtr++];
					highByte = (highByte<<8) | (0xFF & bytecode[bytecodePtr++]);
					highByte = (highByte<<8) | (0xFF & bytecode[bytecodePtr++]);
					highByte = (highByte<<8) | (0xFF & bytecode[bytecodePtr++]);

					int count = highByte-lowByte+1;
					int table[] = new int[count];
					log.trace("nr.etnries="+count);
					for(int idx=0; idx<count; idx++) {
						int val = 0xFF & bytecode[bytecodePtr++];
						val = (val<<8) | (0xFF & bytecode[bytecodePtr++]);
						val = (val<<8) | (0xFF & bytecode[bytecodePtr++]);
						val = (val<<8) | (0xFF & bytecode[bytecodePtr++]);
						val+=startAdres;
						table[idx] = val;
					}
					
					opperation = new OpTableSwitch(startAdres, bytecodePtr-startAdres, defaultByte, lowByte, highByte, table);
					lines.add("tableswitch");
				} break;
				case 0xab : {
					lines.add("lookupswitch");
				} break;
				case 0xac : opperation = new OpReturn(Operation.OP_IRETURN, startAdres, 1); break;
				case 0xad : opperation = new OpReturn(Operation.OP_LRETURN, startAdres, 1); break;
				case 0xae : opperation = new OpReturn(Operation.OP_FRETURN, startAdres, 1); break;
				case 0xaf : opperation = new OpReturn(Operation.OP_DRETURN, startAdres, 1); break;
				case 0xb0 : opperation = new OpReturn(Operation.OP_ARETURN, startAdres, 1); break;
				case 0xb1 : opperation = new OpReturn(Operation.OP_RETURN, startAdres, 1); break;
				case 0xb3 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpPut(Operation.OP_PUTSTATIC, startAdres, 3, index);
				} break;
				case 0xb5 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpPut(Operation.OP_PUTFIELD, startAdres, 3, index);
				} break;
				case 0xb2 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpGet(Operation.OP_GETSTATIC, startAdres, 3, index);
				} break; 
				case 0xb4 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpGet(Operation.OP_GETFIELD, startAdres, 3, index);
				} break;
				case 0xb6 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpInvoke(Operation.OP_INVOKEVIRTUAL, startAdres, index);
				} break;
				case 0xb7 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpInvoke(Operation.OP_INVOKESPECIAL, startAdres, index);
				} break;
				case 0xb8 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpInvoke(Operation.OP_INVOKESTATIC, startAdres, index);
				} break;
				case 0xb9 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					int count = 0xff & bytecode[bytecodePtr++];
					bytecodePtr++;
					opperation = new OpInvoke(startAdres, index, count);
				} break;
				case 0xba : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]);
					bytecodePtr+=2;
					opperation = new OpInvoke(Operation.OP_INVOKEDYNAMIC, startAdres, index);
				} break;
				case 0xbb : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpNew(startAdres, 3, index);
				} break;
				case 0xbc : {
					int typeVal= 0xFF & bytecode[bytecodePtr++];
					Type type = Type.INT;
					switch(typeVal) {
						case 4 : type=Type.BOOLEAN; break;
						case 5 : type=Type.CHAR; break;
						case 6 : type=Type.FLOAT; break;
						case 7 : type=Type.DOUBLE; break;
						case 8 : type=Type.BYTE; break;
						case 9 : type=Type.SHORT; break;
						case 10 : type=Type.INT; break;
						case 11 : type=Type.LONG; break;
					}
					opperation = new OpNewArray(Operation.OP_NEWARRAY, startAdres, 2, type, 0);
				} break;
				case 0xbd : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpNewArray(Operation.OP_ANEWARRAY, startAdres, 3, Type.REFERENCE, index);
				} break;
				case 0xbe : {
					opperation = new OpArrayLength(startAdres,1 );
				} break;
				case 0xbf : {
					opperation = new OpThrow(startAdres, 1);
				} break;
				case 0xc0 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpCheckCast(Operation.OP_CHECKCAST, startAdres, 3, index);
				} break;
				case 0xc1 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					opperation = new OpCheckCast(Operation.OP_INSTANCEOF, startAdres, 3, index);
				} break;
				case 0xc2 : opperation = new OpMonitor(Operation.OP_MONITORENTER, startAdres, 1); break;
				case 0xc3 : opperation = new OpMonitor(Operation.OP_MONITOREXIT, startAdres, 1); break;
				case 0xc4 : {
					isWide = true;
					continue;
				}
				case 0xc5 : {
					int index = (0xFF & bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++]); 
					int dimensions = 0xFF & bytecode[bytecodePtr++];
					opperation = new OpMultiANewArray(startAdres, 4, index, dimensions);
				} break;
				case 0xc6 : {
					short offset = (short) ((bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareReference(Operation.OP_IFNULL, startAdres, offset, ConditionType.EQUAL, true);
				} break; 
				case 0xc7 : {
					short offset = (short) ((bytecode[bytecodePtr++])<<8 | (0xFF & bytecode[bytecodePtr++])); 
					opperation = new OpCompareReference(Operation.OP_IFNONNULL, startAdres, offset, ConditionType.NOT_EQUAL, true);
				} break;
				case 0xc8 : {
					int branchOffset = 0xFF & bytecode[bytecodePtr++];
					branchOffset = (branchOffset<<8) | (0xFF & bytecode[bytecodePtr++]);
					branchOffset = (branchOffset<<8) | (0xFF & bytecode[bytecodePtr++]);
					branchOffset = (branchOffset<<8) | (0xFF & bytecode[bytecodePtr++]);
					opperation = new OpGoto(Operation.OP_GOTO_W, startAdres, 5, branchOffset);
				} break;
				case 0xc9 : {
					lines.add("jsr_w");
				} break;
				default : {
					Integer b = new Integer(0xff & opcode);
					lines.add(""+Integer.toHexString(b));
				}
			}
			if (opperation!=null) {
				lines.add(opperation.toString());
				operations.add(opperation);
				isWide = false;
				
			} else {
				log.error("unresolved:opcode="+TextUtil.hex(opcode,2));
			}
			
			
			
			
//			String adrTxt = "0000"+Integer.toHexString(startAdres);
//			adrTxt = adrTxt.substring(adrTxt.length()-4);
//			String opCodeTxt = "00"+Integer.toHexString(opcode);
//			opCodeTxt = opCodeTxt.substring(opCodeTxt.length()-2);
//		
//			
//			
//			System.out.println(""+VirtualRuntime.hex((operations.size()-1),4)+" | "+ adrTxt+" | "+opCodeTxt+"     :: "+lines.get(lines.size()-1));
		}
		return operations;
	}

	
	public void dumpCode(PrintStream out) {
	
		
		log.logByteArray(LogLevel.DETAIL, bytecode, 0l, 4, 32, true);
	
		labelRepository.dump();
		
		StringBuilder buf = dump("");
		
		log.debug(buf.toString());
	}


	public StringBuilder dump(String prefix) {
		StringBuilder buf = new StringBuilder();
		
		int oppIndex = 0;
		for(IMnemonic opperation : opperationList) {
			Label label = labelRepository.get(oppIndex);
			if (label != null && label.getType()==LabelType.HARD) {
//			if (label != null) {
				buf.append(prefix).append(""+label).append(Logger.ln);
			}
			buf.append(prefix).append(createInfo(oppIndex, opperation)).append(Logger.ln);
			oppIndex++;
		}
		return buf;
	}
	
	 
	private String createInfo(int pc, IMnemonic opperation) {
		StringBuilder buf = new StringBuilder("    ");
		
		String txtPc = "       "+pc;
		
		buf.append(txtPc.substring(txtPc.length()-5)).append(" | ");
		buf.append(TextUtil.hex(opperation.getOffset(), 4)).append(" | ");
		
		int cnt = 0;
		int endOffset = opperation.getOffset()+opperation.getLength();
		for(int bytePtr=opperation.getOffset(); bytePtr<endOffset; bytePtr++) {
			buf.append(TextUtil.hex(bytecode[bytePtr],2)).append(" ");
			cnt+=3;
		}
		
		while(cnt++<20) {
			buf.append(" ");
		}
//		buf.append(opperation.getLength()+"/");
		buf.append(opperation.toString(this));
		return buf.toString();
	}
	
	@Override
	public Iterator<IMnemonic> iterator() {
		return opperationList.iterator();
	}
	
	public IMnemonic get(int index) {
		return opperationList.get(index);
	}


	public Label findOrCreatLabelAt(int oppIndex) {
		Label label = labelRepository.get(oppIndex);
		if (label==null) {
			int bytecodeOffset = -1;
			if (oppIndex>=opperationList.size()) {
				bytecodeOffset = opperationList.get(opperationList.size()-1).getContinuesOffset();
			} else {
				bytecodeOffset = opperationList.get(oppIndex).getOffset();
			}
			label = labelRepository.createSoft(oppIndex, bytecodeOffset);
		}
		return label;
	}

	
	public int findByBytecodeOffset(int bytecodeOffset) {
		int index = 0;
		for(IMnemonic opp : opperationList) {
			if (bytecodeOffset>=opp.getOffset() && bytecodeOffset<opp.getOffset()+opp.getLength()) {
				return index; 
			}
			index++;
		}
		return opperationList.size();
	}
	
	
	public int count() {
		return opperationList.size();
	}

}
