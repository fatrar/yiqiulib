//解析weather/config中的除appconfig.xml以外的四个风向，湿度等的xml文件
package com.vanwins.chart.config
{
	import flash.events.*;
	import flash.text.*;
	import flash.geom.*;
	import flash.ui.*;
	import flash.utils.*
	import flash.events.Event;
	import flash.events.TimerEvent;
	import flash.display.*;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import com.vanwins.chart.bean.*;
	import flash.utils.Timer;
	
	public class WeatherXmlLoader
	{
		public var weatherXml:XML;
		public var weatherUrl:String;
		private var body:Body;
		private var head:Head;
		private var item:XML;
		private var onloaded:Function;
		private var days:Array=new Array();
		private var da:XML;
		
		public function WeatherXmlLoader(url:String,onloaded:Function) 
		{
			this.weatherUrl = url;
			this.onloaded = onloaded;
			var loader:URLLoader = new URLLoader();
			var request:URLRequest = new URLRequest( this.weatherUrl );
			loader.load( request);
			loader.addEventListener( Event.COMPLETE , onComplete );
		}
		function onComplete( event:Event ): void
		{
			var loader:URLLoader = event.target as URLLoader;
			if( loader != null )
			{
				head=new Head();
				body=new Body();
				var alert=new Alert();
				head.alert = alert;
				weatherXml = new XML( loader.data );
				for each(item in weatherXml.head.title )
				{
					var istitle = new BigTitle();
					istitle.isvalue = item.@value;
					istitle.iscolor = item.@color;
					istitle.titSize = item.@titSize;
					head.istitle = istitle;
				}
				for each(item in weatherXml.head.subtitle)
				{
					var subtitle = new BigTitle();
					subtitle.isvalue = item.@value;
					subtitle.iscolor = item.@color;
					subtitle.bgcolor = item.@bgcolor;
					subtitle.titSize = item.@titSize;
					head.subtitle=subtitle;
					
				}
				for each(item in weatherXml.head.rows )
				{
					var rows=new Rows();
					rows.iscolor = item.@color;
					rows.bgcolor = item.@bgcolor;
					rows.values=new Array();
					for each(da in item.row)
					{
						var row = new Row();
						row.isvalue=da.@value;
						row.unit=da.@unit;
						row.isdisplay=int(da.@display);
						row.iscolor=da.@color;
						row.bgcolor=da.@bgcolor;
						row.underLine=int(da.@underLine);
						rows.values.push(row);
					}
					head.rows=rows;
				}
				
				for each(item in weatherXml.head.alert.max )
				{
					var max=new Max();
					max.strname = item.@strname ;
					max.isdisplay = item.@display;
					max.isvalue = item.@value;
					max.unit = item.@unit;
					max.iscolor = item.@color;
					max.bgcolor = item.@bgcolor;
					//max.issize = item.@size;
					head.alert.ismax=max;
				}
				for each(item in weatherXml.head.alert.min )
				{
					var min=new Min();
					min.strname = item.@strname ;
					min.isdisplay = item.@display;
					min.isvalue = item.@value;
					min.unit = item.@unit;
					min.iscolor = item.@color;
					min.bgcolor = item.@bgcolor;
					//min.issize = item.@size;
					head.alert.ismin=min;
				}
				for each(item in weatherXml.head.alert.inappropriate)
				{
					alert.inappropriate=new Inappropriate();
					alert.inappropriate.strname = item.@strname ;
					alert.inappropriate.isdisplay = item.@display;
					alert.inappropriate.iscolor = item.@color;
					alert.inappropriate.bgcolor = item.@bgcolor;
					alert.inappropriate.unit = item.@unit;
					alert.inappropriate.maxval = item.@maxval;
					alert.inappropriate.minval = item.@minval;
					//alert.inappropriate.issize = item.@size;
					head.alert=alert;
				}
				for each(item in weatherXml.head.alert.link)
				{
					alert.link=new Link();
					alert.link.href = item.@href ;
					alert.link.strname = item.@strname ;
					alert.link.isdisplay = item.@display;
					alert.link.iscolor = item.@color;
					alert.link.bgcolor = item.@bgcolor;
					//alert.link.issize = item.@size;
					head.alert.link=alert.link;
				}
				for each(item in weatherXml.body)
				{
					var defaultcolor=new Number;
					body.defaultcolor = item.@defaultcolor ;
					var altercolor=new Number;
					body.altercolor = item.@altercolor ;
				}
				body.days = new Array();
				for each(item in weatherXml.body.day)
				{
					var day=new Day();
					day.date = item.@date ;
					day.weather = item.@weather;
					/*day.head = item.data.@head;
					day.value = item.data.@value;
					day.val4 = item.data.@val4;*/
					day.datas = new Array();
					for each(da in item.data)
					{
						var d = new Data();
						d.head=da.@head;
						d.isvalue=da.@value;
						d.val4=da.@val4;
						day.datas.push(d);
					}
					body.days.push(day);
				}
				this.onloaded();
			 }
			else
			{
				throw new Error(this.weatherUrl + "加载失败");
			}
		}
		
	
		
		
		public function GetBody()
		{
			return this.body;
		}
		public function GetHead()
		{
			return this.head;
		}
		
	}
}


