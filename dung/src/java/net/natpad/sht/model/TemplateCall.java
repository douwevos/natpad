package net.natpad.sht.model;

public class TemplateCall {

	public final String templateName;
	ListValue listValue;
	
	public TemplateCall(String templateName) {
		this.templateName = templateName;
	}

	public void setArgumentList(ListValue el) {
		listValue = el;
	}
	

	public Object getTemplateName() {
		return templateName;
	}

	public Value getValueAt(int idx) {
		if (listValue==null) {
			return null;
		}
		return listValue.get(idx);
	}

}
