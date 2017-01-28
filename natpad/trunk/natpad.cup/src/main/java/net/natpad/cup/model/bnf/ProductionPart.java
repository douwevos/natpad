package net.natpad.cup.model.bnf;

/**
 * This class represents one part (either a symbol or an action) of a production. In this base class it contains only an
 * optional label string that the user can use to refer to the part within actions.
 * <p>
 * 
 * This is an abstract class.
 * 
 * @see net.natpad.cup.model.bnf.Production
 * @version last updated: 11/25/95
 * @author Scott Hudson
 */
public interface ProductionPart {

	/**
	 * Indicate if this is an action (rather than a symbol). Here in the base class, we don't this know yet, so its an
	 * abstract method.
	 */
	public boolean isAction();


}
