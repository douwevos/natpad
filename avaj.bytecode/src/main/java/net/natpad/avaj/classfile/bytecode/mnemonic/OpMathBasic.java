package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;
import net.natpad.avaj.classfile.bytecode.Operator;
import net.natpad.avaj.classfile.bytecode.Type;


public class OpMathBasic extends AbstractMnemonic {

	public enum Opperation { 
		ADD,SUB,MUL,DIV,REM,NEG,BITWISE_OR;
	}
	
	public final Operator operator;
	public final Type type;
	
	public OpMathBasic(Operation operation, int offset, Type type, Operator operator) {
		super(operation, offset, 1);
		this.operator = operator;
		this.type = type;
//		switch(operation) {
//			case OP_IADD : type=Type.INT; operator=Operator.ADD; break;
//			case OP_LADD : type=Type.LONG; operator=Operator.ADD; break;
//			case OP_FADD : type=Type.FLOAT; operator=Operator.ADD; break;
//			case OP_DADD : type=Type.DOUBLE; operator=Operator.ADD; break;
//			case OP_ISUB : type=Type.INT; operator=Operator.SUB; break;
//			case OP_LSUB : type=Type.LONG; operator=Operator.SUB; break;
//			case OP_FSUB : type=Type.FLOAT; operator=Operator.SUB; break;
//			case OP_DSUB : type=Type.DOUBLE; operator=Operator.SUB; break;
//			case OP_IMUL : type=Type.INT; operator=Operator.MULTIPLY; break;
//			case OP_LMUL : type=Type.LONG; operator=Operator.MULTIPLY; break;
//			case OP_FMUL : type=Type.FLOAT; operator=Operator.MULTIPLY; break;
//			case OP_DMUL : type=Type.DOUBLE; operator=Operator.MULTIPLY; break;
//			case OP_IDIV : type=Type.INT; operator=Operator.DIVIDE; break;
//			case OP_LDIV : type=Type.LONG; operator=Operator.DIVIDE; break;
//			case OP_FDIV : type=Type.FLOAT; operator=Operator.DIVIDE; break;
//			case OP_DDIV : type=Type.DOUBLE; operator=Operator.DIVIDE; break;
//			case OP_IREM : type=Type.INT; operator=Operator.REMAINDER; break;
//			case OP_LREM : type=Type.LONG; operator=Operator.REMAINDER; break;
//			case OP_FREM : type=Type.FLOAT; operator=Operator.REMAINDER; break;
//			case OP_DREM : type=Type.DOUBLE; operator=Operator.REMAINDER; break;
//			case OP_INEG : type=Type.INT; operator=Operator.UNARY_MINUS; break;
//			case OP_LNEG : type=Type.LONG; operator=Operator.UNARY_MINUS; break;
//			case OP_FNEG : type=Type.FLOAT; operator=Operator.UNARY_MINUS; break;
//			case OP_DNEG : type=Type.DOUBLE; operator=Operator.UNARY_MINUS; break;
//			default : type=Type.DOUBLE; operator=Operator.UNARY_MINUS; break;
//		}
	}

	
	@Override
	public String toString() {
		return operation.text;
	}
}
