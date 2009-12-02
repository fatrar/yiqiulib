//构造面板的头部
 package com.vanwins.chart.bean
{
	
	import flash.text.TextField;
    import flash.text.TextFieldAutoSize;
    import flash.text.TextFormat;
	import flash.ui.*;
	import flash.utils.*
	import flash.events.Event;
	import flash.display.Sprite;
	import com.vanwins.chart.config.*;
	
	
	public class Head extends Sprite
	{
    
    	public var alert: Alert;

    	public var rows: Rows;

    	public var subtitle: BigTitle;
    	public var istitle: BigTitle;
		//private var weatherXml:WeatherXmlLoader;
		public function Head()
		{
		}
		public function paint()
		{
			addChild(subtitle);
			addChild(istitle);
			addChild(alert);
			
		}
		
	}
}