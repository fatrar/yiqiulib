/**H**************************************************************************
 File            : ViewHelper.h
 Subsystem       : 
 Function Name(s): CViewHelper
 Author          : YiQiu
 Date            : 2010-4-2  
 Time            : 13:39
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _VIEWHELPER_H_2010_4
#define _VIEWHELPER_H_2010_4
#pragma once







namespace ViewHelper
{
   inline void TranslateToRect(
        const RECT ClientRect,
        const WPG_RectangleF& WPG_Rect,
        int& left, int& top, int& right, int& bottom)
   {     
       int nWidth = ClientRect.right-ClientRect.left;
       int nHeight = ClientRect.bottom-ClientRect.top;
       left = int(nWidth*WPG_Rect.x) + ClientRect.left;
       top = int(nHeight*WPG_Rect.y) + ClientRect.top;
       right = int(nWidth*(WPG_Rect.x+WPG_Rect.width)) + ClientRect.left;
       bottom = int(nHeight*(WPG_Rect.y+WPG_Rect.height)) + ClientRect.top;
   }

   inline void TranslateWPGPoint(
       const RECT ClientRect,
       const WPG_PointF& p, 
       int& x, int& y)
   {
       int nWidth = ClientRect.right-ClientRect.left;
       int nHeight = ClientRect.bottom-ClientRect.top;
       x = int(nWidth*p.x) + ClientRect.left;
       y = int(nHeight*p.y) + ClientRect.top;
   }

    /**
    @note 将画出窗口的目标过滤
    */
    inline bool IsVaildWPGTarget(const WPG_Target& Data)
    {
        const WPG_RectangleF& Rect = Data.boundingBox;
        if ( (Rect.height+Rect.y) > 1 ||
            (Rect.width+Rect.x) > 1 ||
            Rect.x > 1 ||
            Rect.y > 1 )
        {
            return false;
        }
        return true;
    }
};



#endif  // _VIEWHELPER_H_2010_


// End of file


