package net.natpad.avaj.ast;
import net.natpad.avaj.ast.formater.Writer;
import net.natpad.avaj.classfile.bytecode.Type;
import net.natpad.avaj.util.Util;

public class DeclarationType {

	public static final DeclarationType DEFAULT_OBJECT = new DeclarationType(new Name("java.lang.Object", false), 0);

	public static final DeclarationType DEFAULT_INT = new DeclarationType(Type.INT, 0);

	public static final DeclarationType DEFAULT_LONG = new DeclarationType(Type.LONG, 0);

	public static final DeclarationType DEFAULT_CHAR = new DeclarationType(Type.CHAR, 0);

	public static final DeclarationType DEFAULT_BOOLEAN = new DeclarationType(Type.BOOLEAN, 0);

	public static final DeclarationType DEFAULT_STRING = new DeclarationType(new Name("java.lang.String", false), 0);

	public static final DeclarationType DEFAULT_CLASS = new DeclarationType(new Name("java.lang.Class", false), 0);


	public final Type type;
	public final Name referenceTypeName;
	public final int dimCount;
	
	public DeclarationType(Type primitiveType, int dimCount) {
		if (primitiveType==Type.REFERENCE) {
			throw new RuntimeException("expected a name with the type REFERENCE");
		}
		this.type = primitiveType;
		this.referenceTypeName = null;
		this.dimCount = dimCount;
	}
	
	public DeclarationType(Name refrenceTypeName, int dimCount) {
		this.type = Type.REFERENCE;
		this.referenceTypeName = refrenceTypeName;
		this.dimCount = dimCount;
	}

	private DeclarationType(Type type, Name refrenceTypeName, int dimCount) {
		this.type = type;
		this.referenceTypeName = refrenceTypeName;
		this.dimCount = dimCount;
	}
	
	public DeclarationType setDimCount(int newDimCount) {
		if (dimCount==newDimCount) {
			return this;
		}
		return new DeclarationType(type, referenceTypeName, newDimCount);
	}

	
	public boolean isReference() {
		return type==Type.REFERENCE;
	}

	public boolean isVoid() {
		return type==Type.VOID;
	}

	public boolean isPrimitive() {
		return type!=Type.VOID && type!=Type.REFERENCE;
	}

	
	public boolean isCategory2() {
		return type == Type.LONG || type == Type.DOUBLE;
	}
	
	
	@Override
	public int hashCode() {
		return 13*type.hashCode()
				+((referenceTypeName==null) ? 0 : referenceTypeName.hashCode())
				+dimCount*101;
	}
	
	@Override
	public boolean equals(Object other) {
		if (other==this) {
			return true;
		}
		if (other instanceof DeclarationType) {
			DeclarationType that = (DeclarationType) other;
			return that.type==type && that.dimCount==dimCount 
					&& Util.nullSafeEquals(that.referenceTypeName, referenceTypeName);
		}
		return false;
	}



	public void write(Writer out) {
		if (isPrimitive()) {
			out.print(type.name().toLowerCase());
		} else if (isVoid()) {
			out.print("void");
			return;
		} else {
			out.print(referenceTypeName.createFQN());
		}
	//	out.print(" ");
		for(int idx=0; idx<dimCount; idx++) {
			out.print("[]");
		}
	}

	
	@Override
	public String toString() {
		StringBuilder buf = new StringBuilder("DeclarationType[");
		if (type==Type.REFERENCE) {
			buf.append(referenceTypeName.createFQN());
			if (dimCount>0) {
				buf.append("|"+dimCount);
			}
		} else if (type==Type.VOID) {
			buf.append("void");
		} else {
			buf.append(type);
			if (dimCount>0) {
				buf.append("|"+dimCount);
			}
		}
		buf.append("]");
		return buf.toString();
	}

	
}
