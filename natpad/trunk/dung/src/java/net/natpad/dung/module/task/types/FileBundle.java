package net.natpad.dung.module.task.types;

import java.io.File;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.regex.Pattern;

public class FileBundle {

	public String path;
	
	ArrayList<HasPattern> includeAndExcludeList = new ArrayList<HasPattern>();
	
	
	public FileBundle() {
	}
	
	public void addInclude(String pattern) {
		includeAndExcludeList.add(new Include(pattern));
	}

	public void addInclude(Include include) {
		includeAndExcludeList.add(include);
	}

	
	public void addExclude(String pattern) {
		includeAndExcludeList.add(new Exclude(pattern));
	}

	public void addExclude(Exclude exclude) {
		includeAndExcludeList.add(exclude);
	}

	interface HasPattern {
		String getPattern();
		boolean matchRelative();
	}
	
	public static class Include implements HasPattern {
		public String pattern;
		public boolean matchRelative;
		
		public Include() {
			pattern=".*";
		}
		
		public Include(String pattern) {
			this.pattern = pattern;
		}
		public String getPattern() {
			return pattern;
		}
		@Override
		public boolean matchRelative() {
			return matchRelative;
		}
	}
	
	public static class Exclude implements HasPattern {
		public String pattern;
		public boolean matchRelative;
		public Exclude() {
			pattern=".*";
		}
		public Exclude(String pattern) {
			this.pattern = pattern;
		}
		public String getPattern() {
			return pattern;
		}
		@Override
		public boolean matchRelative() {
			return matchRelative;
		}
	}

	public boolean match(Path base, Path relativized, File file) {
		if (file.isDirectory()) {
			return true;
		}
		String filename = file.getName();
		String relativename = relativized.toString();
		boolean match = true;
		if (!includeAndExcludeList.isEmpty()) {
			HasPattern hasPattern = includeAndExcludeList.get(0);
			match = hasPattern instanceof Exclude;
		}
		
		for (HasPattern hasPattern : includeAndExcludeList) {
			String pattern = hasPattern.getPattern();
			boolean didMatch = false;
//			System.out.println("pattern:"+pattern+", "+filename+", rel="+relativename+", match-rel="+hasPattern.matchRelative());
			if (hasPattern.matchRelative()) {
				didMatch = match(relativename, pattern);
			} else {
				didMatch = match(filename, pattern);
			}
//			System.out.println("relativename="+relativename+", pattern="+pattern+", didMatch="+didMatch);
			if (didMatch) {
//				System.out.println("match");
				if (hasPattern instanceof Include) {
					match = true;
					break;
				} else {
					match = false;
					break;
				}
			}
			
		}
		return match;
	}

	private boolean match(String filename, String pattern) {
		return Pattern.matches(pattern, filename);
	}
	
}
