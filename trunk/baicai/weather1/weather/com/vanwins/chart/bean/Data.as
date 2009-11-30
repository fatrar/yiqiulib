///////////////////////////////////////////////////////////
//  Data.as
//  Created on:      23-ʮһÔÂ-2009 10:31:24
//  Original author: Administrator
///////////////////////////////////////////////////////////

/**
 * ×ÓÊý¾Ý
 * @author Administrator
 * @version 1.0
 * @created 23-ʮһÔÂ-2009 10:31:24
 */
 package com.vanwins.chart.bean
{
	import flash.text.TextField;
    import flash.text.TextFieldAutoSize;
    import flash.text.TextFormat;
	import flash.display.*;
	
	public class Data extends Sprite
	{

    	public var head: String;

    	public var val4: String;

    	public var isvalue: Number;

		public function Data()
		{

		}
		public function paint()
		{
			var label = new TextField();			
			//label.autoSize = TextFieldAutoSize.CENTER;
			var format:TextFormat = new TextFormat();
			//format.color = iscolor;
			format.size = 12;
			label.defaultTextFormat = format;
			label.text= head;
			addChild(label);
			
			
		}
	}
}