//构造每一天的函数
 package com.vanwins.chart.bean
{
    import flash.display.Sprite;
    import flash.text.TextField;
    import flash.text.TextFormat;
	import flash.text.TextFieldAutoSize;
	
	public class Day extends Sprite
	{

    	public var datas: Array;

    	public var date: String;

    	public var weather: String;
		//datas.length
		//public var datasLen:int=datas.length;

		public function Day()
		{
			
		}
		public function paint()
		{
			var label = new TextField();			
			label.autoSize = TextFieldAutoSize.CENTER;
			var format:TextFormat = new TextFormat();
			//format.color = iscolor;
			format.size = 12;
			label.defaultTextFormat = format;
			label.text= date;
			addChild(label);
			//trace(date);
			
		}
	}
}