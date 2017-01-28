package net.avaj.classfile.source;

public class ComplexMethodsExample {


	protected static String timestr(long time_val, long total_time) {
		boolean neg;
		long ms = 0;
		long sec = 0;
		long percent10;
		String pad;

		/* work with positives only */
		neg = time_val < 0;
		if (neg)
			time_val = -time_val;

		/* pull out seconds and ms */
		ms = time_val % 1000;
		sec = time_val / 1000;

		/* construct a pad to blank fill seconds out to 4 places */
		if (sec < 10)
			pad = "   ";
		else if (sec < 100)
			pad = "  ";
		else if (sec < 1000)
			pad = " ";
		else
			pad = "";

		/* calculate 10 times the percentage of total */
		percent10 = (time_val * 1000) / total_time;

		/* build and return the output string */
		return (neg ? "-" : "") + pad + sec + "." + ((ms % 1000) / 100) + ((ms % 100) / 10) + (ms % 10) + "sec" + " ("
						+ percent10 / 10 + "." + percent10 % 10 + "%)";
	}

}
