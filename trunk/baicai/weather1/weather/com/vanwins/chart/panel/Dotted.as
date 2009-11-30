
package com.vanwins.chart.panel 
{
	import com.vanwins.chart.bean.*;
	import com.vanwins.chart.config.*;	
	
	import flash.net.URLRequest;
    import flash.events.Event;
	import flash.display.*;
	import flash.display.Shape
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import flash.text.TextField;
	import flash.text.TextFormat;
	//import flash.
	
	import flash.geom.ColorTransform;
	public class Dotted extends BasePanel 
	{
		private var dayLen:int;
		private var dataLen:int;
		
		private var startRowX:int=20;
		private var startDataX:int=79;
		private var betweenY:int=29;
		private var startY:int=49;
		private var endY:int=326;
		private var endX:int=735;
		private var betweenX:int;
		private var picArr:Array=[];
		private var betweenUnderY:int;
		//画背景线的行数
		private var backRow:int=5;
		//存放点型容器
		private var _pic:MovieClip=new MovieClip();
		private var _dataInfo:TextField=new TextField();
		
		
		public function Dotted()
		{
			
		}
		public override function paint():void 
		{
			//添加右下角链接
			var _link:Link=Wea.GetHead().alert.link;
			_link.paint();
			_link.x=680
			_link.y=340;
			addChild(_link);
			
			//添加左下角的最小值
			var _min:Min=Wea.GetHead().alert.ismin;
			_min.paint();
			_min.x=20;
			_min.y=340;
			_min.width=110;
			addChild(_min);
			
			//添加左下角的最大值
			var _max:Max=Wea.GetHead().alert.ismax;
			_max.paint();
			_max.x=130;
			_max.y=340;
			addChild(_max);
			
			//添加下方的的不适合比赛阶段
			var Inapp=Wea.GetHead().alert.inappropriate;
			var _inappropriate:Inappropriate=Wea.GetHead().alert.inappropriate;
			_inappropriate.paint();
			_inappropriate.x=240;
			_inappropriate.y=340;
			addChild(_inappropriate);
			
			//添加左边的行集合
			var _rows:Rows=Wea.GetHead().rows;
			_rows.paint();
			_rows.x=startRowX;
			_rows.y=startY;
			addChild(_rows);
			
			//添加右边的行集合
			//添加除最后一个的所有行背景和线条
			var rowLen:int=Wea.GetHead().rows.values.length;
			//valueDisplayNum为Wea.GetHead().rows.values[i].isdisplay的num数
			var valueDisplayNum:int=0;
			for (var i:int=0;i<rowLen-1;i++)
			{
				var _row:Row=Wea.GetHead().rows.values[i];
				_row.paint();
				_row.x=startDataX;
				_row.y=startY+i*betweenY;
				addChild(_row);
				_row.visible=Boolean(Wea.GetHead().rows.values[i].isdisplay);
				if(Wea.GetHead().rows.values[i].isdisplay==1)
				{
					valueDisplayNum++;
				}
			}
			
			//trace(valueDisplayNum);
			//trace("valueDisplayNum");
			
			//添加最后一个的行背景和线条
			var _rowLast:Row=Wea.GetHead().rows.values[rowLen-1];
			_rowLast.paint();
			
			_rowLast.x=startDataX-20;
			_rowLast.y=startY+(rowLen-2)*betweenY+(endY-(rowLen)*betweenY)/2;
			//trace(_rowLast.text.length);
			_rowLast.width=400;
			addChild(_rowLast);			
			_rowLast.visible=Boolean(Wea.GetHead().rows.values[rowLen-1].isdisplay);
			
			
			//设置总共day的数目
			var dayNum:int=0;
			//设置总共data的数目
			var dataNum:int=0;
			//遍历得到day和data的数目
			for(i=0;i<Wea.GetBody().days.length;i++)
			{
				dayNum++;
				for(var j:int=0;j<Wea.GetBody().days[i].datas.length;j++)
				{
					dataNum++;
				}
			}
			//trace(dataNum);//值为20
			//算出横向的间隔值
			betweenX=(endX-startDataX)/dataNum;
			
			
			
			var backRoute = new Shape();
			addChild(backRoute);
			var betweenUnderY=(endY-startY-valueDisplayNum*betweenY)/(backRow);
			var betweenUnderX=(endX-startDataX)/(dataNum);
			backRoute.graphics.lineStyle(1,0xCCCCCC);
			for(j=1;j<=backRow;j++)
			{
				backRoute.graphics.moveTo(startDataX,startY+valueDisplayNum*betweenY+betweenUnderY*j);
				backRoute.graphics.lineTo(endX,startY+valueDisplayNum*betweenY+betweenUnderY*j);
				var kedutxt:TextField=new TextField();
				
			var format:TextFormat = new TextFormat();
			format.font="Tahoma";
			format.color =0x444444;
			format.size = 12;
			kedutxt.defaultTextFormat = format;
				kedutxt.text=String(Math.round(Inapp.maxval+5-(Inapp.maxval-Inapp.minval+10)/backRow*j));
				kedutxt.x=startDataX-7;
				kedutxt.y=startY+valueDisplayNum*betweenY+betweenUnderY*j-15;
				addChild(kedutxt);
			}
			for(j=0;j<=dataNum;j++)
			{
				backRoute.graphics.moveTo(startDataX+betweenX*j,startY+valueDisplayNum*betweenY);
				backRoute.graphics.lineTo(startDataX+betweenX*j,endY);
			}
			//k用来计每一天的data数
			var k:int=0;
			for(i=0;i<Wea.GetBody().days.length;i++)
			{
				//添加day的值，即日期一栏
				var _day:Day=Wea.GetBody().days[i];
				_day.paint();
				
				//添加swf
				var _loader:Loader= new Loader();
				addChild( _loader );
				//var showWeather:TextField=new TextField();
				//_loader.addChild(showWeather);
				for (j=0;j<app.GetWeathers().length;j++)
				{
					//trace(app.GetWeathers()[j].strname);
					if(app.GetWeathers()[j].strname==_day.weather)
					{
						 _loader.load( new URLRequest( app.GetWeathers()[j].src ) );
						 _loader.addEventListener(MouseEvent.MOUSE_OVER,showWeather);
						 _loader.addEventListener(MouseEvent.MOUSE_OUT,showWeather);
						// showWeather.text=app.GetWeathers()[j].msg;
					}
				}
				_loader.x=startDataX+betweenX*k+betweenX/2;
				_loader.y=startY+betweenY-5;
				_loader.scaleX=0.6;
				_loader.scaleY=0.6;
				//_loader.width=betweenY;
				//_loader.height=betweenY;
				
				//遍历每一天中的data
				
				for(j=0;j<Wea.GetBody().days[i].datas.length;j++)
				{
					//添加时间一栏
					var _data:Data=Wea.GetBody().days[i].datas[j];
					_data.paint();
					//将显示文本数值排入数组
					picArr.push(_data.isvalue);
					addChild(_data);
					_data.x=startDataX+betweenX*k+betweenX/2;
					_data.y=startY+4+betweenY*2;
					k++;
					
					//当为每一天的最后一个data时，画竖线
					if(j==Wea.GetBody().days[i].datas.length-1)
					{
						var shape:Shape=new Shape();
						shape.graphics.lineStyle(1,0xFFFFFF);
						//需要继续调式，需要知道长条的长度来算出准确的endX
						shape.graphics.moveTo(startDataX+betweenX*k+betweenX/4,startY);
						shape.graphics.lineTo(startDataX+betweenX*k+betweenX/4,startY+betweenY*4);
						addChild(shape);
					}
					
					/*var shape1:Shape=new Shape();
					shape1.graphics.lineStyle(1,0xFF00FF);
					shape1.graphics.moveTo(endX,startY);
					shape1.graphics.lineTo(endX,startY+betweenY*4);
					addChild(shape1);*/
					
					var dotX=startDataX+betweenX*(k-1)+betweenX/2;
					var tmp_x = (endY-startY-valueDisplayNum*betweenY);
					var tmp_z=(_data.isvalue-Inapp.minval+5);
					var tmp_y = (Inapp.maxval-Inapp.minval+10);
					var dotY=-tmp_x*tmp_z/tmp_y+ endY;
					var _line:Shape=new Shape();
					if(j!=Wea.GetBody().days[i].datas.length-1)
					{
						
						_line.graphics.lineStyle(2,0xCCCCCC);
						_line.graphics.moveTo(dotX,dotY);
						_line.graphics.lineTo(dotX+betweenX,-tmp_x*(Wea.GetBody().days[i].datas[j+1].isvalue-Inapp.minval+5)/tmp_y+ endY);
						addChild(_line);
					}
					else if(i!=Wea.GetBody().days.length-1)
					{
						
						_line.graphics.lineStyle(2,0xCCCCCC);
						_line.graphics.moveTo(dotX,dotY);
						_line.graphics.lineTo(dotX+betweenX,-tmp_x*(Wea.GetBody().days[i+1].datas[0].isvalue-Inapp.minval+5)/tmp_y+ endY);
						addChildAt(_line,10);
					}
					else
						{}
						
					//添加点型
					var _dot:dot=new dot();
					_pic.addChild(_dot);
					_dot.x= dotX;
					
					
					_dot.y= dotY;
										
					//改变颜色
					var colorInfo:ColorTransform;
					
					colorInfo=_dot.transform.colorTransform;
					if(_data.isvalue<Inapp.maxval && _data.isvalue>Inapp.minval)
					{						
						colorInfo.color=Wea.GetBody().defaultcolor;
					}
					else
					{
						
						colorInfo.color=Wea.GetBody().altercolor;						
					}
					
					_dot.transform.colorTransform=colorInfo;
					
					_dot.addEventListener(MouseEvent.MOUSE_OVER,showData);
					_dot.addEventListener(MouseEvent.MOUSE_OUT,showData);
					_dot.mouseChildren=false;
					
				}
				
					
				
				//当data值大于2时，显示日期，否则不显示
				if(k>=2)
				{
					addChild(_day);
				}
				_day.x=startRowX+(endX-startDataX)/6+i*(endX-startDataX)/3;
				_day.y=startY+4;//+betweenY/2;
				
			}
			//添加_pic和显示时间数据文字
			addChild(_pic);
			
		}
		private function showWeather(e:MouseEvent):void
		{
			
			//getChildByName(DisplayObject(e.target)
			if(e.type=="mouseOver")
			{
				trace(e.target);
				//trace("e.targete.targete.target"+getChildIndex(e.target));
			}
			else if (e.type=="mouseOut")
			{
				
			}
		}
		
		private function showData(e:MouseEvent):void
		{
			
			var i=_pic.getChildIndex(DisplayObject(e.target));
			_dataInfo.text=picArr[i];
			_dataInfo.mouseEnabled=false;
			
			if(e.type=="mouseOver")
			{
				addChild(_dataInfo);
				//_dataInfo.visible=true;
				_dataInfo.x=e.target.x;
				_dataInfo.y=e.target.y-20;
			}
			else if (e.type=="mouseOut")
			{
				//_dataInfo.visible=false;
				removeChild(_dataInfo);
				//removeChild(_dataInfo);
			}
		}
	}
}