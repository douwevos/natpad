package net.natpad.gross.runtime;

import java.util.ArrayList;

import net.natpad.gross.runtime.GrossLogger.Level;

public class Parser {
	
	private GrossLogger logger;
	
	
	public Integer maxInt;
	
	public Parser(GrossLogger logger) {
		this.logger = logger;
	}
	
	
	
	@SuppressWarnings("unchecked")
	public <T> T parse(ParserContext context, Scanner scanner) {
		
		int maxIter = 0;
		context.createStartFork(scanner.next());
		
		while(context.forkCount()!=0) {
			if (maxInt!=null) {
				if (maxIter++>maxInt.intValue()) {
					break;
				}
			}
			
			context.errorForksClear();
			Token nextLookahead = scanner.next();
			
			
			logger.log(Level.DEBUG, "################ next-lookahead=%s #############", nextLookahead);
			
			forks:
			for(int forkIdx=0; forkIdx<context.forkCount(); forkIdx++) {
				Fork fork = context.forkAt(forkIdx);
				
				while(true) {
					if (logger.isEnabled(Level.DETAIL)) {
						fork.dump();
					}
					
					StackEntry topEntry = fork.getTopEntry();
					Transition transition = fork.getPendingTransition();
					if (transition==null) {
					
						TransitionsForSymbol transitionsForSymbol = topEntry.state.transitionMap.get(topEntry.lookahead.getSymbol());
						if (transitionsForSymbol==null) {
							Location location = context.getTokenFactory().extractLocation(topEntry.lookahead);
							fork.appendMessage("no transitions for symbol:"+topEntry.lookahead+" for state:"+topEntry.state, location);
							context.markForkAsError(forkIdx);
							forkIdx--;
							continue forks;
						}
						if (transitionsForSymbol.unique.size()>1) {
							for(int idx=1; idx<transitionsForSymbol.unique.size(); idx++) {
								Transition pendingTransition = transitionsForSymbol.unique.get(idx);
								context.fork(forkIdx, pendingTransition);
								if (logger.isEnabled(Level.VERBOSE)) logger.log(Level.VERBOSE, "FORK: pending-trans=%s", pendingTransition);
							}
						}
						transition = transitionsForSymbol.unique.get(0);
					}

					if (logger.isEnabled(Level.DEBUG)) logger.log(Level.DEBUG, "transition=%s", transition);
					if (transition instanceof ReduceTransition) {
						fork.setPostponed(topEntry.lookahead);
						ReduceTransition redTrans = (ReduceTransition) transition;
						int rhsCount = redTrans.getRhsCount();
						if (logger.isEnabled(Level.VERBOSE)) logger.log(Level.VERBOSE, "removing %d from stack and replacing with %s", rhsCount, redTrans.reduceLayout.lhs);
						
						Token reducedToken = context.reduce(fork, redTrans.reduceLayout, redTrans.actionId);
						
						for(int idx=0; idx<rhsCount; idx++) {
							topEntry = topEntry.parent;
						}
						
//						TokenFactory tokenFactory = context.getTokenFactory();
//						Token reducedToken = tokenFactory.createToken(production.lhs, value);
						StackEntry newTop = new StackEntry(topEntry.parent, topEntry.state, reducedToken);
						fork.setTopEntry(newTop);
							
					} else if (transition instanceof ShiftTransition) {
						ShiftTransition shiftTrans = (ShiftTransition) transition;
						boolean consumedNextLookahead = false;
						Token teSym  = fork.getPostponed();
						if (teSym!=null) {
							fork.setPostponed(null);
						} else {
							teSym = nextLookahead;
							consumedNextLookahead = true;
						}
						
						StackEntry newTop = new StackEntry(topEntry, shiftTrans.forwardState, teSym);
						fork.setTopEntry(newTop);

						if (logger.isEnabled(Level.VERBOSE)) logger.log(Level.VERBOSE, "shifting to state %s new lookahead %s", newTop.state, newTop.lookahead );
						if (consumedNextLookahead) {
							continue forks;
						}
					} else if (transition instanceof AcceptTransition) {
						context.markForkAsAccepted(forkIdx);
						forkIdx--;
						continue forks;
					} else {
						System.err.println("no transition....");
						break;
					}
				}
				
			}
			
			
			logger.log(Level.VERBOSE, "forkCount=%d, accepted=%d, error-cnt=%d", context.forkCount(), context.acceptedForks.size(), context.errorForkCount());
			
			if (context.forkCount() == 0 && context.acceptedForks.isEmpty()) {
				if (context.errorForkCount()>0) {
					tryErrorRecovery(context, scanner);
				}
			}
			
		}
		
		if (!context.acceptedForks.isEmpty()) {
			return (T) context.acceptedForks.get(0).getResultValue();
		}
		return null;
		
	}



