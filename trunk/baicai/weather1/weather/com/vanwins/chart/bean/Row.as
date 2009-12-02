//每一行的信息，如行标题，颜色等
package com.vanwins.chart.bean
{
	import flash.text.TextField;
    import flash.text.TextFieldAutoSize;
    import flash.text.TextFormat;
	import flash.display.*;
	
	public class Row extends MovieClip
	{
 
    	public var bgcolor: Number;

    	public var iscolor: Number;

    	public var isdisplay: Boolean;

    	public var unit: String;

    	public var isvalue: String;
		public var underLine : Boolean;
		
		public var startRowX:int=20;
		public var startDataX:int=79;
		public var endX:int=735;
		public var startY:int=49;
		public var endY:int=326;
		//public var betweenX:int;
		public var betweenY:int=29;

		public function Row()
		{
			
		}
		public function paint()
		{
			var mc:MovieClip=new MovieClip();
			
			var label = new TextField();
			//label.backgroundColor=bgcolor;
			var format:TextFormat = new TextFormat();
			format.color = iscolor;
			format.size = 12;
			format.bold=true;
			label.wordWrap = true;
			label.defaultTextFormat = format;
			if (uint.length>=5)
			{
				label.text=isvalue +"\r\n"+ unit;
			}
			else
				label.text=isvalue + unit;
			mc.addChild(label);
			label.x=-(startDataX+startRowX)/2;
			label.y=4;
			
			var shapeRight:Shape=new Shape();
			shapeRight.graphics.beginFill(bgcolor);
			shapeRight.graphics.drawRect(0,0,endX-startDataX,betweenY);
			shapeRight.graphics.endFill();
			mc.addChild(shapeRight);
			shapeRight.visible=underLine;
			/*if (underLine)
			mc.removeChild(shapeRight);*/
			
			var line:Shape=new Shape();
			line.graphics.lineStyle(2,0xFFFFFF);
			line.graphics.moveTo(startRowX-startDataX,betweenY-1);
			line.graphics.lineTo(endX-startDataX,betweenY-1);
			mc.addChild(line);
			line.visible=underLine;
			/*if (underLine)
			mc.removeChild(line);*/
			
			
			//trace(label.text.length);
			
			//trace(unit.length);
			
			addChild(mc);
			mc.visible=Boolean(isdisplay);
			mc.mouseEnabled=false;
		}
	}
}