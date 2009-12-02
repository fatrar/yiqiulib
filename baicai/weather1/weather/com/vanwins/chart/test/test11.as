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
	
	
	public class test11 extends Sprite
	{
		private var col:Column;
		private var dot:Dotted;
		public var app:AppConfig;
		private var i:int = 0;
		public  var Wea:WeatherXmlLoader;
		public var stageHead:MovieClip;
		public var panel:BasePanel;

		public function test11()
		{
			app = new AppConfig("config/appconfig.xml",startto);
			Wea = new WeatherXmlLoader("config/wind.xml",startto);
		}
		private function startto()
		{
			i++;
			if(i==2)
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
				btn.mouseChildren = false;
				btn.addEventListener(MouseEvent.CLICK, butclick);
				

				if(i==0)
				{
					btn.btnR.gotoAndStop(2);
				}
				addChild(btn);
			}
			//trace("Wea.GetBody().days.datas");
			//trace(Wea.GetBody().days.length);
			//trace(Wea.GetBody().days[0].datas[0].head);
			//trace(Wea.GetBody().days[0].datas.length);
			panel = PanelFactory.newInstance("com.vanwins.chart.panel.Column");
			panel.app = app;
			panel.Wea = Wea;
			
			panel.paint();
			addChild(panel);
			
		}
		private function startto2(){
			panel.paint();
			addChild(panel);
			//trace(panel.Wea.GetHead().alert.link.strname);
		}
		private function butclick(e:MouseEvent)
		{
			removeChild(panel);
			panel = PanelFactory.newInstance(e.target.eventclass);
			panel.app = app;
			for(i=0;i<panel.app.GetButtons().length;i++)
			{
				if(panel.app.GetButtons()[i].parametername==e.target.parametername)
				{
					 e.target.btnR.gotoAndStop(2);
				}
				else
					panel.app.GetButtons()[i].btnR.gotoAndStop(1);
			}

			panel.Wea = new WeatherXmlLoader("config/"+e.target.parametername+".xml",startto2);	
		}
		
		private function remove():void
		{
			var k:int=numChildren;
			
			while(k--)
			{
				removeChildAt(k);		
			}
		}
	}
}