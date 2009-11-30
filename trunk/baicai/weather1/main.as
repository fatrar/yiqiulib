package 
{	
	import flash.display.*;
	import flash.events.*;
	import flash.text.*;
	import flash.geom.*;
	import flash.ui.*;
	import flash.utils.*;
	//import com.furWeather.*;
	import cn.cold37.app.*;
	import gs.TweenLite;
	
	public class main extends MovieClip
	{		
		private var __loadclip:Loading;
		private var $xml:Object=new Object;
		
		//将气温，风向，湿度和降水分别排入数组 qiwenArr,fengxiangArr,shiduArr,jiangshuiArr
		private var qiwenNumArr:Array=new Array;
		private var fengxiangNumArr:Array=new Array;
		private var shiduNumArr:Array=new Array;
		private var jiangshuiNumArr:Array=new Array;
		
		//风向排入数组windDirectArr
		private var windDirectArr:Array=new Array;
		
		//图标的起始x的坐标值
		private var picStartX:Number=58;
		//图标的y轴范围内的最低的坐标值
		private var picStartY:Number=262;
		//图标的y轴范围内的最高的坐标值
		private var picEndY:Number=82;
		//图标的x轴之间的间隔值（仅温度，其他的需要乘以2）
		private var picXbetween:Number=33;
		//点击图片时显示数据
		private var _infoBox:MovieClip;
		
		//控制时间间隔来画图
		private var controler;
		//控制时间间隔来删除圆
		private var tempControler;
		//控制时间价格来删除线
		private var routeControler;
		private var i;//:int=0;
		private var j:int=0;
		//气温Y轴最大和最小值
		private var qiwenMaxY:int=40;
		private var qiwenMinY:int=-4;
		//风向Y轴最大和最小值
		private var fengxiangMaxY:int=16;
		private var fengxiangMinY:int=0;
		//湿度Y轴最大和最小值
		private var shiduMaxY:int=100;
		private var shiduMinY:int=0;
		//降水Y轴最大和最小值
		private var jiangshuiMaxY:int=14;
		private var jiangshuiMinY:int=0;
		private var time:MovieClip;
		//画线
		private var routeAll:Sprite;
		//画图，影片剪辑
		private var pic:MovieClip;
		//判断是否全部清除
		private var clearAll:Boolean;
		private var route;
		//画温度的圆
		private var _temp:MovieClip;
		//画湿度的五角星
		private var _wed;
		//画风向的风标
		private var _wind;
		//画降水量的矩形
		private var _rain;
		//图片的总数
		private var picNum:int;
		//线的总数
		private var routeNum:int;
		//星期
		var weekday:Array=[];
		//日期
		var weekdate:Array=[];
		//最低温度值
		private var minTempText:Number;
		//最高温度值
		private var maxTempText:Number;
		//最大风速值
		private var maxWindText:Number;
		//最大湿度值
		private var maxWedText:Number;
		//最大降水值
		private var maxRainText:Number;
		//右上角的四个按钮
		private var _fengxiang:MovieClip=new fengxiang;
		private var _qiwen:MovieClip=new qiwen;
		private var _shidu:MovieClip=new shidu;
		private var _jiangshui:MovieClip=new jiangshui;

		
		
		public function main()
		{
			super();
			stop();
			
			__loadclip=new Loading;
			addChild(__loadclip);
			
			this.loaderInfo.addEventListener(ProgressEvent.PROGRESS,loading);
			this.loaderInfo.addEventListener(Event.COMPLETE,loaddata);
		}
		
		private function loading(e:ProgressEvent) 
		{
			var loadpre:int=e.bytesLoaded/e.bytesTotal*100;
			__loadclip.setload(loadpre/100,"程序装载"+loadpre+" %");
			trace("===-=-"+loadpre);
		}
		
		private function loaddata(e:Event) 
		{
			this.addEventListener(Event.ENTER_FRAME,enterframeHandler);//添加enter_frame侦听
		}
		
		private function enterframeHandler(e:Event) 
		{
			if (currentFrame == 1) 
			{
				this.removeEventListener(Event.ENTER_FRAME,enterframeHandler);
				removeChild(this.getChildAt(0));
				nextFrame();
				
				var xml=this.loaderInfo.parameters.earlyWarning;
				//$host=this.loaderInfo.parameters.picUrl;
				xml="sz7day.xml";				
				$xml=new LoadXML(xml,configXML);
			}
		}
		
		
		private function configXML(e:Event)
		{
			
			var xml=$xml.xml.data.sz7day.child("item");			
			//将气温，风向，湿度，降水读入数组
			for(i=0;i<xml.length();i++)
			{
				var MaxTemp=xml.maxTemperature[i];
				qiwenNumArr.push(MaxTemp);
				var MinTemp=xml.minTemperature[i];
				qiwenNumArr.push(MinTemp);
				var fengxiangNum=xml.wind[i];
				var shiduNum=xml.wed[i];
				var jiangshuiNum=xml.rain[i];
				var windDirect=xml.windDirection[i];
				fengxiangNumArr.push(fengxiangNum);
				shiduNumArr.push(shiduNum);
				jiangshuiNumArr.push(jiangshuiNum);
				windDirectArr.push(windDirect);
				var weekdayPeriod:String=xml.period[i];
				weekday.push(weekdayPeriod.substr(weekdayPeriod.length-3,weekdayPeriod.length-1));				
				weekdate.push(weekdayPeriod.substr (0,weekdayPeriod.length-3));			
				
			}
			//计算各个方面的最大值和最小值
			minTempText=getMin(qiwenNumArr);
			maxTempText=getMax(qiwenNumArr);
			maxWindText=getMax(fengxiangNumArr);
			maxWedText=getMax(shiduNumArr);
			maxRainText=getMax(jiangshuiNumArr);
			//主程序
			init();
		}			
		
		//主程序，
		private function init()
		{
			//var j:int=0;
			//添加显示数据框，设为不可见
			_infoBox = new infoBox;
			addChild(_infoBox);
			_infoBox.visible=false;
			//添加画线线条
			routeAll = new Sprite();
			addChild(routeAll);
			//添加画图图片
			pic = new MovieClip();
			addChild(pic);
			
			//添加右上角四个按钮
			_fengxiang=new fengxiang;
		    _qiwen=new qiwen;
			_shidu=new shidu;
			_jiangshui=new jiangshui;
			addChild(_qiwen);
			_qiwen.x=275.5;
			_qiwen.y=19;
			_qiwen.gotoAndStop(2);
			addChild(_fengxiang);
			_fengxiang.x=338.4;
			_fengxiang.y=19;			
			addChild(_shidu);
			_shidu.x=401.4;	
			_shidu.y=19;
			addChild(_jiangshui);
			_jiangshui.x=464.4;
			_jiangshui.y=19;
			//为右上角四个按钮添加事件侦听器
			_qiwen.addEventListener(MouseEvent.CLICK,gotoQiwen);			
			_fengxiang.addEventListener(MouseEvent.CLICK,gotoFengxiang);
			_shidu.addEventListener(MouseEvent.CLICK,gotoShidu);
			_jiangshui.addEventListener(MouseEvent.CLICK,gotoJiangshui);
						
			//将xml时间和日期值读入文本框
			for(i=0;i<weekday.length;i++)
			{
				bigBox["w"+i].text=weekday[i];
				bigBox["d"+i].text=weekdate[i];
			} 
			//每隔时间间隔执行作图函数drawTemp
			controler=setInterval(drawTemp,10);
		}
		
		//取得数组中的最大值
		public function getMax(Arr:Array):Number
		{
			var Max:Number=Arr[0];
			for(i=1;i<Arr.length;i++)
			{
				if(Max<Arr[i])
				Max=Arr[i];
			}
			return Max;
		}
		//取得数组中的最小值
		public function getMin(Arr:Array):Number
		{
			var Min:Number=Arr[0];
			for(i=1;i<Arr.length;i++)
			{
				if(Min>Arr[i])
				Min=Arr[i];
			}
			return Min;
		}
		
		private function clearPic()
		{
			if(i>=0)
			{
				//将图向picStartY移动，透明度逐减
				TweenLite.to(pic.getChildAt(i), 1, { y:picStartY, alpha:0});
				//删除画图的线
				if(i>0 && routeAll.numChildren>0)
				{
					routeAll.removeChildAt(i-1);
				}
				i--;
				
			}			
			else
			{
				//取消执行tempControler，即clearPic
				clearInterval(tempControler);
				//删除图形
				for(picNum=pic.numChildren-1;picNum>=0;picNum--)
				{
					pic.removeChildAt(picNum);
				}
				//重新赋初值
				j=0;
				picStartX=58;
				//每隔时间间隔执行作图函数drawTemp
				controler=setInterval(drawTemp,10);
			}
		}

		
		//按下_qiwen所执行的函数
		private function gotoQiwen(event:MouseEvent):void
		{
			//如果当前bigBox不在第一帧
			if(bigBox.currentFrame!=1)
			{
				//i为图片个数减1
				i=pic.numChildren-1;
				//每隔时间间隔执行作图函数clearPic,删除图和线
				tempControler=setInterval(clearPic,40);
				
				_jiangshui.gotoAndStop(1);
				_shidu.gotoAndStop(1);
				_fengxiang.gotoAndStop(1);
				_qiwen.gotoAndStop(2);
				bigBox.gotoAndStop(1);
			}
		}
		//按下_fengxiang所执行的函数
		private function gotoFengxiang(event:MouseEvent):void
		{
			if(bigBox.currentFrame!=2)
			
			{
				i=pic.numChildren-1;
				tempControler=setInterval(clearPic,40);
				_jiangshui.gotoAndStop(1);
				_shidu.gotoAndStop(1);
				_fengxiang.gotoAndStop(2);
				_qiwen.gotoAndStop(1);
				bigBox.gotoAndStop(2);
			}
		}
		//按下_shidu所执行的函数
		private function gotoShidu(event:MouseEvent):void
		{
			if(bigBox.currentFrame!=3)
			
			{
				i=pic.numChildren-1;
				tempControler=setInterval(clearPic,40);
				_jiangshui.gotoAndStop(1);
				_shidu.gotoAndStop(2);
				_fengxiang.gotoAndStop(1);
				_qiwen.gotoAndStop(1);
				bigBox.gotoAndStop(3);
			}
		}
		//按下_jiangshui所执行的函数
		private function gotoJiangshui(event:MouseEvent):void
		{
			if(bigBox.currentFrame!=4)
			{
				i=pic.numChildren-1;
				tempControler=setInterval(clearPic,40);
				_jiangshui.gotoAndStop(2);
				_shidu.gotoAndStop(1);
				_fengxiang.gotoAndStop(1);
				_qiwen.gotoAndStop(1);
				bigBox.gotoAndStop(4);
			}
		}
		
		//画温度，风向，湿度，降水动画
		private function drawTemp()
		{
			//画温度动画
			if(bigBox.currentFrame==1)
			{
				//显示最高温度和最低温度
				bigBox["minTemp"].text=minTempText;
				bigBox["maxTemp"].text=maxTempText;
				
				if(j<qiwenNumArr.length-1)
				{
					//画图片
					_temp=new temperature;
					pic.addChild(_temp);
					//图片鼠标悬停的显示
					_temp.addEventListener(MouseEvent.MOUSE_OVER,showQiwen);
					_temp.addEventListener(MouseEvent.MOUSE_OUT,disappearQiwen);
					_temp.x=picStartX;
					_temp.y=(picEndY-picStartY)*(qiwenNumArr[j]-qiwenMinY)/(qiwenMaxY-qiwenMinY)+picStartY;
					j++;
					picStartX+=picXbetween;
					//画线
					route=new Sprite;
					routeAll.addChild(route);
					route.graphics.lineStyle(2,0xCCCCCC);
					route.graphics.moveTo(_temp.x,_temp.y);
					route.graphics.lineTo(_temp.x+picXbetween,(picEndY-picStartY)*(qiwenNumArr[j]-qiwenMinY)/(qiwenMaxY-qiwenMinY)+picStartY);
				
				
				}
				else 
				{
					//画图片
					_temp=new temperature;
					pic.addChild(_temp);
					//图片鼠标悬停的显示
					_temp.addEventListener(MouseEvent.MOUSE_OVER,showQiwen);
					_temp.addEventListener(MouseEvent.MOUSE_OUT,disappearQiwen);
					_temp.x=picStartX;
					_temp.y=(picEndY-picStartY)*(qiwenNumArr[j]-qiwenMinY)/(qiwenMaxY-qiwenMinY)+picStartY;
					//停止调用controler，即drawTemp
					clearInterval(controler);
				}
			}
			//画风向动画
			else if(bigBox.currentFrame==2)
			{
				bigBox["maxWind"].text=maxWindText;
				if(j<fengxiangNumArr.length-1)
				{
					//画图片
					_wind=new wind;
					pic.addChild(_wind);
					//图片鼠标悬停的显示
					_wind.addEventListener(MouseEvent.MOUSE_OVER,showFengxiang);
					_wind.addEventListener(MouseEvent.MOUSE_OUT,disappearFengxiang);
					_wind.x=picStartX;
					_wind.y=(picEndY-picStartY)*(fengxiangNumArr[j]-fengxiangMinY)/(fengxiangMaxY-fengxiangMinY)+picStartY+10;
					//TweenLite.to(_wind, 0.5, {y:_wind.y-10, rotation:360});
					decideDirection(j);
					
					j++;
					picStartX+=picXbetween*2;
					//画线
					route=new Sprite;
					routeAll.addChild(route);
					route.graphics.lineStyle(2,0xCCCCCC);
					route.graphics.moveTo(_wind.x,_wind.y-10);
					route.graphics.lineTo(_wind.x+picXbetween*2,(picEndY-picStartY)*(fengxiangNumArr[j]-fengxiangMinY)/(fengxiangMaxY-fengxiangMinY)+picStartY);
				
				
				}
				else 
				{
					//画图片
					_wind=new wind;
					pic.addChild(_wind);
					//图片鼠标悬停的显示
					_wind.addEventListener(MouseEvent.MOUSE_OVER,showFengxiang);
					_wind.addEventListener(MouseEvent.MOUSE_OUT,disappearFengxiang);
					_wind.x=picStartX;
					_wind.y=(picEndY-picStartY)*(fengxiangNumArr[j]-fengxiangMinY)/(fengxiangMaxY-fengxiangMinY)+picStartY+10;
					decideDirection(j);
					//停止调用controler，即drawTemp
					clearInterval(controler);
				}
			}
			//画湿度动画
			else if(bigBox.currentFrame==3) 
			{
				bigBox["maxWed"].text=maxWedText;
				if(j<shiduNumArr.length-1)
				{
					//画图片
					_wed=new wed;
					pic.addChild(_wed);
					//图片鼠标悬停的显示
					_wed.addEventListener(MouseEvent.MOUSE_OVER,showShidu);
					_wed.addEventListener(MouseEvent.MOUSE_OUT,disappearShidu);
					_wed.x=picStartX;
					_wed.y=(picEndY-picStartY)*(shiduNumArr[j]-shiduMinY)/(shiduMaxY-shiduMinY)+picStartY;
					j++;
					picStartX+=picXbetween*2;
					//画线
					route=new Sprite;
					routeAll.addChild(route);
					route.graphics.lineStyle(2,0xCCCCCC);
					route.graphics.moveTo(_wed.x,_wed.y);
					route.graphics.lineTo(_wed.x+picXbetween*2,(picEndY-picStartY)*(shiduNumArr[j]-shiduMinY)/(shiduMaxY-shiduMinY)+picStartY);
				
				
				}
				else 
				{
					//画图片
					_wed=new wed;
					pic.addChild(_wed);
					//图片鼠标悬停的显示
					_wed.addEventListener(MouseEvent.MOUSE_OVER,showShidu);
					_wed.addEventListener(MouseEvent.MOUSE_OUT,disappearShidu);
					_wed.x=picStartX;
					_wed.y=(picEndY-picStartY)*(shiduNumArr[j]-shiduMinY)/(shiduMaxY-shiduMinY)+picStartY;
					//停止调用controler，即drawTemp
					clearInterval(controler);
				}
			}
			//画降水动画
			else if(bigBox.currentFrame==4) 
			{
				bigBox["maxRain"].text=maxRainText;
				if(j<jiangshuiNumArr.length)
				{
					//画图片
					_rain=new rain;
					pic.addChild(_rain);
					//图片鼠标悬停的显示
					_rain.addEventListener(MouseEvent.MOUSE_OVER,showJiangshui);
					_rain.addEventListener(MouseEvent.MOUSE_OUT,disappearJiangshui);
					_rain.x=picStartX;
					_rain.y=picStartY;
					var _rainScaleY:Number=(picEndY-picStartY)*(jiangshuiNumArr[j]-jiangshuiMinY)/(jiangshuiMaxY-jiangshuiMinY);
					//向上增长的动画效果
					TweenLite.to(_rain, 1, { scaleY:_rainScaleY});
					
					j++;
					picStartX+=picXbetween*2;					
				}
				else 
				{
					//停止调用controler，即drawTemp
					clearInterval(controler);
				}
			}
			
		}
		
		//根据风向确定箭头的方向，为有更好的动画效果，并向上移10像素
		private function decideDirection(j:int)
		{
			if(windDirectArr[j]=="西南")
			{
				TweenLite.to(_wind, 0.5, {y:_wind.y-10,rotation:45});
			}
			if(windDirectArr[j]=="西")
			{
				TweenLite.to(_wind, 0.5, {y:_wind.y-10,rotation:90});
			}
			if(windDirectArr[j]=="西北")
			{
				TweenLite.to(_wind, 0.5, {y:_wind.y-10,rotation:135});
			}
			if(windDirectArr[j]=="北")
			{
				TweenLite.to(_wind, 0.5, {y:_wind.y-10,rotation:180});
			}
			if(windDirectArr[j]=="东北")
			{
				TweenLite.to(_wind, 0.5, {y:_wind.y-10,rotation:225});
			}
			if(windDirectArr[j]=="东")
			{
				TweenLite.to(_wind, 0.5, {y:_wind.y-10,rotation:270});
			}
			if(windDirectArr[j]=="东南")
			{
				TweenLite.to(_wind, 0.5, {y:_wind.y-10,rotation:315});
			}
			if(windDirectArr[j]=="南")
			{
				TweenLite.to(_wind, 0.5, {y:_wind.y-10,rotation:360});
			}
		}
		//显示气温数据框
		private function showQiwen(evt:MouseEvent):void
		{
			_infoBox.visible=true;
			_infoBox.x=mouseX;
			_infoBox.y=mouseY-10;
			//得到当前对象的索引值
			i = pic.getChildIndex(DisplayObject(evt.target));
			
			_infoBox.infoMessage.text=qiwenNumArr[i];
		}
		
		//隐藏气温数据框
		private function disappearQiwen(evt:MouseEvent):void
		{
			_infoBox.visible=false;
		}
		
		//显示风向数据框
		private function showFengxiang(evt:MouseEvent):void
		{
			_infoBox.visible=true;
			_infoBox.x=mouseX;
			_infoBox.y=mouseY-10;
			//得到当前对象的索引值
			i = pic.getChildIndex(DisplayObject(evt.target));
			var fengxiangTxt:String=fengxiangNumArr[i]+""+ windDirectArr[i];
			_infoBox.infoMessage.text=fengxiangNumArr[i]+""+windDirectArr[i];
		}
		
		//隐藏风向数据框
		private function disappearFengxiang(evt:MouseEvent):void
		{
			_infoBox.visible=false;
		}
		
		//显示湿度数据框
		private function showShidu(evt:MouseEvent):void
		{
			_infoBox.visible=true;
			_infoBox.x=mouseX;
			_infoBox.y=mouseY-10;
			//得到当前对象的索引值
			i = pic.getChildIndex(DisplayObject(evt.target));
			
			_infoBox.infoMessage.text=shiduNumArr[i];
		}
		
		//隐藏湿度数据框
		private function disappearShidu(evt:MouseEvent):void
		{
			_infoBox.visible=false;
		}
		
		//显示降水数据框
		private function showJiangshui(evt:MouseEvent):void
		{
			_infoBox.visible=true;
			//得到当前对象的索引值
			i = pic.getChildIndex(DisplayObject(evt.target));
			_infoBox.x=evt.target.x;
			_infoBox.y=(picEndY-picStartY)*(jiangshuiNumArr[i]-jiangshuiMinY)/(jiangshuiMaxY-jiangshuiMinY)+picStartY-10;
			
			_infoBox.infoMessage.text=jiangshuiNumArr[i];
		}
		
		//隐藏降水数据框
		private function disappearJiangshui(evt:MouseEvent):void
		{
			_infoBox.visible=false;
		}
	}
}