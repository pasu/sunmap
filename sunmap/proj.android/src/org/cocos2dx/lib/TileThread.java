package org.cocos2dx.lib;

import org.cocos2dx.sunmap.sunmap;

public class TileThread extends Thread {

	private String tileName;
	
	public TileThread(String tile) {
		tileName = tile;
	}
	
	public void run(){  
    	sunmap.download(tileName);  
    }  
}
