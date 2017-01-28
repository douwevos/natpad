package net.natpad.dung.hill.task;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.RandomAccessFile;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Collections;

import net.natpad.dung.hill.HillSession;
import net.natpad.dung.hill.PipedStreamer;
import net.natpad.dung.module.task.LogLevel;
import net.natpad.dung.run.ProcessInputStreamer;

public class Autoinstall extends Task {

	
	

	static String lastLoadedReproUrl;
	static String lastText;
	
	
	public String url;
	public String repro;
	public String reproName;
	public String dir;
	
	ArhiveType archiveType;
	
	boolean extract = false;

	
	@Override
	public void run(HillSession session) throws Exception {
		URL realUrl;
		try {
			realUrl = extractUrl();
//			realUrl = new URL(url);
			String filename = realUrl.getPath();
			int idx = filename.lastIndexOf('/');
			if (idx>=0) {
				filename = filename.substring(idx+1);
			}
			
			File outputDir = null;
			
			if (dir==null) {
				outputDir = session.getWorkDir();
			} else {
				outputDir = session.createWorkPath(dir).toFile();
			}
			outputDir.mkdirs();

			log(LogLevel.INFO, "Installing:" + realUrl.getFile()+" in "+outputDir.toString());

			
			File archiveFile = null;
			archiveFile = new File(outputDir, filename);
			byte data[] = download(realUrl, archiveFile);
			
			if (archiveType==null) {
				if (realUrl.getFile().endsWith(".rpm")) {
					archiveType = ArhiveType.RPM;
				} else if (realUrl.getFile().endsWith(".deb")) {
					archiveType = ArhiveType.DEB;
				} else if (realUrl.getFile().endsWith(".zip")) {
					archiveType = ArhiveType.ZIP;
				} else if (realUrl.getFile().endsWith(".exe")) {
					archiveType = ArhiveType.EXE;
				}
			}
			
			
			if (extract) {
				switch(archiveType) {
					case RPM : {
						Runtime runtime = Runtime.getRuntime();
						String args[] = new String[2];
//						args[0] = "/bin/bash";
//						args[1] = "-c";
//						args[2] = "/usr/bin/rpm2cpio "+archiveFile.getCanonicalPath()+" | /bin/cpio -idmv";
						args[0] = "/usr/bin/rpm2cpio";
						args[1] = archiveFile.getCanonicalPath();
						Process rpm2cpioProcess = runtime.exec(args, null, outputDir);
//						System.out.println("started rpm2cpio");
						
						String args2[] = new String[3];
						args2[0] = "/bin/cpio";
						args2[1] = "-idmu";
						args2[2] = "--quiet";
						Process cpioProcess = runtime.exec(args2, null, outputDir);
//						System.out.println("started cpio");
						
						InputStream fromRpm2cpio = rpm2cpioProcess.getInputStream();
						new ProcessInputStreamer(rpm2cpioProcess.getErrorStream());
						OutputStream fromCpio = cpioProcess.getOutputStream();
						new PipedStreamer(fromRpm2cpio, fromCpio);
						new ProcessInputStreamer(cpioProcess.getErrorStream());
//						System.out.println("pipe created");
						while(cpioProcess!=null && fromRpm2cpio!=null) {
							boolean doSleep = true;
//							System.out.println("waking up");
							if (cpioProcess!=null) {
								try {
									if (cpioProcess.exitValue()==0) {
										cpioProcess = null;
										doSleep = false;
									}
								} catch(IllegalThreadStateException e) {
								}
							}
							if (rpm2cpioProcess!=null) {
								try {
									if (rpm2cpioProcess.exitValue()==0) {
										rpm2cpioProcess = null;
										doSleep = false;
									}
								} catch(IllegalThreadStateException e) {
								}
							}
							if (doSleep) {
								Thread.sleep(30);
							}
//							System.out.println("still running");
						}
						
////						+" | /bin/cpio -idmv";
//						ProcessInputStreamer ins = new ProcessInputStreamer(p.getInputStream());
//						ProcessInputStreamer ers = new ProcessInputStreamer(p.getErrorStream());
//						log("process started");
//						Thread.sleep(2000);
//						for(String ou : ins.output) {
//							log(ou);
//						}
//						for(String ou : ers.output) {
//							log(ou, Project.MSG_ERR);
//						}
//						int res = p.waitFor();
//						if (res!=0) {
//							throw new BuildException("process exited with:"+res);
//						}
//						log("Extracted to:"+dir.getCanonicalPath());

						
					} break;
					case ZIP : {
						
					} break;
				}
			}
			
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
		
		
//		byte buffer[] = new byte[8192];
//		try {
//			InputStream stream = resource.getInputStream();
//			URL url;
//			InputStream stream = url.openStream();
//			stream.
//			
//			int readCnt = stream.read(buffer);
//			if (readCnt>0) {
//				bb.put(buffer, 0, readCnt);
//			}
//		} catch (IOException e) {
//			throw new BuildException(e.getMessage());
//		}		
	}
	
	enum ArhiveType {
		RPM,
		DEB,
		EXE,
		ZIP
	}
	
	
	public void setUrl(String url) {
		this.url = url;
	}
	
	public void setRepro(String reproUrl) {
		this.repro = reproUrl;
	}
	
	public void setReproName(String name) {
		this.reproName = name;
	}
	
	
	public void setArchiveType(String arType) {
		if (arType.equalsIgnoreCase("rpm")) {
			archiveType = ArhiveType.RPM;
		} else if (arType.equalsIgnoreCase("deb")) {
			archiveType = ArhiveType.DEB;
		} else if (arType.equalsIgnoreCase("exe")) {
			archiveType = ArhiveType.EXE;
		}
		throw new RuntimeException("Unknown archive type: "+arType);
	}
	
	
	public void setExtract(boolean extractOnly) {
		this.extract = extractOnly;
	}
	
	public static byte[] download(URL url, File destFile) {
		byte result[] = null;
		try {
		    InputStream inputStream = null;
		    RandomAccessFile outfile = null;
			
			URLConnection uc = url.openConnection();
			
		    int contentLength = uc.getContentLength();
		    boolean lengthKnown = true;
		    if (contentLength<0) {
		    	result = new byte[0x10000];
		    	lengthKnown = false;
		    } else {
		    	result = new byte[contentLength];
		    }
		    int readSoFar = 0;
		    if (destFile.exists()) {
		    	long destFileLength = destFile.length();
		    	if (lengthKnown) {
		    		inputStream = new FileInputStream(destFile);
		    		long toRead = destFileLength;
		    		toRead = destFileLength<contentLength ? destFileLength : contentLength;
		    		while(readSoFar<toRead) {
		    			int left = (int) (toRead-readSoFar);
		    			int cnt = inputStream.read(result, readSoFar, left);
		    			if (cnt>0) {
		    				readSoFar+=cnt;
		    			} else {
		    				break;
		    			}
		    		}
		    		inputStream.close();
		    		if (readSoFar==contentLength) {
		    			return result;
		    		}
		    	} else {
		    		readSoFar = (int) destFileLength;
		    	}
		    	
		    	
	    		inputStream = uc.getInputStream();
		    	inputStream.skip(readSoFar);
		    	
		    	outfile = new RandomAccessFile(destFile, "rw");
		    	outfile.seek(readSoFar);
		    	
		    } else {
		    	outfile = new RandomAccessFile(destFile, "rw");
		    	inputStream = uc.getInputStream();
		    }
			long startTime = System.currentTimeMillis();
			long nextTimeOut= startTime+250;
			boolean stop = false;
			
			long badSpeedStart = 0;
			
			while(!stop) {
				int left = result.length;
				if (lengthKnown) {
					left = contentLength-readSoFar;
					if (left>16384) {
						left = 16384;
					}
				} 
				int readCount = inputStream.read(result, lengthKnown ? readSoFar : 0, left);
				if (readCount>0) {
					readSoFar += readCount;
				} else {
					stop = true;
				}
				
				long fp = outfile.getFilePointer();
				left = (int) (readSoFar-fp);
				if (left>0) {
					outfile.write(result, (int) fp, left);
				}

				
				long now = System.currentTimeMillis();
				if (now>nextTimeOut || stop) {
					int skipped = 0;
					while(nextTimeOut<now) {
						nextTimeOut+=250;
						skipped += 250;
					}
					
					long diff = (now-startTime)/1000;
					if (diff<1) {
						diff = 1;
					}
					long bytesPerSecond = readSoFar/diff;
					bytesPerSecond /= 1024;
					boolean reconnected = false;
					if (bytesPerSecond<10*1024) {
						if (badSpeedStart==0) {
							badSpeedStart = now;
						} else {
							if (skipped+(now-badSpeedStart)>4000) {
								inputStream.close();
								uc = url.openConnection();
								inputStream = uc.getInputStream();
								inputStream.skip(readSoFar);
								reconnected = true;
							}
							badSpeedStart = 0;
						}
					} else {
						badSpeedStart = 0;
					}

					if (lengthKnown) {
						int per = 100*readSoFar/contentLength;
						System.out.print((char) 0xD);
						System.out.print(readSoFar+"/"+contentLength+"["+per+"%] "+bytesPerSecond+"K/s" + (reconnected ? " [RECONNECTED]" : ""));
					} else {
						System.out.print((char) 0xD);
						System.out.print(readSoFar+" "+bytesPerSecond+"K/s" + (reconnected ? " [RECONNECTED]" : ""));
					}
					
				}
			}
			System.out.println();
			
			outfile.close();

			if (!lengthKnown) {
				FileInputStream fis = new FileInputStream(destFile);
				result = new byte[(int) destFile.length()];
				fis.read(result);
				fis.close();
			}
			
		} catch (Exception e) {
			e.printStackTrace();
		}

		return result;
	}
	
    private String loadUrlAsText(URL url) throws IOException {
    	String result = null;
    	for(int idx=0; idx<5; idx++) {
    		ByteArrayOutputStream baos = new ByteArrayOutputStream();
    		try {
    			URLConnection connection = url.openConnection();
    			connection.setReadTimeout(7000);
    			connection.setRequestProperty("User-Agent", "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0");
    			InputStream stream = connection.getInputStream();
		        
		        byte bbuf[] = new byte[0x10000];
		        while(true) {
		        	int cnt = stream.read(bbuf);
	                if (cnt<=0) {
	                        break;
	                }
	                baos.write(bbuf, 0, cnt);
		        }
		        idx=5;
		        result = new String(baos.toByteArray());
    		} catch(SocketTimeoutException e) {
    			System.out.println("[RECONNECTING]");
    		} catch(Exception e) {
    			if (idx==4) {
    				if (e instanceof IOException) {
    					throw (IOException) e;
    				}
    				e.printStackTrace();
    				return null;
    			}
    		}
    	}
        return result;
}

	
	
	URL extractUrl() throws Exception {
		if (url!=null) {
			log(LogLevel.INFO, "url="+url);
			return new URL(url);
		} 
		String text;
		if (repro.equals(lastLoadedReproUrl)) {
			text = lastText;
		} else {
			log(LogLevel.INFO, "reproUrl="+repro+", lastLoadedReproUrl="+lastLoadedReproUrl);
			text = loadUrlAsText(new URL(repro));
			lastLoadedReproUrl = repro;
			lastText = text;
		}
			
        String rpmlist[] = stripLoadableRpms(text);
        ArrayList<String> matchedRpms = new ArrayList<String>();
        
        boolean do_debug = reproName.indexOf("debug")>=0;
        
        for(String rpm : rpmlist) {
        	int idx = rpm.lastIndexOf('/');
        	String rpmName = rpm;
//        	log("rpm="+rpm, Project.MSG_VERBOSE);
        	if (idx>=0) {
        		rpmName = rpm.substring(idx+1);
        	}
        	boolean rpm_is_debug = rpmName.indexOf("debug")>=0;
        	if (rpm_is_debug!=do_debug) {
        		continue;
        	}
        	if (rpmName.matches(reproName)) {
        		matchedRpms.add(repro+"/"+rpmName);
        	}
        }
        if (matchedRpms.size()==1) {
        	return new URL(matchedRpms.get(0));
        } else if (matchedRpms.size()>1) {
        	Collections.sort(matchedRpms);
        	for(String mrpms : matchedRpms) {
//        		session.log("::"+mrpms);
        	}
        	return new URL(matchedRpms.get(0));
        }
        throw new RuntimeException("no repro matches "+reproName);
	}
	
	

    public String[] stripLoadableRpms(String text) {
        int idxa = 0;
        ArrayList<String> list = new ArrayList<String>();
        while(true) {
                int idxb = text.indexOf("<a href=\"mingw32-", idxa);
                if (idxb<=0) {
                    idxb = text.indexOf("<a href=\"mingw64-", idxa);
                    if (idxb<=0) {
                        break;
                    }
                }
                idxb += 9;
                int idxc = text.indexOf("\"", idxb);
                if (idxc<0) {
                        break;
                }
                String raw = text.substring(idxb, idxc);
                if (raw.endsWith(".rpm") && raw.indexOf("evolution")<0 && raw.indexOf("libqt4")<0 && raw.indexOf("webkit")<0 && raw.indexOf("mono")<0 && raw.indexOf("pidgin")<0 && raw.indexOf("texlive")<0 && raw.indexOf("tomahawk")<0) {
                        list.add(raw);
                }
                idxa = idxb;
        }
        String result[] = new String[list.size()];
        return list.toArray(result);
    }

//    public BackItUp() {
//        try {
//                String baseUrl = "http://download.opensuse.org/repositories/windows:/mingw:/win32/openSUSE_Factory/noarch/";
//                URL url = new URL(baseUrl);
//                byte repo_text[]  = download(baseUrl, new File("/tmp/natpad.repro"));
//                String text = new String(repo_text);
//                String rpmlist[] = stripLoadableRpms(text);
//                for(String rpm : rpmlist) {
//                        System.out.println("rpm::"+rpm);
//
//                        download(new URL(baseUrl+rpm), new File(".", rpm));
//                }
//
//                System.out.println("buf="+text);
//
//        } catch (Exception e) {
//                e.printStackTrace();
//        }
//    }
	
	
	
	public static void main(String[] args) {
		try {
			URL url = new URL("http://download.opensuse.org/repositories/windows:/mingw:/win32/openSUSE_Factory/noarch/mingw32-abiword-2.8.6-12.38.noarch.rpm?test=3");
			String filename = url.getPath();
			int idx = filename.lastIndexOf('/');
			if (idx>=0) {
				filename = filename.substring(idx+1);
			}
			System.out.println(filename);
			download(url, new File("mingw32-abiword-2.8.6-12.38.noarch.rpm"));
//			URLConnection uc = url.openConnection();
//		    String contentType = uc.getContentType();
//		    int contentLength = uc.getContentLength();
//		    
//			
//			System.out.println("contentLength:"+contentLength);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}


}
