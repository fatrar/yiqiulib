package com.vanwins.chart.panel 
{
/*	import com.vanwins.chart.bean.Link;
	import com.vanwins.chart.bean.Min;
	import com.vanwins.chart.bean.Max;
	import com.vanwins.chart.bean.Inappropriate;
	import com.vanwins.chart.bean.Rows;
	import com.vanwins.chart.bean.Row;
	import com.vanwins.chart.bean.Day;*/
	import com.vanwins.chart.bean.*;
	import com.vanwins.chart.config.*;	
	
	import flash.net.URLRequest;
    import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.display.*;
	import flash.display.Shape
	import flash.display.MovieClip;
	
	import flash.text.TextField;
	
	import flash.geom.ColorTransform;
	
	public class Column extends BasePanel 
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
		private var IsDisplayArrow:Boolean=false;
		
		
		public function Column()
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
				if(Wea.GetHead().rows.values[i].isvalue=="风  向" && Wea.GetHead().rows.values[i].isdisplay==1)
				{
					//判断是否添加风标
					IsDisplayArrow=true;
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
					
					//添加风标
					var _wind:arrow= new arrow();
					if (IsDisplayArrow)
						addChild( _wind );
					for (var m=0;m<app.GetWinds().length;m++)
					{
						//trace(app.GetWinds()[m].strname);
						if(app.GetWinds()[m].strname==_data.val4)
						{
							_wind.rotation=app.GetWinds()[m].val;
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
					
					//trace(_data.isvalue);
					//trace("_data.isvalue");
					
					var tmp_x = (endY-startY-valueDisplayNum*betweenY)*(_data.isvalue-Inapp.minval+5);
					var tmp_y = (Inapp.maxval-Inapp.minval+10);
					_rect.scaleY= tmp_x/tmp_y ;//+ endY;
					_rect.width=betweenX-8;
					
					var _dataInfo:TextField=new TextField();
					_dataInfo.text=String(_data.isvalue);
					_dataInfo.x=_rect.x;
					_dataInfo.y=endY-tmp_x/tmp_y-15;
					addChild(_dataInfo);
					//_rect.addEventListener(MouseEvent.MOUSE_OVER,showData);
					//_rect.addEventListener(MouseEvent.MOUSE_OUT,showData);
					
					
					
					
					//改变颜色
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
				
					
				
				//当data值大于2时，显示日期，否则不显示
				if(k>=2)
				{
					addChild(_day);
				}
				_day.x=startRowX+(endX-startDataX)/6+i*(endX-startDataX)/3;
				_day.y=startY+4;//+betweenY/2;
				
			}

		}

		
	}
}