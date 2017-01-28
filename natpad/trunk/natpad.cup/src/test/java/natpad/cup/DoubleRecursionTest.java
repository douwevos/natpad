package natpad.cup;

import java.io.InputStream;

import org.junit.Test;

import net.natpad.cup.Main;
import net.natpad.cup.config.ICupExporter;
import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.parser.NewLexer;
import net.natpad.cup.state.StateMachine;

public class DoubleRecursionTest {

	@Test
	public void simpleTest() {
		InputStream resourceStream = getClass().getResourceAsStream("double_recursion.cup");
		System.setIn(resourceStream);
		
		try {
			Main main = new Main(new String[] {});
			NewLexer lexer = main.lexer;
			BnfModel model = main.parseGrammarSpec();
			System.out.println("model="+model);
			

			StateMachine stateMachine = null;
//			Emit emit = null;

			
			
			/* don't proceed unless we are error free */
			if (lexer.error_count == 0) {
				lexer.warning_count += model.checkUnused();

				stateMachine = main.buildParser(model);
				stateMachine.dump_tables();
				

				main.dump_machine(stateMachine);
//				emit = new Emit(model);
				
//				for(ICupExporter exporter : cupConfiguration.getExporters()) {
//					exporter.export(model, stateMachine);
//				}
				
//				build_end = System.currentTimeMillis();
	//
//				/* output the generated code, if # of conflicts permits */
//				if (Lexer.error_count != 0) {
//					// conflicts! don't emit code, don't dump tables.
//					opt_dump_tables = false;
//				} else { // everything's okay, emit parser.
//					if (print_progress) System.err.println("Writing parser...");
//					emit.doExport(include_non_terms, sym_interface, stateMachine.action_table, stateMachine.reduce_table, start_state.index(),
//									opt_compact_red, suppress_scanner);
//					
	//
//					did_output = true;
//				}
			}
			
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
}
