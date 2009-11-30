///////////////////////////////////////////////////////////
//  Rows.as
//  Created on:      23-ʮһÔÂ-2009 10:31:18
//  Original author: Administrator
///////////////////////////////////////////////////////////

/**
 * Ðм¯ºÏÀà
 * @author Administrator
 * @version 1.0
 * @created 23-ʮһÔÂ-2009 10:31:18
 */
package com.vanwins.chart.bean
{
	import flash.display.Shape;
	import flash.display.MovieClip;
	
	public class Rows extends MovieClip
	{
    	public var values: Array;
		public var iscolor:Number;
		public var bgcolor:Number;
		
		public var startRowX:int=20;
		public var startDataX:int=79;
		public var endX:int=735;
		public var startY:int=49;
		public var endY:int=326;
		public var betweenX:int;
		public var betweenY:int=29;

		public function Rows()
		{

		}
		public function paint()
		{
			var shapeLeft:Shape=new Shape();
			shapeLeft.graphics.beginFill(bgcolor);
			shapeLeft.graphics.drawRect(0,0,startDataX-startRowX,endY-startY);
			//shapeLeft.graphics.lineStyle(1,0xFFFFFF);
			//shapeLeft.graphics.moveTo(0,0);
			//shapeLeft.graphics.lineTo(endX,0);
			
			shapeLeft.graphics.endFill();
			addChild(shapeLeft);
			
		}
	}
}