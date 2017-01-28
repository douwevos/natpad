package net.natpad.caterpillar;

public enum CatLogLevels {

	CAT_LOG_FATAL("FATL"),
	CAT_LOG_ERROR("ERRO"),
	CAT_LOG_WARN("WARN"),
	CAT_LOG_INFO("INFO"),
	CAT_LOG_DETAIL("DETL"),
	CAT_LOG_DEBUG("DEBG"),
	CAT_LOG_TRACE("TRAC");
	
	
	public final String text;
	
	CatLogLevels(String text) {
		this.text = text;
	}

	@Override
	public String toString() {
		return text;
	}
	
}
