﻿///////////////////////////////////////////////////////////
//  Title.as
//  Created on:      23-ʮһÔÂ-2009 10:31:18
//  Original author: Administrator
///////////////////////////////////////////////////////////

/**
 * ±êÌâÀà
 * @author Administrator
 * @version 1.0
 * @created 23-ʮһÔÂ-2009 10:31:18
 */
package com.vanwins.chart.bean
{
	import flash.text.TextField;
    import flash.text.TextFieldAutoSize;
    import flash.text.TextFormat;
	import flash.display.*;
	
 	public class BigTitle extends MovieClip
	{

    	public var bgcolor: Number=0x00ff00;
    	public var iscolor: Number;
		public var titSize: int;
    	public var isvalue: String;

		function BigTitle()
		{
			
		}
		public function paint():void
		{
			var label = new TextField();
			var format:TextFormat = new TextFormat();
			format.color = iscolor;
			format.size = titSize;
			format.bold=true;
			//format.font="黑体";
			label.defaultTextFormat = format;
			label.text=isvalue;
			addChild(label);
		}
	}
}