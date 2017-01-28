package net.natpad.avaj.classfile.constant;

public interface IConstantProvider {

	public IConstant get(int index);

	public String getConstantUtf8Text(int index);
}
