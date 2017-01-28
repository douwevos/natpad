package net.natpad.dung.module.dependency;

import net.natpad.dung.expression.IExpressionValue;
import net.natpad.dung.expression.StringLiteral;
import net.natpad.dung.module.IDependency;
import net.natpad.dung.module.task.cc.CcCompilerSettings;
import net.natpad.dung.module.task.cc.CcLinkerSettings;
import net.natpad.dung.module.task.cc.ICcConfigItem;
import net.natpad.dung.run.Session;
import net.natpad.dung.workspace.ModuleRaw;

public class RawDependency implements ICcConfigItem, IDependency {

	private final ModuleRaw raw;
	
	public RawDependency(ModuleRaw raw) {
		this.raw = raw;
	}
	
	@Override
	public IExpressionValue getById(Object id) {
		if (id instanceof String) {
			String st = (String) id;
			if ("libPath".equals(st)) {
				return new StringLiteral(raw.libPath);
			} else if ("libName".equals(st)) {
				return new StringLiteral(raw.libName);
			} else if ("headerPath".equals(st)) {
				return new StringLiteral(raw.headerPath);
			}
		}
		return null;
	}

	@Override
	public IDependency[] children() {
		return null;
	}

	@Override
	public String description() {
		return "Raw[]";
	}

	@Override
	public void setup(Session session, CcCompilerSettings compilerSettings) {
		System.err.println("raw.headerPath="+raw.headerPath);
		if (raw.headerPath!=null) {
			String resolved = session.resolveProperties(raw.headerPath);
			System.err.println("resolved="+resolved);
			compilerSettings.addArgumentOnce("-I"+resolved, false);
		}
	}

	@Override
	public void setup(Session session, CcLinkerSettings linkerSettings) {
		if (raw.libName!=null) {
			String resolved = session.resolveProperties(raw.libName);
			linkerSettings.addLibName(resolved);
		}
		if (raw.libPath!=null) {
			String resolved = session.resolveProperties(raw.libPath);
			linkerSettings.addLibSearchPath(resolved);
			
		}
	}
}
