//右下方的link
 package com.vanwins.chart.bean
{
    import flash.display.Sprite;
    import flash.text.TextField;
  
    import flash.text.TextFormat;
	
	public class Link extends Sprite
	{

    	public var bgcolor: Number;
    	public var iscolor: Number;
    	public var isdisplay: Boolean;
    	public var href: String;
    	public var strname: String;
		//public var issize:int;

		public function Link()
		{

		}
		public function paint()
		{
			var label = new TextField();			
			var format:TextFormat = new TextFormat();
			format.color = iscolor;
			format.size = 12;
			label.defaultTextFormat = format;
			label.htmlText="<a href='"+href+"'> "+strname+"</a>";
			addChild(label);
		}
	}
}