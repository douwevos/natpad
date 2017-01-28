package net.natpad.cup.config;

import java.util.ArrayList;
import java.util.List;

public class CupConfiguration {

	protected List<ICupExporter> exporters = new ArrayList<ICupExporter>();
	
	protected int expectConflicts = 0;

	/**
	 * User option -- should we include non terminal symbol numbers in the
	 * symbol constant class.
	 */
	protected boolean exportNonTerms;

	
	
	public CupConfiguration() {
	}

	
	public void addExporter(ICupExporter exporter) {
		exporters.add(exporter);
	}
	
	
	public List<ICupExporter> getExporters() {
		return exporters;
	}

	
	public void setExepectConflicts(int val) {
		this.expectConflicts = val;
	}

	public int getExpectConflicts() {
		return expectConflicts;
	}


	public void setExportNonTerms(boolean b) {
		this.exportNonTerms = true;
	}
	
	public boolean getExportNonTerms() {
		return exportNonTerms;
	}

	
}