	private void tryErrorRecovery(ParserContext context, Scanner scanner) {
		if (logger.isEnabled(Level.VERBOSE)) logger.log(Level.VERBOSE, "try-error-recovery");
		
		context.errorsHold();
		Terminal errorTerminal = context.getErrorTerminal();
		for(int forkIdx=context.errorForkCount()-1; forkIdx>=0; forkIdx--) {
			Fork fork = context.errorForkAt(forkIdx);
			StackEntry topEntry = fork.getTopEntry();
			logger.log(Level.DEBUG, "fork:%s, topEntry=%s", fork, topEntry);
			while(topEntry!=null) {
				TransitionsForSymbol trans = topEntry.state.transitionMap.get(errorTerminal);
				logger.log(Level.DEBUG, "topEntry=%s, error-symbol-trans:%s", topEntry, trans);
				if (trans!=null && trans.unique.size()>0) {
					Transition transition = trans.unique.get(0);
					logger.log(Level.DEBUG, "transition=%s", transition);
					if (transition instanceof ShiftTransition) {
//						ShiftTransition shiftTrans = (ShiftTransition) transition;
						Token lookahead = context.createErrorToken();
//						StackEntry stackEntry = new StackEntry(topEntry.parent, topEntry.state, lookahead);
//						stackEntry = new StackEntry(stackEntry, shiftTrans.forwardState, null);
						StackEntry stackEntry = new StackEntry(topEntry.parent, topEntry.state, lookahead);
						fork.setTopEntry(stackEntry);
						break;
					}
				}
				topEntry = topEntry.parent;
			}
			
			if (topEntry==null) {
				context.errorForks.remove(forkIdx);
			}
		}
		
		int recSymCount = 5;
		if (context.errorForkCount()>0) {
			ArrayList<Fork> spawned = new ArrayList<Fork>();

			boolean keepRecovering = true;
			
			while(keepRecovering) {

			
				Token nextLookahead = scanner.next();
				logger.log(Level.DEBUG, "################ next-lookahead=%s #############", nextLookahead);

				
				/* each round we spawn a new batch of forks with the error-forks as base and the recovery-count initialized at the beginning */
				if (nextLookahead.getSymbol()!=context.getEofTerminal()) {
					for(int idx=context.errorForkCount()-1; idx>=0; idx--) {
						Fork errorFork = context.errorForkAt(idx);
						Fork duplicate = errorFork.duplicate(recSymCount);
						StackEntry newTop = errorFork.getTopEntry();
//						newTop = new StackEntry(newTop.parent, newTop.state, nextLookahead);
						duplicate.setTopEntry(newTop);
						spawned.add(duplicate);
					}
				}

				if (spawned.size()==0) {
					break;
				}
				
				for(int forkIdx=0; forkIdx<spawned.size(); forkIdx++) {
					Fork fork = spawned.get(forkIdx);
					
					while(true) {
						if (logger.isEnabled(Level.DETAIL)) {
							fork.dump();
						}
						
						StackEntry topEntry = fork.getTopEntry();
						Transition transition = fork.getPendingTransition();
						if (transition==null) {
							
							TransitionsForSymbol transitionsForSymbol = topEntry.state.transitionMap.get(topEntry.lookahead.getSymbol());
							if (transitionsForSymbol==null) {
								spawned.remove(forkIdx);
								forkIdx--;
								fork = null;
								break;
							}
							if (transitionsForSymbol.unique.size()>1) {
								for(int idx=1; idx<transitionsForSymbol.unique.size(); idx++) {
									Transition pendingTransition = transitionsForSymbol.unique.get(idx);
									Fork newSpawned = fork.duplicate(fork.getRecoveryCount());
									newSpawned.setPendingTransition(pendingTransition);
									spawned.add(newSpawned);
									if (logger.isEnabled(Level.VERBOSE)) logger.log(Level.VERBOSE, "FORK: pending-trans=%s", pendingTransition);
								}
							}
							transition = transitionsForSymbol.unique.get(0);
						}
						
						if (logger.isEnabled(Level.DEBUG)) logger.log(Level.DEBUG, "transition=%s", transition);
						if (transition instanceof ReduceTransition) {
							fork.setPostponed(topEntry.lookahead);
							ReduceTransition redTrans = (ReduceTransition) transition;
							int rhsCount = redTrans.getRhsCount();
							if (logger.isEnabled(Level.VERBOSE)) logger.log(Level.VERBOSE, "removing %d from stack and replacing with %s", rhsCount, redTrans.reduceLayout.lhs);
							
							Token reducedToken = context.reduce(fork, redTrans.reduceLayout, redTrans.actionId);
							
							for(int idx=0; idx<rhsCount; idx++) {
								topEntry = topEntry.parent;
							}
							
//						TokenFactory tokenFactory = context.getTokenFactory();
//						Token reducedToken = tokenFactory.createToken(production.lhs, value);
							StackEntry newTop = new StackEntry(topEntry.parent, topEntry.state, reducedToken);
							fork.setTopEntry(newTop);
							
						} else if (transition instanceof ShiftTransition) {
							ShiftTransition shiftTrans = (ShiftTransition) transition;
							
							boolean consumedNextLookahead = false;
							Token teSym = fork.getPostponed();
							if (teSym!=null) {
								fork.setPostponed(null);
							} else {
								teSym = nextLookahead;
								consumedNextLookahead = true;
							}
							
							StackEntry newTop = new StackEntry(topEntry, shiftTrans.forwardState, teSym);
							fork.setTopEntry(newTop);
							
							if (logger.isEnabled(Level.VERBOSE)) logger.log(Level.VERBOSE, "shifting to state %s new lookahead %s", newTop.state, newTop.lookahead );
							if (consumedNextLookahead) {
								break;
							}
						} else if (transition instanceof AcceptTransition) {
							context.addAccepted(fork);
							spawned.remove(forkIdx);
							fork = null;
							forkIdx--;
							keepRecovering = false;
							break;
						} else {
							System.err.println("no transition....");
							break;
						}
					}
					
					
					if (fork!=null) {
						if (fork.decreaseRecovery()) {
							keepRecovering = false;
							context.addFork(fork);
							spawned.remove(forkIdx);
							forkIdx--;
						}
					}
				}
			
			}
		}
		

		if (context.forkCount()==0) {
			context.errorRestore();
		}
	}
	
	
}
