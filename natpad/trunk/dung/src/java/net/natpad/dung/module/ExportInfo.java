package net.natpad.dung.module;

import net.natpad.dung.expression.IExpressionValue;
import net.natpad.dung.module.model.ExportDescr;
import net.natpad.dung.module.model.ExportHeadersDescr;
import net.natpad.dung.module.model.ExportLibraryDescr;

public class ExportInfo implements IExpressionValue {

	public ExportHeadersDescr headers;
	public ExportLibraryDescr lib;
	
	public final ExportLibrary library;
	
	public ExportInfo(ExportDescr descr) {
		headers = descr.headers;
		lib = descr.lib;
		library = descr.lib==null ? null : new ExportLibrary(descr.lib);
	}
	
	
	@Override
	public IExpressionValue getById(Object id) {
		if (id instanceof String) {
			String st = (String) id;
			if ("library".equals(st)) {
				return library;
			}
		}
		return null;
	}
	
}
