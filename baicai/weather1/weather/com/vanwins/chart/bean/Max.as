//左下方的最大值
package com.vanwins.chart.bean
{
    import flash.display.Sprite;
    import flash.text.TextField;
    import flash.text.TextFormat;
	
	public class Max extends Sprite
	{

    	public var bgcolor: Number;

    	//public var issize: int;
		public var iscolor: Number;
    	public var isdisplay: Boolean;
    	public var strname: String;
    	public var unit: String;
    	public var isvalue: Number;

		public function Max()
		{

		}
		public function paint():void
		{
			var label = new TextField();			
			
			var format:TextFormat = new TextFormat();
			format.color = iscolor;
			format.size = 12;
			label.defaultTextFormat = format;
			label.text=strname + "：" + isvalue + unit;
			addChild(label);
		}
	}
}