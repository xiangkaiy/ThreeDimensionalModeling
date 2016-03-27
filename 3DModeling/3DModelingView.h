
// 3DModelingView.h : CMy3DModelingView 类的接口
//

#pragma once

struct Mypoint{
	float x;
	float y;
	float z;
	Mypoint():x(0.0),y(0.0),z(0.0){}
};

enum selectMode{
	noSelectMode = 0,
	selectTeapotPoint,
	selectHandlerPoint,
	selectSpoutPoint,
	dragTeapot//控制茶壶旋转
};

enum showMode{
	showNothing = 0,
	showTeapotEveryPoint,
	showHandlerPoint,
	showSpoutPoint,
	showCurvedLine,
	showTeapot,
	showHandler,
	showSpout,
	showFinalTeapot
};
class CMy3DModelingView : public CView
{
protected: // 仅从序列化创建
	CMy3DModelingView();
	DECLARE_DYNCREATE(CMy3DModelingView)

// 特性
public:
	CMy3DModelingDoc* GetDocument() const;

public:
	CDC*		m_pDC;
	HGLRC m_hGLContext;

	showMode sceneMode;
	selectMode selMode;
	
	vector <Mypoint> currentP;
	vector <Mypoint> handler;
	vector <Mypoint> spout;

	CPoint leftClick;
	CPoint leftRelease;
	float xAxis;
	float yAxis;
	float xh;
	float yh;
	float X;//控制壶盖掉落的三个分量
	float Y;
	float Z;
	float angle;
// 操作
public:
	bool SetWindowPixelFormat();
	BOOL OpenGLInit();//初始化
	float BinomialCoeffs(const int n,const int k);
	vector<Mypoint> MyBezier(vector <Mypoint> p,int startPos,int num,int totalNum);
	void DrawCurvedLine();
	void DrawEveryPoint(vector<Mypoint> P);
	void drawTeapot();
	void drawHandler();
	void drawSpout();
	void InitLight();
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CMy3DModelingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBèzier();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnselectTeapotPoint();
	afx_msg void OnDrawTeapot();
	afx_msg void OnSelectHandler();
	afx_msg void OnDrawHandler();
	afx_msg void OnSelectSpout();
	afx_msg void OnDrawSpout();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRotate();
	afx_msg void OnDisableRotate();
	afx_msg void OnOpenModel();
	afx_msg void OnSaveModel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnStartSlide();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStopFlip();
	afx_msg void OnFinalTeapot();
};

#ifndef _DEBUG  // 3DModelingView.cpp 中的调试版本
inline CMy3DModelingDoc* CMy3DModelingView::GetDocument() const
   { return reinterpret_cast<CMy3DModelingDoc*>(m_pDocument); }
#endif

