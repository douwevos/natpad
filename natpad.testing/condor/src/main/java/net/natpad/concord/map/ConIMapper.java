package net.natpad.concord.map;

import net.natpad.concord.Configuration;

public interface ConIMapper {


	  void map(ConMapContext map_context);
	  void unmap(ConUnmapContext unmap_context);

	  Configuration createDefault(Configuration config);

	  Configuration validate(Configuration config);
	
}
