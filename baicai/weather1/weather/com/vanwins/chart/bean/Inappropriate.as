///////////////////////////////////////////////////////////
//  Inappropriate.as
//  Created on:      23-ʮһÔÂ-2009 10:31:22
//  Original author: Administrator
///////////////////////////////////////////////////////////

/**
 * Á´½ÓÀà
 * @author Administrator
 * @version 1.0
 * @created 23-ʮһÔÂ-2009 10:31:22
 */
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