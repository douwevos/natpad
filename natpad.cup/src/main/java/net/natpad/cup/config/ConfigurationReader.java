package net.natpad.cup.config;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.lang.reflect.Field;

import net.natpad.cup.export.caterpillar.CNameTupple;
import net.natpad.cup.export.caterpillar.CaterpillarConfiguration;
import net.natpad.cup.export.caterpillar.CaterpillarCupExporter;
import net.natpad.cup.export.java.JavaCupExporter;
import net.natpad.cup.export.java.JavaExportConfiguration;
import net.natpad.cup.export.java.ast.AstQualifiedName;
import net.natpad.cup.export.purec.PureCConfiguration;
import net.natpad.cup.export.purec.PureCCupExporter;
import net.natpad.json.IJsonItem;
import net.natpad.json.JsonArray;
import net.natpad.json.JsonException;
import net.natpad.json.JsonObject;
import net.natpad.json.JsonParser;


public class ConfigurationReader {

	public final Reader reader;
	
	
	public ConfigurationReader(File file) throws IOException {
		reader = new FileReader(file);
	}

	public ConfigurationReader(Reader reader) throws IOException {
		this.reader = reader;
	}

	
	public CupConfiguration readConfiguration(CupConfiguration configuration) throws IOException, JsonException {
		JsonParser jsonParser = new JsonParser(reader);
		IJsonItem jsonItem = jsonParser.parse();
		JsonObject mainObj = (JsonObject) jsonItem;
		JsonArray generators = mainObj.getArray("generators");
		if (generators!=null) {
			for(IJsonItem item : generators) {
				ICupExporter readExporter = readExporter(configuration, (JsonObject) item);
				if (readExporter!=null) {
					configuration.addExporter(readExporter);
				}
			}
		}
		return configuration;
	}


	private ICupExporter readExporter(CupConfiguration cupConfiguration, JsonObject genObject) throws JsonException {
		String type = genObject.getString("type");
		if ("java".equals(type)) {
			return readJavaGenerator(cupConfiguration, genObject);
		} else if ("caterpillar".equals(type)) {
			return readCaterpillarExporter(cupConfiguration, genObject);
		} else if ("purec".equals(type)) {
			return readPureCExporter(cupConfiguration, genObject);
		} else {
			throw new JsonException("Unknown generator type:"+type);
		}
//		return null;
		
	}


	private JavaCupExporter readJavaGenerator(CupConfiguration cupConfiguration, JsonObject genObject)  {
		JavaExportConfiguration exportConfiguration = new JavaExportConfiguration();
		autoReadFields(exportConfiguration, genObject);
		return new JavaCupExporter(cupConfiguration, exportConfiguration);
	}

	private CaterpillarCupExporter readCaterpillarExporter(CupConfiguration cupConfiguration, JsonObject genObject)  {
		CaterpillarConfiguration config = new CaterpillarConfiguration();
		autoReadFields(config, genObject);
		return new CaterpillarCupExporter(cupConfiguration, config);
	}
	
	private PureCCupExporter readPureCExporter(CupConfiguration cupConfiguration, JsonObject genObject)  {
		PureCConfiguration config = new PureCConfiguration();
		autoReadFields(config, genObject);
		return new PureCCupExporter(cupConfiguration, config);
	}
	

	private void autoReadFields(Object configurable, JsonObject genObject)  {
//		System.out.println("key-count:"+genObject.count());
		for(String key : genObject.keys()) {
			try {
				Field field = tryGetField(configurable, key);
				if (field==null) {
					String lkey = key.replaceAll("-", "_");
					field = tryGetField(configurable, lkey);
					if (field==null) {
						lkey = createCamelCase(key);
						field = tryGetField(configurable, lkey);
					}
				}
//				System.out.println("key:"+key+", field="+field);
				if (field!=null) {
					field.setAccessible(true);
					Class<?> fieldType = field.getType();
					if (fieldType.equals(int.class)) {
						field.setInt(configurable, genObject.getInt(key));
					} else if (fieldType.equals(long.class)) {
						field.setLong(configurable, genObject.getLong(key));
					} else if (fieldType.equals(boolean.class)) {
						field.setBoolean(configurable, genObject.getBoolean(key));
					} else if (fieldType.equals(String.class)) {
						field.set(configurable, genObject.getString(key));
					} else if (fieldType.equals(File.class)) {
						field.set(configurable, new File(genObject.getString(key)));
					} else if (fieldType.equals(AstQualifiedName.class)) {
						field.set(configurable, new AstQualifiedName(genObject.getString(key)));
					} else if (fieldType.equals(CNameTupple.class)) {
						field.set(configurable, new CNameTupple(genObject.getString(key)));
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	
	private Field tryGetField(Object object, String fieldName) {
		try {
			return object.getClass().getDeclaredField(fieldName);
		} catch (Exception e) {
		}
		return null;
	}

	private String createCamelCase(String key) {
		StringBuilder buf = new StringBuilder();
		boolean nextIsCap = false;
		for(int idx=0; idx<key.length(); idx++) {
			char ch = key.charAt(idx);
			if (ch=='-') {
				nextIsCap = true;
			} else {
				buf.append(nextIsCap ? Character.toUpperCase(ch) : ch);
				nextIsCap = false;
			}
		}
		return buf.toString();
	}

}
