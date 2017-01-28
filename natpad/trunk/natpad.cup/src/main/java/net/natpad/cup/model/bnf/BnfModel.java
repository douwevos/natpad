package net.natpad.cup.model.bnf;

import java.util.List;

import net.natpad.cup.TerminalSet;
import net.natpad.cup.export.java.Emit;


public class BnfModel {
	
	public final ProductionList productions = new ProductionList();
	public final NonTerminalList nonTerminals = new NonTerminalList();
	public final TerminalList terminals = new TerminalList();

	protected Production startProduction;
	
	/** special non-terminal for start symbol */
	public final NonTerminal START_nt;

	
	/** Special terminal for end of input. */
	public final Terminal EOF;

	/** special terminal used for error recovery */
	public final Terminal error;


	/** Count of unused terminals. */
	public int unused_term = 0;

	/** Count of unused non terminals. */
	public int unused_non_term = 0;

	/** Number of conflict found while building tables. */
	public int num_conflicts = 0;
	
	public int warningCount = 0;

	
	public BnfModel() {
		EOF = new Terminal(terminals.count(), "EOF");
		terminals.add(EOF);
		error = new Terminal(terminals.count(), "error");
		terminals.add(error);
		
		START_nt = new NonTerminal(getNextNonTerminalIndex(), "$START");
		nonTerminals.add(START_nt);
		
	}
	
	public Production getStartProduction() {
		return startProduction;
	}
	
	
	public void add(Production production) {
		productions.addSafe(production);
		removeEmbeddedActions(production);
//		System.out.println("added production="+production);
	}
	
	
	/**
	 * Remove all embedded actions from a production by factoring them out into
	 * individual action production using new non terminals. if the original
	 * production was:
	 * 
	 * <pre>
	 *    A ::= B {action1} C {action2} D
	 * </pre>
	 * 
	 * then it will be factored into:
	 * 
	 * <pre>
	 *    A ::= B NT$1 C NT$2 D
	 *    NT$1 ::= {action1}
	 *    NT$2 ::= {action2}
	 * </pre>
	 * 
	 * where NT$1 and NT$2 are new system created non terminals.
	 */

	/*
	 * the declarations added to the parent production are also passed along, as
	 * they should be perfectly valid in this code string, since it was
	 * originally a code string in the parent, not on its own. frank 6/20/96
	 */
	protected void removeEmbeddedActions(Production production) {
		NonTerminal new_nt;

		/* walk over the production and process each action */
		for (int act_loc = 0; act_loc < production.rhsLength(); act_loc++)
			if (production.rhs(act_loc).isAction()) {

				List<ActionDeclaration> declarations = production.declareLabels(production._rhs, act_loc);
				
				/* create a new non terminal for the action production */
				new_nt = NonTerminal.createNew(getNextNonTerminalIndex());
				nonTerminals.add(new_nt);
				new_nt.is_embedded_action = true; /* 24-Mar-1998, CSA */

				/* create a new production with just the action */
				ActionProduction new_prod = new ActionProduction(getNextProductionIndex(), production, new_nt, ((ActionPart) production.rhs(act_loc)).code_string());
				new_prod.declarations.addAll(declarations);
				productions.addSafe(new_prod);

				/* replace the action with the generated non terminal */
				production._rhs[act_loc] = new SymbolPart(new_nt);
			}
	}
	
	
	


	/**
	 * Check for unused symbols. Unreduced productions get checked when tables
	 * are created.
	 */
	public int checkUnused() {

		/* check for unused terminals */
		for(Terminal term : terminals) {

			/* don't issue a message for EOF */
			if (term == EOF)
				continue;

			/* or error */
			if (term == error)
				continue;

			/* is this one unused */
			if (term.use_count() == 0) {
				/* count it and warn if we are doing warnings */
				unused_term++;
				if (!Emit.getNowarn()) {
					System.err.println("Warning: Terminal \"" + term.name() + "\" was declared but never used");
					warningCount++;
				}
			}
		}

		/* check for unused non terminals */
		for (NonTerminal nonTerm : nonTerminals) {

			/* is this one unused */
			if (nonTerm.use_count() == 0) {
				/* count and warn if we are doing warnings */
				unused_term++;
				if (!Emit.getNowarn()) {
					System.err.println("Warning: Non terminal \"" + nonTerm.name() + "\" was declared but never used");
					warningCount++;
				}
			}
		}
		return warningCount;
	}

	

	/** Compute nullability of all non-terminals. */
	public void computeNullability() {
		boolean change = true;

		/* repeat this process until there is no change */
		while (change) {
			/* look for a new change */
			change = false;

			/* consider each non-terminal */
			for(NonTerminal nonTerm : nonTerminals) {
				/* only look at things that aren't already marked nullable */
				if (!nonTerm.nullable()) {
					if (nonTerm.looks_nullable()) {
						nonTerm._nullable = true;
						change = true;
					}
				}
			}
		}

		/* do one last pass over the productions to finalize all of them */
		for (Production prod : productions) {
			prod.setNullable(prod.checkNullable());
		}
	}

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Compute first sets for all non-terminals. This assumes nullability has
	 * already computed.
	 */
	public void computeFirstSets() {
		boolean change = true;
		TerminalSet prod_first;

		/* repeat this process until we have no change */
		while(change) {
			/* look for a new change */
			change = false;

			/* consider each non-terminal */
			for (NonTerminal nonTerm : nonTerminals) {

				/* consider every production of that non terminal */
				for(Production prod : nonTerm.productions()) {
					/* get the updated first of that production */
					prod_first = prod.checkFirstSet();

					/* if this going to add anything, add it */
					if (!prod_first.isSubSetOf(nonTerm._first_set)) {
						change = true;
						nonTerm._first_set.add(prod_first);
					}
				}
			}
		}
	}


	public Production createStartProduction(NonTerminal startNonTerminal) {
		ProductionPart parts[] = new ProductionPart[] {
				new SymbolPart(startNonTerminal, "start_val"),
				new SymbolPart(EOF),
				new ActionPart("RESULT = start_val;")
		};
		startProduction = new Production(getNextProductionIndex(), START_nt, parts, parts.length);
		add(startProduction);
		return startProduction;
	}


	public int getNextProductionIndex() {
		return productions.count();
	}

	
	public int getNextNonTerminalIndex() {
		return nonTerminals.count();
	}

	public int getNextTerminalIndex() {
		return terminals.count();
	}

}
