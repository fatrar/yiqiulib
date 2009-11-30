package com.vanwins.chart.test{
	import flash.display.Sprite;
	import flash.utils.Timer;
	import flash.events.TimerEvent;
	import com.vanwins.chart.config.AppConfig;
	import com.vanwins.chart.bean.*;
	import flash.display.MovieClip;
	public class Test extends Sprite{
		private var app:AppConfig;
		function Test(){
			app = new AppConfig("config/appconfig.xml", showto);
		}
		function showto(){
			app.GetButtons()[0].paint();
			this.addChild(app.GetButtons()[0]);
			app.GetButtons()[1].paint();
			this.addChild(app.GetButtons()[1]);
			app.GetButtons()[2].paint();
			this.addChild(app.GetButtons()[2]);
		}
	}
}