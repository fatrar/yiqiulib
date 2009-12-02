//不适合比赛时段
 package com.vanwins.chart.bean
{
	import flash.text.TextField;
    import flash.text.TextFormat;
	import flash.display.*;
	
	public class Inappropriate extends MovieClip
	{
    	public var bgcolor: Number;
     	public var iscolor: Number;
    	public var isdisplay: Boolean;
     	public var unit: String;
    	public var strname: String;
		public var maxval: Number;
		public var minval: Number;
		//public var issize: int;
		
		

		public function Inappropriate()
		{
					
		}
		public function paint()
		{
			var shape:Shape = new Shape();
			shape.graphics.beginFill(iscolor);
			shape.graphics.drawRect(0,3,15,12);
			shape.graphics.endFill();
			addChild(shape);
			var label = new TextField();			
			var format:TextFormat = new TextFormat();
			format.color = iscolor;
			format.size = 12;
			label.defaultTextFormat = format;
			label.text=strname;
			addChild(label);
			label.x=16;
		}
	}
}