package net.natpad.gross.runtime;

public class Message {

	public final Message parent;
	public final String text;
	public final Location location;
	
	public Message(Message parent, String text, Location location) {
		this.parent = parent;
		this.text = text;
		this.location = location;
	}
}
