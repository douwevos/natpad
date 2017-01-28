package net.natpad.concord.ui;

public interface ConIPanelFactory {

	void attach(ConDialog con_dialog, ConIPostBox post_box);
	void detach(ConDialog con_dialog);
	ConPanel create(ConDialog con_dialog, ConDialogEntry tree_entry, Object user_data_1, Object user_data_2);

}
