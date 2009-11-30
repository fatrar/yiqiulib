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
				
				//var xml=this.loaderInfo.parameters.earlyWarning;
				//$host=this.loaderInfo.parameters.picUrl;
				xml="config/wind.xml";				
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
		