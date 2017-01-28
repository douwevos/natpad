package net.natpad.dung.module.task;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Path;

import net.natpad.dung.run.Session;


public class XmlToHInclude extends Task {

	String file;

	
	@Override
	public void runTask(Session session) throws Exception {
		Path srcPath = session.createModulePath(file);
		File fileIn = srcPath.toFile();
		String name = fileIn.getName();
		String basename = name.substring(0, name.length()-6);
		String hname = basename + ".h";
		File hfile = new File(fileIn.getParentFile(), hname);
		if (!hfile.exists() || hfile.lastModified()<fileIn.lastModified()) {
			try {
				String xmlText = session.context.readFile(fileIn);
				xmlText = stripWhitespaces(xmlText);
				
				String htext = asInclude(basename, xmlText);
				FileWriter writer = new FileWriter(hfile);
				writer.write(htext);
				writer.flush();
				writer.close();
			} catch (IOException e) {
				throw new RuntimeException("input file not found:"+fileIn.getAbsolutePath());
			}
		}
	}
	
	private String asInclude(String varname, String xmlText) {
		StringBuilder buf = new StringBuilder();
		
		buf.append("unsigned char "+varname+"_glade[] = {\n");
		
		int tl = xmlText.length()+3;
		tl = tl - (tl%4);

		for(int idx=0; idx<tl; idx++) {
			if ((idx%32)==0) {
				buf.append("\t\t");
			}
			boolean isEod = (idx==tl-1);
			int ch = idx>=xmlText.length() ? 0 : (int) xmlText.charAt(idx);
			String hval = "00"+Integer.toHexString(ch & 0xFF);
			hval = hval.substring(hval.length()-2);
			buf.append("0x").append(hval);
			if (!isEod) {
				buf.append(',');
			}
			if (((idx%32)==31) || isEod) {
				buf.append("\n");
			}
		}
		buf.append("};\n");
		buf.append("unsigned int "+varname+"_glade_len = "+xmlText.length()+";\n");
		return buf.toString();
	}

	private String stripWhitespaces(String xmlText) {
		StringBuilder buf = new StringBuilder();
		char markCh = 0;
		char ch = 0;
		char lastCh = 0;
		for(int idx=0; idx<xmlText.length(); idx++) {
			lastCh = ch;
			ch = xmlText.charAt(idx);
			if (markCh!=0) {
				if (markCh==ch) {
					markCh = 0;
				}
			} else { 
				if ((ch == '"') || (ch == '\'')) {
					markCh = ch;
				} else if ((lastCh==' ') && (ch==' ')) {
					continue;
				}
			}
			buf.append(ch);
		}
		return buf.toString();
	}
	
}
