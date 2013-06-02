package org.cocos2dx.lib;

import java.util.Stack;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import org.cocos2dx.sunmap.*;
import org.cocos2dx.lib.TileThread;


public class TileLoaderJni implements Runnable {
	private static final int MAX_THREADS = 8;
	private Stack<String> loadQueue = new Stack<String>();
	private int counter = 0;
	
	public synchronized void load(String tile) {
		addToQueue(tile);
	}

	public synchronized void addToQueue(String tile) {
		loadQueue.push(tile);
	}

	public synchronized String getFromQueue() {
		return loadQueue.pop();
	}

	
	public void run() {
		while (true) {
			try {
				Thread.sleep(100);
				if (counter < MAX_THREADS && loadQueue.size() > 0) {
					String rt = getFromQueue();
//					
						//counter++;
						new TileThread(rt).start();  
					}		
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

}
