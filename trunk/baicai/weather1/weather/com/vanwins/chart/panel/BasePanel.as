//为Column和Dotted的父类
package com.vanwins.chart.panel 
{
	import flash.display.MovieClip;
	import com.vanwins.chart.config.*;

	
	public class BasePanel extends MovieClip
	{
		public var app:AppConfig;
		public var Wea:WeatherXmlLoader;
		/*public var startRowX:int=20;
		public var startDataX:int=79;
		public var endX:int=735;
		public var startY:int=49;
		public var endY:int=326;
		public var betweenX:int;
		public var betweenY:int=29;*/
		
		function BasePanel() 
		{
			
		}
		public function paint():void 
		{
			
		}
		
	}
}