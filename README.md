### 一、Introduction

sunmap is a cross-platform map,you can run it anywhere such as iOS,Android,Windows,Mac OS and so on.I have not test on Linux.  
There are five importance I have to say at first:  
1.sunmap is a map framework based on the cocos2d-x project,use coocs2d-x render(OpenGL ES).  
2.sunmap now only support Web Mercator projection such as Google Maps,OpenStreetMaps,Bing Maps,ArcGis online,Map ABC.  
3.Now,it still has a long way to go,and the project is a little disordered（main problem is about cross-platform folder）。I am afraid you can not run it on Android platform if you do not know much about cocos2d-x and Android NDK,
and I do not supply help document to help you,I hope I can do this soon：）  
4.I am not sure whether you can understand my poor English introduce,so I will try my best to make you understand my Chinese introduce:)  
5.Why I name this project as sunmap,because the first name of my wife is Sun(孙)，last month,She has been sick,I hope nothing just bless she could enjoy life and understand life  

Some detailed information about sunmap:  
you can extend your Web Mercator Map easily,just format the url with xyz.  
sunmap support multi-touch ZoomIn and ZoomOut,translation,multithreading tile download and all of them is asynchronous,you can set thread number,default 6  
sunmap support database file(Sqlite type:sqlitedb),and memory cache data.  

Some thing to do
0. Multi-thread,so we can separate  IO from Render,we can get much more benefit from it
1.GeoLocation  
2.Device for Map(Camera...)  
3.Web&APP,such as GeoLocation shared with your friends,location some importance things with Map.  
4.OpenLayers for C++,I think it is cool and a lot of things we can do.  

### 二、How to build it

For windows:
 1 Now,it supports all platforms by the view of technology
 2 I test it by the windows platform(Win7 VS2012)
 3 In the VS,you open the solution of sunMap.sln from the path "sunmap\projects\sunMap\proj.win32" and then build it,it will build successfully and you can debug it.
 4 In windows,you need copy the directory named "Resources" into the Debug or Release directory,remeber it,there is a direcotry named "Resources" in the folder "Resources",you can see it apparently.But in debug or release folder,we just need one "Resources" folder.
 5 I do not think it is too diffcult to run it in the Windows environment,if you have some problems ,you can send me the detail information.
 6 I will create the iOS and Android environment later~

### 三、Show/效果展现

 sunmap on Mac OS(10.8.3 mac mini google satelite)  
 ![image](https://github.com/pasu/sunmap/raw/master/images/mac.png)
 
 sunmap on iPad4(AMap)  
 ![image](https://github.com/pasu/sunmap/raw/master/images/ipad4.png)
 
 sunmap on Windows(Win7 64 laptop ArcGis online)  
 ![image](https://github.com/pasu/sunmap/raw/master/images/win7.png)
 
 sunmap on Android Pad(Moto Pad google vector)  
 ![image](https://github.com/pasu/sunmap/raw/master/images/moto.png)
 
 sunmap family photo  
 ![image](https://github.com/pasu/sunmap/raw/master/images/family.png)

### 四、License/许可授权

I disclaims copyright to this source code,just bless you can use it and give me your suggestion.  
没有许可约束，可以任意使用。  

### 五、Contact/联系

Email: bjfubjfu@gmail.com  
Blog:   http://www.lugw.net/  
