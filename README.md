### 一、Introduce/简介

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
1.GeoLocation  
2.Device with Map(Camera...)  
3.Web&APP,such as GeoLocation shared with your friends,location some importance things with Map.  
4.OpenLayers for C++,I think it is cool and a lot of things we can do.  

sunmap是一个跨平台的地图，你可以在iOS，Windows，Android，Mac OS等任何平台运行,我并没有在Linux平台运行过，但理论上应该没有任何问题。  
首先我有下面5点重要的事情先声明一下：  
1.sunmap是基于cocos2d-x上构建的地图框架,采用OpenGL ES的渲染方式  
2.sunmap只支持Web墨卡托投影坐标，比如Google，OSM，Bing,ArcGis online以及高德地图  
3.到目前为止，sunmap还有很多要做的事情，而且sunmap的工程结构还有一点混乱,主要是因为跨平台的一些文件存放路径有点随意而没有整理。  
如果你对cocos2d-x和Android NDK缺乏了解的话，我担心可能在Android平台运行时会游很多困扰，而且目前我没有提供任何文档作为参考，希望我能尽快完成这个吧（我不喜欢强迫自己做不太有意愿的事情）  
4.我很担心你是否能看到我的英文简介，不过我将努力让你明白我的中文介绍:)  
5.为什么命名为sunmap，因为我的老婆姓孙，和sun谐音，上个月她生了一场病，我并不期望任何事情，只保佑她能够享受生活，感悟生命。  

一些关于sunmap的详细信息  
你可以方便的扩展你的web墨卡托投影坐标地图，仅仅需要实现通过xyz拼接url的实现  
sunmap支持多点触摸的缩放和平移,多线程的瓦片下载，并且都是异步方式，你可以自设线程数目，默认为6  
sunma支持内存缓存数据，以及sqlite的数据库格式  

一些要做的事情
1.定位以及纠偏
2设备在地图的展现，比如相机，语音等
3.Web APP的方式，比如位置共享，定位功能的深入应用
4.SDK的方式，支持GML，一些vector的Geometry，style绘制，还有一些图表图形的展现效果等，参照OpenLayers的功能封装，这是我最感兴趣的，也是最有事可做的一个方向。

### 二、Show/效果展现

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

### 三、License/许可授权

I disclaims copyright to this source code,just bless you can use it and give me your suggestion.  
没有许可约束，可以任意使用。  

### 四、Contact/联系

Email: bjfubjfu@gmail.com  
Blog:   http://www.lugw.net/  
