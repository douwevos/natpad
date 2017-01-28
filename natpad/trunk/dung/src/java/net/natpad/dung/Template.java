package net.natpad.dung;

import net.natpad.sht.model.BlockValue;

public class Template extends BlockValue {

	public final String id;

	public Template(String id) {
		this.id = id;
	}

	@Override
	public String toString() {
		return "Template [id=" + id + ", list=" + list + "]";
	}

}
