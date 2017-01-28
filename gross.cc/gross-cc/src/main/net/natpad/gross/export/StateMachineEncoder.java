package net.natpad.gross.export;

import java.util.ArrayList;
import java.util.List;

import net.natpad.gross.build.AcceptTransition;
import net.natpad.gross.build.CCStateMachine;
import net.natpad.gross.build.DotState;
import net.natpad.gross.build.LrarState;
import net.natpad.gross.build.Production;
import net.natpad.gross.build.ReduceTransition;
import net.natpad.gross.build.ShiftTransition;
import net.natpad.gross.build.Transition;
import net.natpad.gross.build.TransitionsForSymbol;
import net.natpad.gross.runtime.Symbol;

public class StateMachineEncoder {

	public final CCStateMachine stateMachine;

	public List<Production> actionIndexList = new ArrayList<Production>();
	
	public List<String> reduceStringList = new ArrayList<String>();

	public StateMachineEncoder(CCStateMachine stateMachine) {
		this.stateMachine = stateMachine;
	}
	

	public String createStateConfigurationText() {
		StringBuilder buf = new StringBuilder();
		for(int stateIdx=0 ; stateIdx<stateMachine.stateCount(); stateIdx++) {
			LrarState state = stateMachine.stateAt(stateIdx);
			buf.append("A").append(stateIdx);
			exportState(buf, stateIdx, state);
		}
		StringBuilder result = new StringBuilder();
		for(String reds : reduceStringList) {
			result.append(reds);
		}
		result.append(buf);
		return result.toString();
	}

	

	private void exportState(StringBuilder buf, int stateIdx, LrarState state) {
		for(Symbol sym : state.transitionMap.keySet()) {
			buf.append('B').append(sym.id.intValue());
			TransitionsForSymbol transitionsForSymbol = state.transitionMap.get(sym);
			for(Transition trans : transitionsForSymbol.unique) {
				exportTransition(buf, trans);
			}
		}
		
	}


	private void exportTransition(StringBuilder buf, Transition trans) {
		if (trans instanceof AcceptTransition) {
			buf.append('E');
		} else if (trans instanceof ShiftTransition) {
			ShiftTransition st = (ShiftTransition) trans;
			int stateIndex = stateMachine.indexOf(st.shiftToState);
			buf.append('S').append(stateIndex);
		} else if (trans instanceof ReduceTransition) {
			ReduceTransition rt = (ReduceTransition) trans;
			int actionId = getOrAddActionForRule(rt.reduceRule.production);
			int rsi = addOrGetReduceStringIndex(rt.reduceRule);
			buf.append('R').append(rsi).append(',').append(actionId);
		}
		
	}

	
	private int getOrAddActionForRule(Production production) {
		String actionCode = production.getActionCode();
		if (actionCode==null) {
			return -1;
		}
		for(int idx=0; idx<actionIndexList.size(); idx++) {
			Production pp = actionIndexList.get(idx);
			if (pp==production) {
				return idx;
			}
		}
		actionIndexList.add(production);
		
		return actionIndexList.size()-1;
	}
	
	
	public int addOrGetReduceStringIndex(DotState dotState) {
		String reduceString = createReduceString(dotState);
		for(int idx=0; idx<reduceStringList.size(); idx++) {
			if (reduceString.equals(reduceStringList.get(idx))) {
				return idx;
			}
		}
		System.out.println(""+reduceString+", prod="+dotState.production);
		int result = reduceStringList.size();
		reduceStringList.add(reduceString);
		return result;
	}

	
	public String createReduceString(DotState dotState) {
		StringBuilder buf = new StringBuilder("r");
		buf.append(dotState.production.lhs.id);
		buf.append(',').append(dotState.production.rhsCount());
		if (dotState.nullified!=null) {
			for(int noff : dotState.nullified) {
				buf.append(',').append(noff);
			}
		}
		buf.append("p").append(dotState.production.id);
		
		return buf.toString();
	}
}
