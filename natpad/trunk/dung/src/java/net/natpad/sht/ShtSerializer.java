package net.natpad.sht;

import net.natpad.sht.model.BlockValue;
import net.natpad.sht.model.ListValue;
import net.natpad.sht.model.MapValue;
import net.natpad.sht.model.Model;
import net.natpad.sht.model.Reference;
import net.natpad.sht.model.SimpleValue;
import net.natpad.sht.model.Value;

public class ShtSerializer {

	int indent;
	
	
	
	public void serializeReferences(MapValue references, StringBuilder buf) {
		
		boolean didAdd = false;
		
		for(Reference ref : references) {
			if (!didAdd) {
				didAdd = true;
				if (!(references instanceof Model)) {
					doIndent(buf);
					buf.append("{\n");
					indent++;
				}
			}
			
			doIndent(buf);
			serializeReference(ref, buf, true);
			buf.append("\n");
		}
		
		if (didAdd) {
			if (!(references instanceof Model)) {
				indent--;
				doIndent(buf);
				buf.append('}');
			}
		}
		
	}



	private void serializeReference(Reference ref, StringBuilder buf, boolean b) {
		buf.append(ref.refId);
		buf.append(' ');
		Value value = ref.getValue();
		serializeValue(value, buf, true);
	}


	private void serializeValue(Value value, StringBuilder buf, boolean close_with_semicolon) {
		if (value==null) {
			if (close_with_semicolon) {
				buf.append("nil;");
			} else {
				buf.append("nil");
			}
		} else if (value instanceof BlockValue) {
			serializeReferences((BlockValue) value, buf);
		} else if (value instanceof SimpleValue) {
			SimpleValue simpleValue = (SimpleValue) value;
			seriailizeSimpleValue(simpleValue, buf);
			if (close_with_semicolon) {
				buf.append(";");
			}
		} else if (value instanceof ListValue) {
			ListValue listValue = (ListValue) value;
			serializeListValue(listValue, buf);
		}		
	}



	private void serializeListValue(ListValue listValue, StringBuilder buf) {
		boolean is_first = true;
		buf.append("{");
		for(Value value : listValue) {
			if (is_first) {
				is_first = false;
			} else {
				buf.append(", ");
			}
			serializeValue(value, buf, false);
		}
		buf.append("}");
		
	}



	private void seriailizeSimpleValue(SimpleValue simpleValue, StringBuilder buf) {
		if (simpleValue.value==null) {
			buf.append("nil");
		} else {
			encode(buf, simpleValue.value, '"');
		}
	}



	private void encode(StringBuilder buf, String in, char marker) {
		int cnt = in.length();
		int idx;
		buf.append(marker);
		for(idx=0; idx<cnt; idx++) {
			char ch = in.charAt(idx);
			if (ch==marker) {
				buf.append('\\');
				buf.append(marker);
			} else {
				switch(ch) {
					case '\\' : buf.append("\\\\"); break;
					case 7 : buf.append("\\a"); break;
					case 8 : buf.append("\\b"); break;
					case 9 : buf.append("\\t"); break;
					case 10 : buf.append("\\n"); break;
					case 11 : buf.append("\\v"); break;
					case 12 : buf.append("\\f"); break;
					case 13 : buf.append("\\r"); break;
					case 27 : buf.append("\\e"); break;
					default : buf.append(ch); break;
				}
			}
		}
		buf.append(marker);
		
	}



	private void doIndent(StringBuilder buf) {
		for(int idx=0; idx<indent; idx++) {
			buf.append("  ");
		}
	}
	
}
