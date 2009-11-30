package com.vanwins.chart.test
{
	import flash.display.Sprite;
	import flash.display.MovieClip;
	import flash.display.DisplayObject;

	//import com.vanwins.chart.config.WeatherXmlLoader;
	import com.vanwins.chart.bean.*;
	import com.vanwins.chart.config.*;
	import com.vanwins.chart.panel.*;
	import flash.text.*;
	import flash.events.MouseEvent;
	
	
	public class test1 extends Sprite
	{
		private var col:Column;
		private var dot:Dotted;
		public var app:AppConfig;
		private var i:int = 0;
		public  var Wea:WeatherXmlLoader;
		public var Rain:WeatherXmlLoader;
		public var Rel:WeatherXmlLoader;
		public var Temp:WeatherXmlLoader;
		public var stageHead:MovieClip;
		public var panel:BasePanel;
//		private var panel1:BasePanel;
		public function test1()
		{
			app = new AppConfig("config/appconfig.xml",startto);
			Wea = new WeatherXmlLoader("config/wind.xml",startto);
			Rain= new WeatherXmlLoader("config/rainfall.xml",startto);
			Rel = new WeatherXmlLoader("config/relativehumidity.xml",startto);
			Temp= new WeatherXmlLoader("config/temperature.xml",startto);
			
			//Wea = new WeatherXmlLoader("config/temperature.xml",startto);			
		}
		private function startto()
		{
			i++;
			if(i==5)
			showto();			
		}
		private function showto()
		{
			var _subtitle:BigTitle=Wea.GetHead().subtitle;
			_subtitle.paint();
			_subtitle.x=18;
			_subtitle.y=16;
			addChild(_subtitle);
			var _istitle:BigTitle=Wea.GetHead().istitle;
			_istitle.paint();
			_istitle.x=119;
			_istitle.y=15;
			_istitle.width=115;
			addChild(_istitle);
			
			var buttons:Array = app.GetButtons();
			for (var i:Number = 0; i < buttons.length; i++ ) 
			{
				var btn:SupperMyButton = buttons[i];
				btn.paint();
				btn.x = 710-(buttons.length-i)*63;
				btn.y=10
				//btn.txt.text=buttons[i].istext;
				//btn.txt.defaultTextFormat.bold=true;
				btn.mouseChildren = false;
				btn.addEventListener(MouseEvent.CLICK, butclick);
				

				if(i==0)
				{
					btn.gotoAndStop(2);
				}
				addChild(btn);
			}
			trace("Wea.GetBody().days.datas");
			trace(Wea.GetBody().days.length);
			trace(Wea.GetBody().days[0].datas[0].head);
			trace(Wea.GetBody().days[0].datas.length);
			
			panel = PanelFactory.newInstance("com.vanwins.chart.panel.Column");
			panel.app = app;
			panel.Wea = Wea;
			
			panel.paint();
			addChild(panel);
			
		}
		private function butclick(e:MouseEvent)
		{
			removeChild(panel);
			//var _name=getChildByName(DisplayObject(e.target));
			trace("@@@@@@@@_name  config/"+e.target.parametername+".xml");
			
			panel = PanelFactory.newInstance(e.target.eventclass);
			trace("aaaaaaaaaaa"+e.target.eventclass);
			panel.app = app;
			if(e.target.parametername=="wind")
			{
				panel.Wea = Wea;
			}
			if(e.target.parametername=="rainfall")
			{
				panel.Wea = Rain;
			}
			if(e.target.parametername=="temperature")
			{
				panel.Wea = Temp;
			}
			if(e.target.parametername=="relativehumidity")
			{
				panel.Wea = Rel;
			}
			panel.paint();
			addChild(panel);
		}
		
		private function remove():void
		{
			var k:int=numChildren;
			
			while(k--)
			{
				removeChildAt(k);		
			}
		}
		/*function loadedapp(){
			//wea = new WeatherXmlLoader("config/wind.xml",loaded);
			//panel1 = PanelFactory.newInstance(app.getButtons()[0]);
			//panel1.appConfig = app;
			
		}
		function loaded()
		{
			//m_head=new Head;
			//trace(app.GetBody().days[1].date);
			//trace(app.GetHead().istitle.isvalue);
		}*/
		
		
	}
}