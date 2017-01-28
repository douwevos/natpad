package net.natpad.gross.runtime;

public class Fork {

	private static int sequence;
	
	public final int id = sequence++;
	
	private StackEntry reverseStack;
	private Token postponed;
	private Transition pendingTransition;
	private int recoveryCount;
	
	private Message messages;


	
	public Fork duplicate(int recoveryCount) {
		Fork result = new Fork();
		result.reverseStack = reverseStack;
		result.postponed = postponed;
		result.recoveryCount = recoveryCount;
		result.messages = messages;
		return result;
	}

	public void appendMessage(String text, Location location) {
		messages = new Message(messages, text, location);
	}

	public Message getMessages() {
		return messages;
	}
	
	public int getRecoveryCount() {
		return recoveryCount;
	}

	public boolean decreaseRecovery() {
		return recoveryCount--<=0;
	}

	
	public void setPendingTransition(Transition pendingTransition) {
		this.pendingTransition = pendingTransition;
	}
	
	public Transition getPendingTransition() {
		Transition result = pendingTransition;
		pendingTransition = null;
		return result;
	}

	public void setPostponed(Token lookahead) {
		postponed = lookahead;
	}
	
	public Token getPostponed() {
		return postponed;
	}

	
	public StackEntry getTopEntry() {
		return reverseStack;
	}

	public Object getResultValue() {
		StackEntry result = reverseStack;
		while(result.parent!=null) {
			result = result.parent;
		}
		return result.lookahead==null ? null : result.lookahead.getValue();
	}

	
	public StackEntry getEntry(int plIdx) {
		if (plIdx<0) {
			return null;
		}
		StackEntry result = reverseStack;
		while(result!=null && plIdx>0) {
			plIdx--;
			result = result.parent;
		}
		return result;
	}



	public void setTopEntry(StackEntry newTop) {
		reverseStack = newTop;
	}

	
	public void push(State state, Token lookahead) {
		reverseStack = new StackEntry(reverseStack, state, lookahead);
	}

	public void dump() {
		System.out.println("Fork["+id+"]");
		StackEntry e = reverseStack;
		while(e!=null) {
			System.out.println(" # state="+e.state+", lookahead="+e.lookahead);
			e=e.parent;
		}
		System.out.println(" - pending-trans :"+pendingTransition);
		System.out.println(" - postponed     :"+(postponed!=null ? postponed : "<nil>"));
		if (recoveryCount>0) {
			System.out.println(" - rec-count     :"+recoveryCount);
			
		}
	}



}
