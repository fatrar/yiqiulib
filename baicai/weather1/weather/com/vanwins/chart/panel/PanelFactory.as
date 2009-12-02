//使用类发射来确定按钮的类
package com.vanwins.chart.panel
{
	import flash.utils.*;
	import com.vanwins.chart.panel.Column;
	import com.vanwins.chart.panel.Dotted;
	
	public class PanelFactory
	{
		public static function newInstance(classpath:String):BasePanel
		{
			trace("........."+classpath);
			var classReference:Class =getDefinitionByName(classpath) as Class;
			trace(getDefinitionByName(classpath) as Class);
			var o:BasePanel = new classReference();
			return o;
		}
	}
}