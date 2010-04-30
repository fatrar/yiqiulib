/*H***************************************************************************
 File            : DrawContainer.h
 Subsystem       : 
 Function Name(s): CDrawContainer
 Author          : YiQiu
 Date            : 2010-3-2  
 Time            : 16:39
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _DRAWCONTAINER_H_2010_3
#define _DRAWCONTAINER_H_2010_3

typedef BOOL (IDrawerEx::*DoMessgaeFn)(UINT, CPoint&);

class CDrawContainer : 
    public IDrawContainer
{
    DECLARE_DYNAMIC(CDrawContainer)
public:
    CDrawContainer();
    virtual ~CDrawContainer();

    //void SetSelectCursor(HWND Cursor);
    //void SetEditCursor();
    virtual void SetDrawModeNotify(IDrawModeNotify* pDrawModeNotify){m_pDrawModeNotify=pDrawModeNotify;};
    virtual void SetDrawMode(DrawMode mode){m_Mode=mode;};
    virtual void OnUseDraw(CDC& dc);

    virtual IDrawer* Add(IDrawerGraphType t);
    virtual BOOL Remove(IDrawer* pDrawer);
    virtual IDrawer* GetSelect(){return m_pSelect;}
    virtual void SwtichTool(IDrawer* pDrawer);

    virtual void EnableToolExcept(IDrawer* pDrawer, BOOL bEnbale);
    virtual void EnableToolAndNotEnableOther(IDrawer* pDrawer, BOOL bEnbale);

protected:
    DECLARE_MESSAGE_MAP()
protected:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();

   
    template<DoMessgaeFn T>
    inline void OnMouseMessage(UINT nFlags, CPoint& point);

protected:
    struct DrawerIter
    {
        DrawerIter(IDrawerEx* p);
        ~DrawerIter();

        IDrawerEx* pDrawer;
        //BOOL bEnbale;

        bool operator > (const DrawerIter& a) const;
        bool operator < (const DrawerIter& a) const;
        bool operator == (const DrawerIter& a) const;
    };
private:
    typedef set<DrawerIter> DrawerList;
    DrawerList m_GraphContainer;
    IDrawerEx* m_pSelect;
    IDrawModeNotify* m_pDrawModeNotify;
    DrawMode m_Mode;
};




#endif  // _DRAWCONTAINER_H_2010_


 // End of file



