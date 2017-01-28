package net.natpad.gross.build;

import java.io.PrintStream;
import java.util.HashMap;
import java.util.HashSet;

import net.natpad.gross.runtime.ReduceLayout;
import net.natpad.gross.runtime.State;
import net.natpad.gross.runtime.Symbol;

public class LrarState {

	public final Kernel kernel;
	public final int id;
	public final HashMap<Symbol, TransitionsForSymbol> transitionMap  = new HashMap<Symbol, TransitionsForSymbol>();
	
	public LrarState(Kernel kernel, int id) {
		this.kernel = kernel;
		this.id = id;
	}
	

	public void addAccept(Symbol symbolAtDot) {
		TransitionsForSymbol transitionsForSymbol = getOrCreateTransitionsForSymbol(symbolAtDot);
		transitionsForSymbol.addTransition(new AcceptTransition());
	}

	
	public TransitionsForSymbol getOrCreateTransitionsForSymbol(Symbol followSym) {
		TransitionsForSymbol transitionsForSymbol = transitionMap.get(followSym);
		if (transitionsForSymbol==null) {
			transitionsForSymbol = new TransitionsForSymbol(followSym);
			transitionMap.put(followSym, transitionsForSymbol);
		}
		return transitionsForSymbol;
	}

	
	private void describeLink(PrintStream out, HashMap<DotState, DotLink> linkMap, DotState referredBy, String indent, HashSet<DotState> done) {
		DotLink childDotLink = linkMap.get(referredBy);
		out.println(indent+""+childDotLink.dotState);
		indent = indent+"  ";
		for(DotState sub : childDotLink) {
			if (done.add(sub)) {
				describeLink(out, linkMap, sub, indent, done);
			}
		}
	}

	public void describe(PrintStream out, boolean full, HashMap<DotState, DotLink> linkMap) {
		out.println("state "+id);
		out.println();
		out.println("  kernel id="+kernel.id+", size="+kernel.size());
		for(DotLink dotLink : kernel) {
			out.println("    "+dotLink.dotState);
			if (full) {
				for(DotState link : dotLink) {
					String indent = "          ";
					describeLink(out, linkMap, link, indent, new HashSet<DotState>());
				}
			}
		}
		out.println();
		out.println("  expect");
		
		for(TransitionsForSymbol transForSymbol : transitionMap.values()) {
			out.println("    "+transForSymbol.symbol.name);
			if (full && transForSymbol.rawList.size()>0) {
				for(Transition transition : transForSymbol.rawList) {
					out.println("      "+transition);
				}
			} else {
				for(Transition transition : transForSymbol.unique) {
					out.println("      "+transition);
				}
			}
		}
		out.println();
		
	}

	
	private int findIndex(LrarState[] ccStates, LrarState state2find) {
		for(int idx=0; idx<ccStates.length; idx++) {
			if (ccStates[idx]==state2find) {
				return idx;
			}
		}
		return -1;
	}

	public void configureState(State state2config, LrarState[] ccStates, State[] states) {
//			System.out.println("configure state="+state2config);
			for(TransitionsForSymbol transForSymbol : transitionMap.values()) {
				Symbol forwardSymbol = transForSymbol.symbol;
//				System.out.println("forwardSymbol="+forwardSymbol.name);
				for(Transition transition : transForSymbol.unique) {
					if (transition instanceof ReduceTransition) {
						ReduceTransition redTrans = (ReduceTransition) transition;
						DotState reduceRule = redTrans.reduceRule;
						
						ReduceLayout reduceLayout = new ReduceLayout(reduceRule.production.lhs, reduceRule.production.rhsCount(), reduceRule.nullified, reduceRule.production.id.intValue());
						state2config.addReduce(forwardSymbol, reduceLayout, reduceRule.production.id);
					} else if (transition instanceof ShiftTransition) {
						ShiftTransition shiftTrans = (ShiftTransition) transition;
						int shiftTIdx = findIndex(ccStates, shiftTrans.shiftToState);
						state2config.addShift(forwardSymbol, states[shiftTIdx]);
					} else if (transition instanceof AcceptTransition) {
						state2config.addAccept(forwardSymbol);
					}
				}
			}
		
	}

	

}
