//画柱形图
package com.vanwins.chart.panel 
{
	import com.vanwins.chart.bean.*;
	import com.vanwins.chart.config.*;	
	
	import flash.net.URLRequest;
    import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.display.*;
	import flash.display.Shape
	import flash.display.MovieClip;
	
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.geom.ColorTransform;
	
	public class Column extends BasePanel 
	{
		//中间界面最左边的x值
		private var startRowX:int=20;
		//中间界面显示日期数据的左边的x值
		private var startDataX:int=79;
		//中间界面日期，时间等的y轴间隔值
		private var betweenY:int=29;
		//中间界面最上部的y值
		private var startY:int=49;
		//中间界面最下部的y值
		private var endY:int=326;
		//中间界面最右边的x值
		private var endX:int=735;
		//中间界面时间等的x轴间隔值
		private var betweenX:Number;
		//是否显示风标
		private var IsDisplayArrow:Boolean=false;
		//前一天有多少个时间数目
		private var lastDataX:Number=0;
		//之前天数总共有多少个时间数目
		private var allLastDataX:Number=0;
		
		
		public function Column()
		{
			
		}
		//进行构造面板
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
			_min.width=120;
			addChild(_min);
			
			//添加左下角的最大值
			var _max:Max=Wea.GetHead().alert.ismax;
			_max.paint();
			_max.x=140;
			_max.y=340;
			_max.width=120;
			addChild(_max);
			
			//添加下方的的不适合比赛阶段
			var Inapp=Wea.GetHead().alert.inappropriate;
			var _inappropriate:Inappropriate=Wea.GetHead().alert.inappropriate;
			_inappropriate.paint();
			_inappropriate.x=255;
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
			//valueDisplayNum为Wea.GetHead().rows.values[i].isdisplay的num-1数
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
				if(Wea.GetHead().rows.values[i].isvalue=="风  向" && Wea.GetHead().rows.values[i].isdisplay==1)
				{
					//判断是否添加风标
					IsDisplayArrow=true;
				}
			}
			//trace(valueDisplayNum);
			
			//添加最后一个的行背景和线条
			var _rowLast:Row=Wea.GetHead().rows.values[rowLen-1];
			_rowLast.paint();
			
			_rowLast.x=startDataX-20;
			_rowLast.y=startY+(rowLen-2)*betweenY+(endY-(rowLen)*betweenY)/2;
			//trace(_rowLast.text.length);
			_rowLast.width=400;
			addChild(_rowLast);			
			_rowLast.visible=Boolean(Wea.GetHead().rows.values[rowLen-1].isdisplay);
			
			//存放柱形容器
			//addChild(_pic);
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
			//k用来计每一天的data数
			var k:int=0;
			for(i=0;i<Wea.GetBody().days.length;i++)
			{
				//添加day的值，即日期一栏
				var _day:Day=Wea.GetBody().days[i];
				_day.paint();
				_day.mouseChildren=false;
				
				//添加swf
				var _loader:Loader= new Loader();
				addChild( _loader );
				for (j=0;j<app.GetWeathers().length;j++)
				{
					//trace(app.GetWeathers()[j].strname);
					if(app.GetWeathers()[j].strname==_day.weather)
					//trace(app.GetWeathers()[j].src);
					{
						 _loader.load( new URLRequest( app.GetWeathers()[j].src ) );
						 //添加一个遮罩，放在swf上面，右边有txt本文框，将对应的天气信息写入文本，可见度设为false
						 //cover为fla中的影片剪辑导出的类
						 var _cover:cover=new cover();
						 addChild(_cover);
						 _cover.txt.text=app.GetWeathers()[j].msg;
						 _cover.txt.visible=false;
						 _cover.addEventListener(MouseEvent.MOUSE_OVER,showWeather);
						 _cover.addEventListener(MouseEvent.MOUSE_OUT,showWeather);
					}
				}
				//_loader.x=startDataX+betweenX*k+betweenX/2;
				_loader.y=startY+betweenY-5;
				_loader.scaleX=0.6;
				_loader.scaleY=0.6;
				
				//遍历每一天中的data
				for(j=0;j<Wea.GetBody().days[i].datas.length;j++)
				{
					//添加时间一栏
					var _data:Data=Wea.GetBody().days[i].datas[j];
					_data.paint();
					addChild(_data);
					_data.mouseChildren=false;
					
					_data.x=startDataX+betweenX*k+betweenX/2-8;
					_data.y=startY+4+betweenY*2;
					k++;
					
					//当为每一天的最后一个data时，画竖线
					if(j==Wea.GetBody().days[i].datas.length-1)
					{
						var shape:Shape=new Shape();
						shape.graphics.lineStyle(1,0xFFFFFF);
						shape.graphics.moveTo(startDataX+betweenX*k,startY);
						shape.graphics.lineTo(startDataX+betweenX*k,startY+betweenY*valueDisplayNum);
						addChild(shape);
						lastDataX+=j+1;
					}
					
					//添加风标
					var _wind:arrow= new arrow();
					//var _coverWind:coverWind=new coverWind();
					trace(IsDisplayArrow,i,j);
					if (IsDisplayArrow)
					{
						addChild( _wind );
					}
					for (var m=0;m<app.GetWinds().length;m++)
					{
						trace(app.GetWinds()[m].strname);
						if(app.GetWinds()[m].strname==_data.val4)
						{
							trace("app.GetWinds()[m].strname"+app.GetWinds()[m].strname);
							_wind.rotation=app.GetWinds()[m].val;
							//添加一个遮罩，放在风标图上面，下边有txt本文框，将对应的风向信息写入文本，可见度设为false
							//coverWind为fla中的影片剪辑导出的类
							var _coverWind:coverWind=new coverWind();
							_coverWind.mouseChildren=false;
							_coverWind.txt.text=app.GetWinds()[m].msg;
						 	_coverWind.txt.visible=false;
							_coverWind.addEventListener(MouseEvent.MOUSE_OVER,showWind);
							_coverWind.addEventListener(MouseEvent.MOUSE_OUT,showWind);
							addChild(_coverWind);
							_coverWind.x=startDataX+betweenX*(k-1)+betweenX/2;
							_coverWind.y=startY+15+betweenY*3;
						}						
					}
					_wind.x=startDataX+betweenX*(k-1)+betweenX/2;
					_wind.y=startY+15+betweenY*3;
					
					
					//添加柱形
					var _rect:rect=new rect();
					//_pic.addChild(_rect);
					addChild(_rect);
					_rect.x=startDataX+betweenX*(k-1)+betweenX/2;
					_rect.y=endY;
					
					var tmp_x = (endY-startY-valueDisplayNum*betweenY)*(_data.isvalue-_min.isvalue+5);
					var tmp_y = (_max.isvalue-_min.isvalue+10);
					_rect.scaleY= tmp_x/tmp_y ;
					_rect.width=betweenX-8;
					
					//添加柱形提示信息
					var _dataInfo:TextField=new TextField();
					_dataInfo.autoSize = TextFieldAutoSize.CENTER;
					_dataInfo.text=String(_data.isvalue);
					_dataInfo.x=_rect.x-8;
					_dataInfo.y=endY-tmp_x/tmp_y-15;
					addChild(_dataInfo);
					//_dataInfo.mouseChildren=false;
					//_rect.addEventListener(MouseEvent.MOUSE_OVER,showData);
					//_rect.addEventListener(MouseEvent.MOUSE_OUT,showData);
										
					//如为不适合比赛时段，柱形和风标改变颜色
					var colorInfo:ColorTransform;
					colorInfo=_wind.transform.colorTransform;
					colorInfo=_rect.transform.colorTransform;
					if(_data.isvalue<Inapp.maxval && _data.isvalue>Inapp.minval)
					{
						//_wind.transform.colorTransform=0x000000;//Wea.GetBody().defaultcolor;						
						colorInfo.color=Wea.GetBody().defaultcolor;
					}
					else
					{
						
						colorInfo.color=Wea.GetBody().altercolor;
						_wind.transform.colorTransform=colorInfo;
					}
					
					_rect.transform.colorTransform=colorInfo;
					
				}
				
					
				
				//当data值大于2或者总共长度大于66时，显示日期，否则不显示
				if(k>=2||betweenX*k>66)
				{
					addChild(_day);
				}
				_day.x=startDataX+betweenX*allLastDataX+(lastDataX-allLastDataX)*betweenX/2-50;
				_loader.x=_day.x+30;
				_cover.x=_loader.x;
				_cover.y=_loader.y;
				_day.y=startY+4;
				trace(allLastDataX,lastDataX);
				allLastDataX+=lastDataX-allLastDataX;
			}
		}
		
		private function showWeather(e:MouseEvent):void
		{			
			//鼠标移上swf文件时显示天气信息
			if(e.type=="mouseOver")
			{
				e.target.txt.visible=true;
			}
			else if (e.type=="mouseOut")
			{
				e.target.txt.visible=false;
			}
		}		
		private function showWind(e:MouseEvent):void
		{
			//鼠标移上风标图像时显示风向信息
			if(e.type=="mouseOver")
			{
				e.target.txt.visible=true;
				//trace(e.target.txt.text);
			}
			else if (e.type=="mouseOut")
			{
				e.target.txt.visible=false;
			}
		}
	}
}