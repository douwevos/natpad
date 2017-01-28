package net.natpad.cup.state;

import java.io.PrintStream;
import java.util.Hashtable;
import java.util.List;
import java.util.Stack;

import net.natpad.cup.FatalCupException;
import net.natpad.cup.ParseActionRow;
import net.natpad.cup.ParseActionTable;
import net.natpad.cup.ParseReduceRow;
import net.natpad.cup.ParseReduceTable;
import net.natpad.cup.config.CupConfiguration;
import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.model.bnf.NonTerminal;
import net.natpad.cup.model.bnf.Production;
import net.natpad.cup.model.bnf.Symbol;
import net.natpad.cup.model.bnf.Terminal;

public class StateMachine {

	protected LalrStateMap stateMap = new LalrStateMap();
	
	public final BnfModel model;
	public final Production startProduction;
	
	public int warningCount = 0;
	public int errorCount = 0;
	
	public LalrState startState;

	/** Resulting parse action table. */
	public ParseActionTable action_table;

	/** Resulting reduce-goto table. */
	public ParseReduceTable reduce_table;

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/**
	 * Hash table to find states by their kernels (i.e, the original, unclosed,
	 * set of items -- which uniquely define the state). This table stores state
	 * objects using (a copy of) their kernel item sets as keys.
	 */
	protected Hashtable<LalrItemSet, LalrState> allKernels = new Hashtable<LalrItemSet, LalrState>();

	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	

	public StateMachine(BnfModel model, Production startProduction) {
		this.model = model;
		this.startProduction = startProduction;
	}


	public void buildMachine(CupConfiguration cupConfiguration) {
		startState = createStates();
//		machine_end = System.currentTimeMillis();

		/* build the LR parser action and reduce-goto tables */
//		if (opt_do_debug || print_progress) System.err.println("  Filling in tables...");
		action_table = new ParseActionTable(this, model);
		reduce_table = new ParseReduceTable(this, model);
		for (LalrState lst : getStateMap()) {
			lst.buildTableEntries(model, action_table, reduce_table);
			warningCount += lst.warningCount;
		}

//		table_end = System.currentTimeMillis();

		/* check and warn for non-reduced productions */
//		if (opt_do_debug || print_progress)
//			System.err.println("  Checking for non-reduced productions...");
		action_table.checkReductions();
		warningCount += action_table.warningCount;

//		reduce_check_end = System.currentTimeMillis();

		/* if we have more conflicts than we expected issue a message and die */
		if (model.num_conflicts > cupConfiguration.getExpectConflicts()) {
			System.err.println("*** More conflicts encountered "+model.num_conflicts +" than expected " + cupConfiguration.getExpectConflicts() +  "-- parser generation aborted");
			errorCount++; // indicate the problem.
			// we'll die on return, after clean up.
		}
		
	}

	/**
	 * Build an LALR viable prefix recognition machine given a start production.
	 * This method operates by first building a start state from the start
	 * production (based on a single item with the dot at the beginning and EOF
	 * as expected lookahead). Then for each state it attempts to extend the
	 * machine by creating transitions out of the state to new or existing
	 * states. When considering extension from a state we make a transition on
	 * each symbol that appears before the dot in some item. For example, if we
	 * have the items:
	 * 
	 * <pre>
	 *    [A ::= a b * X c, {d,e}]
	 *    [B ::= a b * X d, {a,b}]
	 * </pre>
	 * 
	 * in some state, then we would be making a transition under X to a new
	 * state. This new state would be formed by a "kernel" of items
	 * corresponding to moving the dot past the X. In this case:
	 * 
	 * <pre>
	 *    [A ::= a b X * c, {d,e}]
	 *    [B ::= a b X * Y, {a,b}]
	 * </pre>
	 * 
	 * The full state would then be formed by "closing" this kernel set of items
	 * so that it included items that represented productions of things the
	 * parser was now looking for. In this case we would items corresponding to
	 * productions of Y, since various forms of Y are expected next when in this
	 * state (see lalr_item_set.compute_closure() for details on closure).
	 * <p>
	 * 
	 * The process of building the viable prefix recognizer terminates when no
	 * new states can be added. However, in order to build a smaller number of
	 * states (i.e., corresponding to LALR rather than canonical LR) the state
	 * building process does not maintain full loookaheads in all items.
	 * Consequently, after the machine is built, we go back and propagate
	 * lookaheads through the constructed machine using a call to
	 * propagate_all_lookaheads(). This makes use of propagation links
	 * constructed during the closure and transition process.
	 * 
	 * @see net.natpad.cup.state.LalrItemSet#compute_closure
	 * @see net.natpad.cup.state.LalrState#propagate_all_lookaheads
	 */

