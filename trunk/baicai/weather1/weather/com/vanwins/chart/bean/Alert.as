//构造面板下部的link，inappropriate，ismax，ismin
 package com.vanwins.chart.bean
{
	
	import flash.display.Sprite;
	import flash.display.MovieClip;
	
	public class Alert extends MovieClip
	{

   		public var link: Link;
		public var inappropriate:Inappropriate;
    	public var ismax: Max;
    	public var ismin: Min;

		public function Alert()
		{}
		public function paint()
		{
			addChild(link);
			addChild(inappropriate);
			addChild(ismax);
			addChild(ismin);
		}
	}
}