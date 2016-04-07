# ThreeDimensionalModeling
该项目使用Visual Studio 2010完成。  
主要功能：  
1.根据用户指定的点，首先画出Bezier曲线，接着根据Bezier曲线画出茶壶的3D模型。  
2.根据用户指定的点画出Bezier曲线，然后根据此曲线进行茶壶的壶柄3D建模。  
3.根据用户指定的点画Bezier曲线，根据此曲线进行茶壶的壶嘴3D建模。  
4.利用键盘对茶壶进行旋转控制。  
5.模拟壶盖的翻转掉落动画。  
6.可以保存当前场景到一个静态文件，也可以导入静态文件解析出3D茶壶模型。  

首先介绍一下OpenGL的配置：    

1.glut.h,glaux.h复制到x:\Program Files\Microsoft\Visual Studio 10.0\VC\include\gl文件夹中,如果没有gl这个文件夹则可以自己新建一个。（x是你安装VS的盘符号）  
2.glut.lib,glut32.lib,glaux.lib放到静态函数库所在文件夹（即与include并排的lib文件夹下）  
3.glut.dll和glut32.dll放到操作系统目录下面的system32文件夹内。（典型的位置为：C:\Windows\System32）  

##下面是效果图
###3D茶壶
![3D茶壶](https://github.com/xiangkaiy/ThreeDimensionalModeling/blob/master/Images/1.jpg)  
  
    
###进行旋转控制  
![旋转之后的3D茶壶](https://github.com/xiangkaiy/ThreeDimensionalModeling/blob/master/Images/2.jpg)  
  
  
###模拟壶盖掉落
![模拟壶盖掉落](https://github.com/xiangkaiy/ThreeDimensionalModeling/blob/master/Images/3.jpg)
