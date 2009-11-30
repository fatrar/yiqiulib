///////////////////////////////////////////////////////////
//  Max.as
//  Created on:      23-ʮһÔÂ-2009 10:31:20
//  Original author: Administrator
///////////////////////////////////////////////////////////

/**
 * AlertÊôÐԶÔÏó
 * @author Administrator
 * @version 1.0
 * @created 23-ʮһÔÂ-2009 10:31:20
 */
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