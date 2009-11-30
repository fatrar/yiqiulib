package com.vanwins.chart.config {
	import flash.events.Event;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import com.vanwins.chart.bean.*;
	public class AppConfig{
		public var appConfigXml:XML;
		public var appConfigUrl:String;
		private var buttons:Array;
		private var weathers:Array;
		private var winds:Array;
		private var item:XML;
		private var onloaded:Function;
		function AppConfig(url:String,onloaded:Function) {
			this.appConfigUrl = url;
			this.onloaded = onloaded;
			this.buttons = new Array();
			this.weathers = new Array();
			this.winds = new Array();
			var loader:URLLoader = new URLLoader();
			var request:URLRequest = new URLRequest( this.appConfigUrl );
			loader.load( request);
			loader.addEventListener( Event.COMPLETE , onComplete );
		}
		function onComplete( event:Event ): void
		{
			var loader:URLLoader = event.target as URLLoader;
			if( loader != null )
			{
				appConfigXml = new XML( loader.data );
				for each(item in appConfigXml.weathers.weather )
				{
					var wea:Weather = new Weather();
					wea.strname = item.@strname;
					wea.msg = item.@msg;
					wea.src = item.@src;
					this.weathers.push(wea);
				}
				for each(item in appConfigXml.buttons.button )
				{
					
					var button:SupperMyButton = new SupperMyButton();
					button.eventclass = item.@eventclass;
					button.parametername = item.@parametername;
					button.istext = item.@text;
					button.bgcolor = item.@bgcolor;
					button.sbgcolor = item.@sbgcolor;
					button.sfontcolor = item.@sfontcolor;
					button.fontcolor = item.@fontcolor;
					button.defaultselect = item.@defaultselect;
					this.buttons.push(button);
				}
				for each(item in appConfigXml.winds.wind )
				{
					var wind:Wind = new Wind();
					wind.msg = item.@msg;
					wind.strname = item.@strname;
					wind.val = item.@val;
					this.winds.push(wind);
				}
				this.onloaded();
			 }
			else
			{
				throw new Error(this.appConfigUrl + "加载失败");
			}
		}
		
		public function GetButtons():Array {
			return this.buttons;
		}
		public function GetWeathers():Array {
			return this.weathers;
		}
		public function GetWinds():Array {
			return this.winds;
		}
	}
}