
package com.vanwins.chart.bean
{
    
	import flash.display.MovieClip;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextField;
	import flash.text.TextFormat;
	
 	public class SupperMyButton extends MovieClip
	{
    	public var bgcolor: Number=0x00ff00;
    	public var defaultselect: Number = 0;
    	public var eventclass: String;
    	public var fontcolor: Number;
    	public var parametername: String;
    	public var sbgcolor: Number;
    	public var sfontcolor: Number;
		public var s_width:Number = 80;
		public var s_height:Number = 30;
    	public var istext: String;
		public var btnR:RightBtn = new RightBtn();
		
		public function SupperMyButton()
		{
			
		}
		public function paint() 
		{
			//var btn:RightBtn = new RightBtn();
			addChild(btnR);
			btnR.x=20;
			btnR.y=10
			var label = new TextField();
			label.autoSize = TextFieldAutoSize.CENTER;
			var format:TextFormat = new TextFormat();
			format.color = fontcolor;
			format.size = 13;
			label.defaultTextFormat = format;
			label.text=istext;
			addChild(label);
			//label.x=5;
			//label.y=-10;
		}
	}
	/*public function paint()
{


}


var buttons:Array = app.GetButtons();
for (var i:Number = 0; i < buttons.length; i++ )
{
var btn:SupperMyButton = buttons[i];
btn.paint();
btn.x = 730-(buttons.length-i)*63;
btn.y=20
//btn.txt.text=buttons[i].istext;
//btn.txt.defaultTextFormat.bold=true;
btn.mouseChildren = false;
btn.addEventListener(MouseEvent.CLICK, butclick);
addChild(btn);
if(i==0)
{
btn.gotoAndStop(2);
}
}

*/
}