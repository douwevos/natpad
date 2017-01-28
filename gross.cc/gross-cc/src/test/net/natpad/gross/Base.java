package net.natpad.gross;


import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.junit.Assert;

import net.natpad.gross.ast.AstScanner;
import net.natpad.gross.ast.bnf.AstFile;
import net.natpad.gross.build.CCModel;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.NonTerminalExt;
import net.natpad.gross.build.Production;
import net.natpad.gross.build.ProductionPart;
import net.natpad.gross.build.TerminalExt;
import net.natpad.gross.build.parser.Actions;
import net.natpad.gross.build.parser.GrossParserTables;
import net.natpad.gross.definition.GrossScanner;
import net.natpad.gross.definition.bnf.Specification;
import net.natpad.gross.runtime.DefaultLogger;
import net.natpad.gross.runtime.DefaultTokenFactory;
import net.natpad.gross.runtime.Fork;
import net.natpad.gross.runtime.FullTokenFactory;
import net.natpad.gross.runtime.Message;
import net.natpad.gross.runtime.Parser;
import net.natpad.gross.runtime.ParserContext;
import net.natpad.gross.runtime.ParserContext.ReduceAction;
import net.natpad.gross.runtime.ReduceLayout;
import net.natpad.gross.runtime.Scanner;
import net.natpad.gross.runtime.StateModel;
import net.natpad.gross.runtime.Symbol;
import net.natpad.gross.runtime.Terminal;
import net.natpad.gross.runtime.TokenFactory;

public class Base implements ParserContext.ReduceAction {

	protected HashMap<Integer, ReduceAction> actionMap = new HashMap<Integer, ReduceAction>(); 

	protected DefaultLogger logger = new DefaultLogger(System.out, DefaultLogger.Level.ALL);
	
	
	protected Production addRule(CCModel model, ReduceAction ract, NonTerminalExt ntLhs, Object ... list) {
		ArrayList<ProductionPart> partsList = new ArrayList<ProductionPart>();
		for(int idx=0; idx<list.length; idx+=2) {
			Symbol sym = (Symbol) list[idx];
			String label = (String) list[idx+1];
			ProductionPart part = new ProductionPart(sym, label);
			partsList.add(part);
		}
		Production production = model.addProduction(ntLhs, partsList);
		if (ract!=null) {
			actionMap.put(production.id, ract);
		}
		return production;
	}

	protected Production addRule(CCModel model, NonTerminalExt ntLhs, Object ... list) {
		ArrayList<ProductionPart> partsList = new ArrayList<ProductionPart>();
		for(int idx=0; idx<list.length; idx++) {
			Symbol sym = (Symbol) list[idx];
			ProductionPart part = new ProductionPart(sym, null);
			partsList.add(part);
		}
		Production production = model.addProduction(ntLhs, partsList);
		return production;
	}

	
	protected ParserContext doParse(CCStateMachine stateMachineBuilder, TokenFactory tokenFactory, Scanner scanner) {
		logger.increaseLogLevel();
		logger.increaseLogLevel();
		logger.increaseLogLevel();
		logger.increaseLogLevel();
		Parser parser = new Parser(logger);
		StateModel stateModel = stateMachineBuilder.createStateModel();
		ParserContext context = new ParserContext(tokenFactory, stateModel, (ReduceAction) this);
//		parser.maxInt = Integer.valueOf(30);
		parser.parse(context, scanner);
		return context;
	}

	@Override
	public Object doAction(Fork fork, ReduceLayout reduceLaout, int actionId) {
		ReduceAction action = actionMap.get(actionId);
		if (action!=null) {
			return action.doAction(fork, reduceLaout, actionId);
		}
		return null;
	}
	

	protected void scanAndAccept(CCStateMachine stateMachine, DefaultTokenFactory tokenFactory,
			List<Terminal> scannerTerminals, Integer maxNrOfAccForks) {

		BasicScanner scanner = new BasicScanner(scannerTerminals, stateMachine.model.getEofTerminal());
		ParserContext context = doParse(stateMachine, tokenFactory, scanner);

		List<Fork> acceptedForks = context.getAcceptedForks();
		Assert.assertNotNull("no accepted forks", acceptedForks);
		Assert.assertFalse("no accepted forks", acceptedForks.isEmpty());
		Fork acceptedFork = acceptedForks.get(0);
		
		
		System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> accepted fork");
		acceptedFork.dump();
		
		
		if (maxNrOfAccForks!=null) {
			Assert.assertTrue("Number of accepted-forks:"+acceptedForks.size()+" exceeds expexted:"+maxNrOfAccForks.intValue(), acceptedForks.size()<=maxNrOfAccForks.intValue());
		}
	}

