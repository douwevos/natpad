package net.natpad.sht.model;

import java.util.List;

public interface MapValue extends Iterable<Reference> {

	void add(Reference ref);

	List<Reference> enlistReferenceById(String id);

}
