package org.cocos2dx.lib;

import com.map.sunMap.*;

public class TileThread extends Thread {

	private String tileName;
	
	public TileThread(String tile) {
		tileName = tile;
	}
	
	public void run(){  
    	sunMap.download(tileName);  
    }  
}
