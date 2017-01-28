package net.natpad.avaj.classfile.bytecode.mnemonic;

import net.natpad.avaj.classfile.bytecode.AbstractMnemonic;
import net.natpad.avaj.classfile.bytecode.Operation;
//import net.natpad.avaj.vm.tree.FieldPrimitive;
import net.natpad.avaj.classfile.bytecode.Type;

public class OpLoadConstant extends AbstractMnemonic {

	
	public final Type type;
	public final short value;
	public Number object;
	
	public OpLoadConstant(Operation operation, Type type, Number value, int offset) {
		super(operation, offset, 1);
		this.value = value.shortValue();
		this.type = type;
		this.object = value;
//		switch(operation) {
//			case OP_ICONST_M1 : type=Type.INT; value=-1; object=new Integer(-1); break;
//			case OP_ICONST_0 : type=Type.INT; value=0; object=new Integer(0); break;
//			case OP_ICONST_1 : type=Type.INT; value=1; object=new Integer(1); break;
//			case OP_ICONST_2 : type=Type.INT; value=2; object=new Integer(2); break;
//			case OP_ICONST_3 : type=Type.INT; value=3; object=new Integer(3); break;
//			case OP_ICONST_4 : type=Type.INT; value=4; object=new Integer(4); break;
//			case OP_ICONST_5 : type=Type.INT; value=5; object=new Integer(5); break;
//			case OP_LCONST_0 : type=Type.LONG; value=0; object=new Long(0l); break;
//			case OP_LCONST_1 : type=Type.LONG; value=1; object=new Long(1l); break;
//			case OP_FCONST_0 : type=Type.FLOAT; value=0; object=new Float(0f); break;
//			case OP_FCONST_1 : type=Type.FLOAT; value=1; object=new Float(1f); break;
//			case OP_FCONST_2 : type=Type.FLOAT; value=2; object=new Float(2f); break;
//			case OP_DCONST_0 : type=Type.DOUBLE; value=0; object=new Double(0d); break;
//			case OP_DCONST_1 : type=Type.DOUBLE; value=1; object=new Double(1d); break;
//			default : type = null; value=0;
//		}
	}
	


//	public FieldPrimitive getPrimitiveType() {
//		return type.getPrimitive();
//	}
//	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder();
		buf.append(operation.text);
		return buf.toString();
	}
	
}
