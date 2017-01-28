package net.natpad.gross.runtime;

import java.util.ArrayList;
import java.util.List;

public class ParserContext {

	private final TokenFactory tokenFactory;
	private final StateModel stateModel;
	private final ReduceAction actionHandler;
	List<Fork> forks = new ArrayList<Fork>();

	List<Fork> acceptedForks = new ArrayList<Fork>();
	List<Fork> errorForks = new ArrayList<Fork>();
	List<Fork> errorForksHold = new ArrayList<Fork>();
	
	protected boolean calculateAdditionalInformation;

	public ParserContext(TokenFactory tokenFactory, StateModel stateModel, ReduceAction actionHandler) {
		this.tokenFactory = tokenFactory;
		this.stateModel = stateModel;
		this.actionHandler = actionHandler;
		calculateAdditionalInformation = tokenFactory instanceof FullTokenFactory;
	}

	public Terminal getErrorTerminal() {
		return stateModel.getErrorTerminal();
	}

	public Symbol getEofTerminal() {
		return stateModel.getEofTerminal();
	}

	public Token createErrorToken() {
		return tokenFactory.createToken(stateModel.getErrorTerminal(), "error", null, null);
	}
	
	public void createStartFork(Token lookahead) {
		forks.clear();
		Fork fork = new Fork();
		State startState = stateModel.getStartState();
		fork.push(startState, lookahead);
		forks.add(fork);
	}

	
	public void addFork(Fork fork) {
		forks.add(fork);
	}

	public void fork(int forkIdx, Transition pendingTransition) {
		Fork forked = forks.get(forkIdx).duplicate(0);
		forked.setPendingTransition(pendingTransition);
		forks.add(forkIdx+1, forked);
	}

	public int forkCount() {
		return forks.size();
	}

	public Fork forkAt(int forkIdx) {
		return forks.get(forkIdx);
	}

	public Fork acceptedAt(int forkIdx) {
		return acceptedForks.get(forkIdx);
	}

	public void markForkAsError(int forkIdx) {
		Fork fork = forks.get(forkIdx);
		forks.remove(forkIdx);
		errorForks.add(fork);
	}

	public void markForkAsAccepted(int forkIdx) {
		Fork fork = forks.get(forkIdx);
		forks.remove(forkIdx);
		System.out.println("ACCEPTED:fork["+fork.id+"]");
		acceptedForks.add(fork);
	}

	public void addAccepted(Fork fork) {
		System.out.println("ACCEPTED:fork["+fork.id+"]");
		acceptedForks.add(fork);
	}

	
	
	public List<Fork> getAcceptedForks() {
		return acceptedForks;
	}

	public TokenFactory getTokenFactory() {
		return tokenFactory;
	}
	
	
	public void errorForksClear() {
		errorForks.clear();
		errorForksHold.clear();
	}


	
	public void errorFork(int forkIdx, Transition pendingTransition) {
		Fork forked = errorForks.get(forkIdx).duplicate(0);
		forked.setPendingTransition(pendingTransition);
		forked.setPostponed(null);
		errorForks.add(forkIdx+1, forked);
	}

	public int errorForkCount() {
		return errorForks.size();
	}

	public Fork errorForkAt(int forkIdx) {
		return errorForks.get(forkIdx);
	}


	public void errorsHold() {
		errorForksHold.clear();
		errorForksHold.addAll(errorForks);
	}
	
	public void errorRestore() {
		errorForks.clear();
		errorForks.addAll(errorForksHold);
	}
	
	protected Location combineLocations(Fork fork, int rhsCount) {
		Location result = null;
		if (calculateAdditionalInformation) {
			StackEntry leftEntry = fork.getEntry(rhsCount-1);
			StackEntry rightEntry = fork.getEntry(0);
			Location leftLoc = null;
			if (leftEntry.lookahead instanceof FullToken) {
				leftLoc = ((FullToken) leftEntry.lookahead).location;
			}
			Location rightLoc = null;
			if (rightEntry.lookahead instanceof FullToken) {
				rightLoc = ((FullToken) rightEntry.lookahead).location;
			}
			if (leftLoc!=null && rightLoc!=null) {
				if (leftLoc==rightLoc) {
					result = leftLoc;
				} else {
					result = new Location(leftLoc.beginColumn, leftLoc.beginRow, rightLoc.endColumn, rightLoc.endRow);
				}
			}
			
		}
		return result;
	};


	public Token reduce(Fork fork, ReduceLayout reduceLayout, int actionId) {
		Location location = combineLocations(fork, reduceLayout.getStackRhsCount());
		Object value = null;
		if (actionHandler!=null) {
			value = actionHandler.doAction(fork, reduceLayout, actionId);
		}
		return tokenFactory.createToken(reduceLayout.lhs, value, location, null);
	}


	
	
	public interface ReduceAction {
		Object doAction(Fork fork, ReduceLayout reduceLaout, int actionId);
	}

}
