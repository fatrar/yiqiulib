///////////////////////////////////////////////////////////
//  Alert.as
//  Created on:      23-ʮһÔÂ-2009 10:31:20
//  Original author: Administrator
///////////////////////////////////////////////////////////



/**
 * ÌáʾÐÅϢÀà
 * @author Administrator
 * @version 1.0
 * @created 23-ʮһÔÂ-2009 10:31:20
 */
 package com.vanwins.chart.bean
{
	
	import flash.display.*;
	
	public class Alert extends MovieClip
	{

   		public var link: Link;
		public var inappropriate:Inappropriate;
    	public var ismax: Max;
    	public var ismin: Min;

		public function Alert()
		{}
		public function paint()
		{
			addChild(link);
			addChild(inappropriate);
			addChild(ismax);
			addChild(ismin);
		}
			/*link=new Link;
			inappropriate=new Inappropriate;
			ismax=new Max;
			ismin=new Min;
			addChild(link);
			addChild(inappropriate);
			addChild(ismax);
			addChild(ismin);
		}
		private function Link()
		{
			var label = new TextField();
			//label.backgroundColor=bgcolor;
			this.isvalue = val;
			this.iscolor = colo;
			this.titSize = sizes;
			var format:TextFormat = new TextFormat();
			format.color = iscolor;
			format.size = titSize;
			label.defaultTextFormat = format;
			label.text=isvalue;
			addChild(label);
			var txt:TextField=new TextField() ;
			addChild(txt);
			txt.x=245;
			txt.y=198+223*n;
			txt.htmlText="<a href='"+url+""strname"'></a>";
		}
		private function Inappropriate()
		{
			
		}
		private function Max()
		{
			
		}
		private function Min()
		{
			
		}*/
	}
}