	public LalrState createStates() {
		LalrState start_state;
		LalrItemSet startItems;
		LalrItemSet new_items;
		LalrItemSet linked_items;
		LalrItemSet kernel;
		Stack<LalrState> workStack = new Stack<LalrState>();
		LalrState workState, new_st;
		SymbolSet outgoing;
		LalrItem new_itm, existing;
		Symbol sym2;

		/* sanity check */
		if (startProduction == null)
			throw new FatalCupException("Attempt to build viable prefix recognizer using a null production");

		/* build item with dot at front of start production and EOF lookahead */
		startItems = new LalrItemSet();

		LalrItem itm = new LalrItem(startProduction);
		itm.getLookaheadSet().add(model.EOF);

		startItems.add(itm);

		/* create copy the item set to form the kernel */
		kernel = new LalrItemSet(startItems);

		/* create the closure from that item set */
		startItems.computeClosure();

		/* build a state out of that item set and put it in our work set */
		start_state = createLalrState(startItems);
		workStack.push(start_state);

		/* enter the state using the kernel as the key */
		allKernels.put(kernel, start_state);

		/* continue looking at new states until we have no more work to do */
		while (!workStack.empty()) {
			/* remove a state from the work set */
			workState = workStack.pop();
			
			/* gather up all the symbols that appear before dots */
			outgoing = new SymbolSet();
			for(LalrItem precedingItem : workState.items()) {
				
				/* add the symbol before the dot (if any) to our collection */
				Symbol sym = precedingItem.symbolAfterDot;
				if (sym != null) {
					outgoing.add(sym);
				}
			}

			/* now create a transition out for each individual symbol */
			for(Symbol sym : outgoing) {

				/* will be keeping the set of items with propagate links */
				linked_items = new LalrItemSet();

				/*
				 * gather up shifted versions of all the items that have this
				 * symbol before the dot
				 */
				new_items = new LalrItemSet();
				for (LalrItem shitem : workState.items()) {

					/* if this is the symbol we are working on now, add to set */
					sym2 = shitem.symbolAfterDot;
					if (sym.equals(sym2)) {
						/* add to the kernel of the new state */
						LalrItem shiftedTo = shitem.shift();
						new_items.add(shiftedTo);

						/* remember that shitem has propagate link to it */
						linked_items.add(shitem);
					}
				}

				/* use new items as state kernel */
				kernel = new LalrItemSet(new_items);

				/* have we seen this one already? */
				new_st = allKernels.get(kernel);

				/* if we haven't, build a new state out of the item set */
				if (new_st == null) {
					/* compute closure of the kernel for the full item set */
					new_items.computeClosure();

					/* build the new state */
					new_st = createLalrState(new_items);

					/* add the new state to our work set */
					workStack.push(new_st);

					/* put it in our kernel table */
					allKernels.put(kernel, new_st);
				} else {
				/* otherwise relink propagation to items in existing state */
				
					/* walk through the items that have links to the new state */
					for (LalrItem fixItem : linked_items) {
						

						/* look at each propagate link out of that item */
						for (int l = 0; l < fixItem.propagate_items().size(); l++) {
							/* pull out item linked to in the new state */
							new_itm = (LalrItem) fixItem.propagate_items().elementAt(l);

							/* find corresponding item in the existing state */
							existing = new_st.items().find(new_itm);

							/* fix up the item so it points to the existing set */
							if (existing != null) {
								fixItem.propagate_items().setElementAt(existing, l);
							}
						}
					}
				}


				/* add a transition from current state to that state */
				workState.add_transition(sym, new_st);
			}
		}

		/* all done building states */

		/* propagate complete lookahead sets throughout the states */
		propagateAllLookaheads();

		return start_state;
	}
	
	

	private LalrState createLalrState(LalrItemSet startItems) {
		LalrState lalrState = new LalrState(startItems, stateMap.count());
		stateMap.add(lalrState);
		return lalrState;
	}



	/**
	 * Propagate lookahead sets through the constructed viable prefix
	 * recognizer. When the machine is constructed, each item that results in
	 * the creation of another such that its lookahead is included in the
	 * other's will have a propagate link set up for it. This allows additions
	 * to the lookahead of one item to be included in other items that it was
	 * used to directly or indirectly create.
	 */
	protected void propagateAllLookaheads() {
		/* iterate across all states */
		for(LalrState state : stateMap) {
			state.propagateLookaheads();
		}
	}

	public LalrStateMap getStateMap() {
		return stateMap;
	}


	/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

	/** Produce a (semi-) human readable dumps of the parse tables */
	public void dump_tables() {
//		System.err.println(action_table);
//		System.err.println(reduce_table);
		List<Terminal> terminals = model.terminals.getOrderedByIndex();
		for (Terminal terminal : terminals) {
			System.out.println("term["+terminal.index()+"]: name='"+terminal.name()+"'");
		}
		
		List<NonTerminal> nonTerminals = model.nonTerminals.getOrderedByIndex();
		for (NonTerminal nonTerminal : nonTerminals) {
			System.out.println("non-term["+nonTerminal.index()+"]: name='"+nonTerminal.name()+"'"+", "+(nonTerminal.nullable() ? "nullable" : "not-nlbl")+", first-set="+nonTerminal.first_set());
		}
		
		PrintStream out = System.out;
		
		int stateCnt = action_table.num_states();
		for(int stateIdx=0; stateIdx<stateCnt; stateIdx++) {

			LalrState state = null;
			for(LalrState tstate : allKernels.values()) {
				if (tstate.index()==stateIdx) {
					state = tstate;
					break;
				}
			}

			out.println("State["+stateIdx+"]");
			if (state!=null) {
				state.describe(out);
			}
			
			ParseActionRow parseActionRow = action_table.under_state[stateIdx];
			parseActionRow.describe(out);

			ParseReduceRow parseReduceRow = reduce_table.under_state[stateIdx];
			parseReduceRow.describe(out);
			
		}
		
		
	}


	
	
}
