package net.natpad.cup.util;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Map;
import java.util.Properties;

public class TextFile {

	public final Properties properties;
	
	public TextFile(Properties props) {
		this.properties = props;
		Map<String, String> map = System.getenv();
		props.putAll(map);
	}
	
	public String readText(File file) throws IOException {
		StringBuilder buf = new StringBuilder();
		FileReader reader = new FileReader(file);
		char cbuf[] = new char[4096];
		while(true) {
			int cnt = reader.read(cbuf);
			if (cnt>0) {
				buf.append(cbuf,0,cnt);
			} else {
				break;
			}
		}
		reader.close();
		return replaceAll(buf.toString());
	}

	private String replaceAll(String input) {
		StringBuilder buf = new StringBuilder();
		int next = 0;
		int lastPos = 0;
		while(true) {
			next = input.indexOf('$', next);
			if (next<0) {
				buf.append(input.substring(lastPos));
				break;
			} else {
				next++;
				if (next>=input.length()) {
					buf.append(input.substring(lastPos));
					break;
				}
				char ch = input.charAt(next);
				if (ch=='(') {
					ch = ')';
				} else if (ch=='{') {
					ch = '}';
				} else if (next=='$') {
					buf.append(input.substring(lastPos, next));
					next++;
					lastPos = next;
					continue;
				} else {
					continue;
				}
				int nextEnd = input.indexOf(ch, next);
				if (nextEnd<0) {
					next++;
					continue;
				}
				String propName = input.substring(next+1, nextEnd);
				String resolved = resolveProperty(propName);
				if (resolved==null) {
					next++;
					continue;
				}
				buf.append(input.substring(lastPos, next-1));
				buf.append(resolved);
				lastPos = nextEnd+1;
			}
		}
		return buf.toString();
	}

	public String resolveProperty(String propName) {
		return properties.getProperty(propName);
	}
	
}
