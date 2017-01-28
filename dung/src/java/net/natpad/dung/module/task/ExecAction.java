package net.natpad.dung.module.task;

import java.io.File;
import java.io.IOException;

import net.natpad.dung.StreamHelper;
import net.natpad.dung.run.BashHelper;
import net.natpad.dung.run.ProcessInputStreamer;

public class ExecAction {

	private final String prefix;
	private final String args[];
	private final String env[];
	private final File dir;
	int exitcode;

	public ExecAction(String prefix, String args[]) {
		this(prefix, args, null,null);
	}

	
	public ExecAction(String prefix, String args[], String env[], File dir) {
		this.prefix = prefix;
		this.args = args;
		this.env = env;
		this.dir = dir;
		exitcode = 0;
	}
	
	public int runAction() {
		exitcode = 0;
		StreamHelper streamHelper = (StreamHelper) System.out;
		String oldPrefix = streamHelper.getPrefix();
		try {
			Runtime runtime = Runtime.getRuntime();
			Process process = runtime.exec(args, env, dir);
			
			
			streamHelper.setPrefix(prefix);
			
			ProcessInputStreamer errstreamer = new ProcessInputStreamer(process.getErrorStream());
			Thread errthread = errstreamer.launch();
			ProcessInputStreamer outstreamer = new ProcessInputStreamer(process.getInputStream());
			Thread outthread = outstreamer.launch();
			exitcode = process.waitFor();
			errthread.join();
			outthread.join();
			for(String line : outstreamer.output) {
				BashHelper.printLine(line);
//				System.err.println(line);
			}
			for(String line : errstreamer.output) {
				BashHelper.printLine(line);
			}
			if (exitcode!=0) {
				System.err.println("Dumping arguments");
				for(int ia=0; ia<args.length; ia++) {
					System.err.println(" ["+ia+"]="+args[ia]);
				}
				
//				throw new BuildException("cc compile returned "+exitcode);
			}
			
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			streamHelper.setPrefix(oldPrefix);
		}
		return exitcode;
	}

}
