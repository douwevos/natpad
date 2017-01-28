package net.natpad.gross.runtime;

import java.util.ArrayList;

public class StateModel implements SymbolProvider {

	Symbol symbols[];
	State states[];

	Terminal errorTerminal,eofTerminal;
	
	@Override
	public Terminal getTerminalByName(String name) {
		for (Symbol sym : symbols) {
			if (sym.name.equals(name)) {
				return (Terminal) sym;
			}
		}
		return null;
	}
	
	public StateModel(Symbol symbols[], State states[]) {
		this.symbols = symbols;
		this.states = states;
		
		errorTerminal = getTerminalByName("⊗");
		eofTerminal = getTerminalByName("⟧");
	}

	public StateModel(Symbol symbols[], String configText, int nrOfStates) {
		this(symbols, new State[nrOfStates]);
		for (int idx = 0; idx < states.length; idx++) {
			states[idx] = new State(idx);
		}

		extractConfiguration(configText);
	}

	public Terminal getErrorTerminal() {
		return errorTerminal;
	}

	public Terminal getEofTerminal() {
		return eofTerminal;
	}

	
	public State getStartState() {
		return states[0];
	}
	
	private void extractConfiguration(String configText) {
		ArrayList<ReduceLayout> reduceLayouts = new ArrayList<ReduceLayout>();
		State state = null;
		Symbol forSymbol = null;
		StringExtractor extractor = new StringExtractor(configText);
		boolean notDone = true;
		while (notDone) {
			char ch = extractor.next();
			switch (ch) {
				case 'r' : {
						Symbol lhsSymbol = symbols[extractor.readNumber()];
						ch = extractor.next();
						int rhsCount = extractor.readNumber();
						ch = extractor.next();
						int offsets[] = new int[50];
						int offsetsOut = 0;
						while(ch==',') {
							int offset = extractor.readNumber();
							if (offsetsOut>=offsets.length) {
								int noffsets[] = new int[offsets.length + 50];
								System.arraycopy(offsets, 0, noffsets, 0, offsets.length);
							}
							offsets[offsetsOut++] = offset;
							ch = extractor.next();
						}
						int productionId = -1;
						if (ch=='p') {
							ch = extractor.next();
							productionId = extractor.readNumber();
						} else {
							extractor.offset--;
						}
						
						if (offsetsOut>0) {
							int noffsets[] = new int[offsetsOut];
							System.arraycopy(offsets, 0, noffsets, 0, offsetsOut);
							offsets = noffsets;
						} else {
							offsets = null;
						}
						ReduceLayout reduceLayout = new ReduceLayout(lhsSymbol, rhsCount, offsets, productionId);
						reduceLayouts.add(reduceLayout);
					} break;
				case 'A': {
						/* next number is a state-index */
						state = states[extractor.readNumber()];
					} break;
				case 'B': {
						/* next number is a symbol-index */
						forSymbol = symbols[extractor.readNumber()];
					} break;
				case 'S': {
						/* add shift */
						State shiftTo = states[extractor.readNumber()];
						state.addShift(forSymbol, shiftTo);
					} break;
				case 'R': {
						/* add reduce */
//						int rhsCount = extractor.readNumber();
//						extractor.next();
//						Symbol lhs = symbols[extractor.readNumber()];
//						extractor.next();
//						int actionId = extractor.readNumber();
//						state.addReduce(forSymbol, rhsCount, lhs, actionId);
					
						int redLayIdx = extractor.readNumber();
						extractor.next();
						int actionId = extractor.readNumber();
						state.addReduce(forSymbol, reduceLayouts.get(redLayIdx), actionId);
					
					} break;
				case 'E': {
						state.addAccept(forSymbol);
					} break;
				default:
					notDone = false;
					break;
			}
		}
	}

	static class StringExtractor {

		public final String text;
		int offset;

		public StringExtractor(String text) {
			this.text = text;
		}

		public char next() {
			if (offset < text.length()) {
				return text.charAt(offset++);
			}
			return 0;
		}

		public int readNumber() {
			int result = 0;
			boolean isNeg = false;
			while (offset < text.length()) {
				char ch = text.charAt(offset);
				if (ch=='-') {
					offset++;
					isNeg = true;
				} else  if (ch >= '0' && ch <= '9') {
					result = result * 10 + (ch - '0');
					offset++;
				} else {
					break;
				}
			}
			return isNeg ? -result : result;
		}
	}


}