	protected void scanAndNoAccept(CCStateMachine stateMachine, DefaultTokenFactory tokenFactory,
			List<Terminal> scannerTerminals) {

		BasicScanner scanner = new BasicScanner(scannerTerminals, stateMachine.model.getEofTerminal());
		ParserContext context = doParse(stateMachine, tokenFactory, scanner);

		List<Fork> acceptedForks = context.getAcceptedForks();
		Assert.assertTrue("no accepted forks", acceptedForks==null || acceptedForks.isEmpty());
	}

	

	protected void scanAndAccept(CCStateMachine stateMachine, DefaultTokenFactory tokenFactory,
			List<Terminal> scannerTerminals, ReduceAction reduceHandler) {

		BasicScanner scanner = new BasicScanner(scannerTerminals, stateMachine.model.getEofTerminal());
		ParserContext context = doParse(stateMachine, tokenFactory, scanner);

		List<Fork> acceptedForks = context.getAcceptedForks();
		Assert.assertNotNull("no accepted forks", acceptedForks);
		Assert.assertFalse("no accepted forks", acceptedForks.isEmpty());
		Fork acceptedFork = acceptedForks.get(0);
		
		
		System.out.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> accepted fork");
		acceptedFork.dump();
	}

	

	protected CCStateMachine createStateMachine(InputStream stream) throws IOException {
		InputStreamReader reader = new InputStreamReader(stream);
		
		GrossParserTables parserTables = new GrossParserTables();
		StateModel stateModel = new StateModel(parserTables.symbols, parserTables.cfgText, parserTables.stateCount);

		FullTokenFactory tokenFactory = new FullTokenFactory();
		GrossScanner scanner = new GrossScanner(tokenFactory, reader, stateModel);

		Actions actionHandler = new Actions();
		
		ParserContext parserContext = new ParserContext(tokenFactory, stateModel, actionHandler);

		DefaultLogger plogger = new DefaultLogger(System.out, DefaultLogger.Level.DEBUG);
		Parser parser = new Parser(plogger);
		Specification specification = parser.parse(parserContext, scanner);

		CCModel buildModel = specification.createModel();
		CCStateMachine stateMachine = new CCStateMachine(plogger, buildModel);
		stateMachine.build();
		return stateMachine;
	}

	protected AstFile createAstFile(InputStream stream) throws IOException {
		InputStreamReader reader = new InputStreamReader(stream);
		
		net.natpad.ast.build.parser.GrossParserTables parserTables = new net.natpad.ast.build.parser.GrossParserTables();
		StateModel stateModel = new StateModel(parserTables.symbols, parserTables.cfgText, parserTables.stateCount);

		FullTokenFactory tokenFactory = new FullTokenFactory();
		AstScanner scanner = new AstScanner(tokenFactory, reader, stateModel);

		net.natpad.ast.build.parser.Actions actionHandler = new net.natpad.ast.build.parser.Actions();
		
		ParserContext parserContext = new ParserContext(tokenFactory, stateModel, actionHandler);

		DefaultLogger plogger = new DefaultLogger(System.out, DefaultLogger.Level.WARN);
		Parser parser = new Parser(plogger);
		AstFile result = parser.parse(parserContext, scanner);
		if (result==null) {
			if (parserContext.errorForkCount()>0) {
				Fork fork = parserContext.errorForkAt(0);
				Message messages = fork.getMessages();
				while(messages!=null) {
					System.err.println(""+messages.text+" at "+messages.location);
					messages = messages.parent;
				}
			}
		}

		return result;
	}

	
	protected TerminalExt extractTerminal(CCModel model, String symName) {
		Terminal terminalByName = model.getTerminalByName(symName);
		if (terminalByName==null) {
			throw new RuntimeException("Terminal with name:"+symName+" not found in model");
		}
		return (TerminalExt) terminalByName;
	}

	
}
