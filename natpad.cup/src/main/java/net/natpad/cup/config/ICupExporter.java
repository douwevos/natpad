package net.natpad.cup.config;

import net.natpad.cup.model.bnf.BnfModel;
import net.natpad.cup.state.StateMachine;

public interface ICupExporter {

	void export(BnfModel model, StateMachine stateMachine);

	/**
	 * @param ts
	 *            timestamp in milliseconds
	 * @return true if the ts exceeds the timestamp of at least one of the
	 *         output files.
	 */
	boolean testForChanges(long ts);
}
