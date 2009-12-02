package
{
	import flash.display.Sprite;
	import flash.display.MovieClip;
	import flash.display.DisplayObject;

	//import com.vanwins.chart.config.WeatherXmlLoader;
	import com.vanwins.chart.bean.*;
	import com.vanwins.chart.config.*;
	import com.vanwins.chart.panel.*;
	import flash.text.*;
	import flash.events.*;
	import flash.events.MouseEvent;
	import cn.cold37.app.*;
	
	
	public class main extends Sprite
	{
		private var col:Column;
		private var dot:Dotted;
		public var app:AppConfig;
		private var i:int = 0;
		public  var Wea:WeatherXmlLoader;
		public var stageHead:MovieClip;
		public var panel:BasePanel;
		
		private var __loadclip:Loading;

		public function main()
		{
			super();
			//stop();
			
			__loadclip=new Loading;
			addChild(__loadclip);
			__loadclip.x=stage.stageWidth/2-120;
			__loadclip.y=stage.stageHeight/2-30;
			
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
			removeChild(__loadclip);
			app = new AppConfig("config/appconfig.xml",showto);
			//Wea = new WeatherXmlLoader("config/wind.xml",startto);
		}
		/*private function startto()
		{
			i++;
			if(i==2)
				showto();
		}*/
		
		private function showto()
		{
			
			//trace("Wea.GetBody().days.datas");
			//trace(Wea.GetBody().days.length);
			//trace(Wea.GetBody().days[0].datas[0].head);
			//trace(Wea.GetBody().days[0].datas.length);
			/*var _subtitle:BigTitle=Wea.GetHead().subtitle;
			_subtitle.paint();
			_subtitle.x=18;
			_subtitle.y=16;
			addChild(_subtitle);
			var _istitle:BigTitle=Wea.GetHead().istitle;
			_istitle.paint();
			_istitle.x=119;
			_istitle.y=15;
			_istitle.width=115;
			addChild(_istitle);*/
			
			var buttons:Array = app.GetButtons();
			for (var i:Number = 0; i < buttons.length; i++ ) 
			{
				/*var btn:SupperMyButton = buttons[i];
				btn.paint();
				btn.x = 710-(buttons.length-i)*63;
				btn.y=10
				btn.mouseChildren = false;
				btn.addEventListener(MouseEvent.CLICK, butclick);
				addChild(btn);
*/
				if(buttons[i].defaultselect==1)
				{
					//btn.btnR.gotoAndStop(2);
					Wea = new WeatherXmlLoader(this.loaderInfo.parameters[buttons[i].parametername],startto1);
					//this.loaderInfo.parameters["wind"]
					panel = PanelFactory.newInstance(buttons[i].eventclass);
				}				
			}
		}
		private function startto1()
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
				addChild(btn);

				if(buttons[i].defaultselect==1)
				{
					btn.btnR.gotoAndStop(2);
					//Wea = new WeatherXmlLoader(this.loaderInfo.parameters[buttons[i].parametername],startto1);
					//this.loaderInfo.parameters["wind"]
					//panel = PanelFactory.newInstance(buttons[i].eventclass);
				}				
			}
			
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