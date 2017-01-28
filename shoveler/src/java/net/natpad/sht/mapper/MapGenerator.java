package net.natpad.sht.mapper;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;

public class MapGenerator {

	public MapGenerator() {
		File file = new File("./terconfigmapper.c");
		try {
			PrintWriter out = new PrintWriter(file);
			out.println("static TerDocumentClazz *l_map_document_clazz(ShoMapContext *map_context, ShoBlockValue *block) {");
			out.println("	TerDocumentClazz *result = NULL;");
			out.println("	CatFixedString *name = sho_map_context_get_string(map_context, block, cat_fixed_string_new(\"name\"));");
			out.println("	CatFixedString *grammar = sho_map_context_get_string(map_context, block, cat_fixed_string_new(\"grammar\"));");
			out.println("	return result;");
			out.println("}");
			
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	
	public static void main(String[] args) {
		new MapGenerator();
	}
	
}
