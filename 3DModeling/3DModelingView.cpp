
// 3DModelingView.cpp : CMy3DModelingView 类的实现
//the left buttom control the y axis and z axis

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "3DModeling.h"
#endif

#include "3DModelingDoc.h"
#include "3DModelingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//#define totalNum 80.0
#define exp 3//贝塞尔曲线的次数
#define circleR 0.2
GLfloat LightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; 				// 环境光参数
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };					// 漫射光参数
GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };					//镜面反射光
GLfloat LightPosition[] = { 7.0f, 8.0f,8.0f, 1.0f };				 // 光源位置
GLfloat am[]={0.8f,0.8f,0.8f,1.0f};//材质的环境光颜色
GLfloat di[]={0.0f,0.0f,0.8f,1.0f};//材质的漫反射光颜色
GLfloat sp[]={1.0f,1.0f,1.0f,1.0f};//材质的镜面反射光颜色
GLfloat shininess = 80.0; //镜面指数越大，高光区越小,shininess为0时基本只能看到物体镜面反射光了
// CMy3DModelingView

IMPLEMENT_DYNCREATE(CMy3DModelingView, CView)

BEGIN_MESSAGE_MAP(CMy3DModelingView, CView)
	ON_WM_CREATE()
	ON_COMMAND(ID_32771, &CMy3DModelingView::OnBèzier)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_32772, &CMy3DModelingView::OnselectTeapotPoint)
	ON_COMMAND(ID_32773, &CMy3DModelingView::OnDrawTeapot)
	
	ON_COMMAND(ID_32780, &CMy3DModelingView::OnSelectHandler)
	ON_COMMAND(ID_32781, &CMy3DModelingView::OnDrawHandler)
	ON_COMMAND(ID_32782, &CMy3DModelingView::OnSelectSpout)
	ON_COMMAND(ID_32783, &CMy3DModelingView::OnDrawSpout)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_32786, &CMy3DModelingView::OnRotate)
	ON_COMMAND(ID_32787, &CMy3DModelingView::OnDisableRotate)
	ON_COMMAND(ID_32788, &CMy3DModelingView::OnOpenModel)
	ON_COMMAND(ID_32789, &CMy3DModelingView::OnSaveModel)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_32791, &CMy3DModelingView::OnStartSlide)
	ON_WM_TIMER()
	ON_COMMAND(ID_32792, &CMy3DModelingView::OnStopFlip)
	ON_COMMAND(ID_32793, &CMy3DModelingView::OnFinalTeapot)
END_MESSAGE_MAP()

// CMy3DModelingView 构造/析构

CMy3DModelingView::CMy3DModelingView()
{
	// TODO: 在此处添加构造代码
	sceneMode = showNothing;
	selMode = noSelectMode;
	
	currentP.reserve(100);
	handler.reserve(100);
	spout.reserve(100);
	xAxis = xh = 0.0;
	yAxis = yh = 0.0;
	X= Y= Z = 0.0; 
	angle = 0.0;
}

CMy3DModelingView::~CMy3DModelingView()
{
	delete m_pDC;
}

BOOL CMy3DModelingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMy3DModelingView 绘制

void CMy3DModelingView::OnDraw(CDC* /*pDC*/)
{
	CMy3DModelingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//模型视图矩阵的设置
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	this->InitLight();
	gluLookAt(0.0,0.0,10.0,0.0,0.0,0.0,0.0,1.0,0.0);//等价于模型视图变换，所以注意防止越界
	
	if(this->selMode == dragTeapot)
	{	
		glLoadIdentity();
		glPushMatrix();		
		glRotatef(xAxis,0.0,1.0,0.0);
		glRotatef(yAxis,0.0,0.0,-1.0);
	}
	switch(this->sceneMode)
	{
	case showTeapotEveryPoint:
		glDisable(GL_DEPTH_TEST);
		this->DrawEveryPoint(this->currentP);
		break;
	case showHandlerPoint:
		glDisable(GL_DEPTH_TEST);
		this->DrawCurvedLine();
		this->DrawEveryPoint(this->handler);
		break;
	case showSpoutPoint:
		glDisable(GL_DEPTH_TEST);
		this->DrawCurvedLine();
		this->DrawEveryPoint(this->spout);
		break;
	case showCurvedLine:
		glDisable(GL_DEPTH_TEST);
		this->DrawCurvedLine();
		break;
	case showTeapot:
		glEnable(GL_DEPTH_TEST);
		this->drawTeapot();
		break;
	case showHandler:
		glEnable(GL_DEPTH_TEST);
		this->drawHandler();
		break;
	case showSpout:
		glEnable(GL_DEPTH_TEST);
		this->drawSpout();
		break;
	case showFinalTeapot:
		glEnable(GL_DEPTH_TEST);
		this->drawTeapot();
		this->drawHandler();
		this->drawSpout();
		break;
	default:
		break;
	}
	if(this->selMode == dragTeapot)
	{
		glPopMatrix();
	}
	SwapBuffers( m_pDC->GetSafeHdc());
	// TODO: 在此处为本机数据添加绘制代码
}


// CMy3DModelingView 诊断

#ifdef _DEBUG
void CMy3DModelingView::AssertValid() const
{
	CView::AssertValid();
}

void CMy3DModelingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy3DModelingDoc* CMy3DModelingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy3DModelingDoc)));
	return (CMy3DModelingDoc*)m_pDocument;
}
#endif //_DEBUG

/*MFC中OpenGL环境的搭建
*OpengGLInit()
*SetWindowPixelFormat()
*重写OnCreate(LPCREATESTRUCT lpCreateStruct)
*/
bool CMy3DModelingView::SetWindowPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd = 
	{
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
        PFD_SUPPORT_OPENGL |		    // support OpenGL
        PFD_DOUBLEBUFFER,				// double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        32,                             // 32-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,0, 0, 0, 0,                         // no accumulation buffer                 
        32,                             // 32-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // 保留结构数
        0, 0, 0                         //不支持结构数
    };
    
    int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	if ( m_nPixelFormat == 0 )
	{
		m_nPixelFormat = 1;
        if(DescribePixelFormat(m_pDC->GetSafeHdc(),m_nPixelFormat,sizeof(PIXELFORMATDESCRIPTOR),&pfd)==0)
        {
            return FALSE;
        }
	}

    if ( ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
    {
       return FALSE;
	}
    return TRUE;
}


BOOL CMy3DModelingView::OpenGLInit()
{
	//Get a DC for the Client Area
	m_pDC = new CClientDC(this);
	//Failure to Get DC
	if(m_pDC == NULL)
	{
		MessageBox("Error Obtaining DC");
		return FALSE;
	}
	//Failure to set the pixel format
	if(!SetWindowPixelFormat())
	{
		return FALSE;
	}
	//Create Rendering Context
	m_hGLContext = ::wglCreateContext (m_pDC->GetSafeHdc ());
	//Failure to Create Rendering Context
	if(m_hGLContext == 0)
	{
		MessageBox("Error Creating RC");
		return FALSE;
	}
	//Make the RC Current
	if(::wglMakeCurrent (m_pDC->GetSafeHdc (), m_hGLContext)==FALSE)
	{
		MessageBox("Error making RC Current");
		return FALSE;
	}
	glViewport(0,0,700,700);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0,10.0,-10.0,10.0, -10.0, 10.0);
	return TRUE;
}


int CMy3DModelingView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	this->OpenGLInit();
	
	return 0;
}

/*
*以下是茶壶的绘制 
*主要分为三个部分，壶身（壶盖），壶柄，壶嘴
*/

//辅助数学计算
float CMy3DModelingView::BinomialCoeffs(const int n,const int k)
{
	if(k!=0&&k!=n)
	{
		float a=1.0;
		for(int i=k+1;i!=n+1;i++)
		{
			a=a*i;
		}
		float b = n-k;
		int m = n-k;
		while(m > 1)
		{
			b = b*(m-1);
			m--;
		}
		return a/b;
	}
	else
	{
		return 1.0;
	}
}

//光照初始化
void CMy3DModelingView::InitLight()
{
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);//定义光源
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,am);//定义材质，否则全灰色
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,di);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,sp);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&shininess);//镜面指数越大，高光区越小,shininess为0时基本只能看到物体镜面反射光了

	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE); 
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
}

//绘制每一个响应鼠标点击的点
void CMy3DModelingView::DrawEveryPoint(vector<Mypoint> P)
{
	glColor3f(0.0,0.0,0.0);
	glPointSize(6.0);
	glBegin(GL_POINTS);
	for(int i=0;i!=P.size();i++)
	{
		glVertex3f(P[i].x,P[i].y,P[i].z);
	}
	glEnd();
	glFinish();
}

//num+1个点一组绘制bezier曲线
vector<Mypoint> CMy3DModelingView::MyBezier(vector <Mypoint> p,int startPos,int num,int totalNum)//num是多项式次数
{
	vector<Mypoint> re;
	glColor3f(0.0,0.0,1.0);
	glLineWidth(2.0); 
	glBegin(GL_LINE_STRIP);
	float lastxi,lastyi,lastzi;
	for(int i=0;i<=totalNum;i++)
	{
		float xi=0,yi=0,zi=0;
		for(int j=0;j!=num+1;j++)
		{
			xi+=p[startPos+j].x*BinomialCoeffs(num,j)*pow((float)i/totalNum,j)*pow(1-(float)i/totalNum,num-j);
			yi+=p[startPos+j].y*BinomialCoeffs(num,j)*pow((float)i/totalNum,j)*pow(1-(float)i/totalNum,num-j);
			zi+=p[startPos+j].z*BinomialCoeffs(num,j)*pow((float)i/totalNum,j)*pow(1-(float)i/totalNum,num-j);
		}
		if(i>0)//法向量的设置
		{
			if(p[0].x>=0)
				glNormal3f(lastyi-yi,xi-lastxi, 0.0f);
			else
				glNormal3f(yi-lastyi,lastxi-xi,0.0f);
		}
		glVertex3f(xi,yi,zi);
		Mypoint te;
		te.x = xi;
		te.y = yi;
		te.z = zi;
		re.push_back(te);
		lastxi = xi;
		lastyi = yi;
		lastzi = zi;
	}
	glEnd();
	return re;
}

//四个点一组绘制壶身bezier曲线，满足C1连续
void CMy3DModelingView::DrawCurvedLine()
{
	int k = 0;
	int tempPointNum = currentP.size();
	while(tempPointNum > exp)
	{	
		this->MyBezier(currentP,k*exp,3,100);
		tempPointNum -= exp;
		k++;
	}
	if(tempPointNum == 3)
		this->MyBezier(currentP,k*exp,2,100);
	if(tempPointNum == 2)
		this->MyBezier(currentP,k*exp,1,100);
	//显示控制顶点
	if(this->sceneMode!= showTeapot&&this->sceneMode!=showHandlerPoint 
		&&this->sceneMode!=showHandler&&this->sceneMode!=showSpout&&
		this->sceneMode!=showSpoutPoint&&this->sceneMode!=showFinalTeapot)
	{
		glColor3f(0.0,0.0,0.0);
		glPointSize(5.0);
		glBegin(GL_POINTS);
		for(int i=0;i!=this->currentP.size();i++)
		{
			glVertex3f(currentP[i].x,currentP[i].y,currentP[i].z);
		}
		glEnd();
	}
    glFlush();
}

//旋转壶身曲线绘制3D壶身
void CMy3DModelingView::drawTeapot()
{ 
	float segma = 0.0f; 
	while(segma<360.0)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glRotatef(segma,0.0,1.0,0.0);
		DrawCurvedLine();
		glPopMatrix();
		segma += 0.4;
	}
	//画出壶盖
	glPushMatrix();
	glTranslatef(0.0,currentP[0].y,0.0);
	glTranslatef(X,Y,Z);//控制壶盖掉落
	glRotatef(angle,0.0,0.0,-1.0);
	glRotatef(-90.0,1.0,0.0,0.0);
	
	glColor3f(0.0,1.0,0.0);
	GLUquadricObj *g_text = gluNewQuadric(); 
	gluCylinder(g_text,abs(currentP[0].x),0.0f,1.0f,32.0f,16.0f);
	glPopMatrix();
}

//按照壶柄轨迹利用一个圆平移绘制壶柄
void CMy3DModelingView::drawHandler()
{
	int totalNum = 200;
	vector<Mypoint> vec;
	int startPos = 0;
	int num = handler.size()-1;
	for(int i=0;i<=totalNum;i++)
	{
		float xi=0,yi=0,zi=0;
		for(int j=0;j!=num+1;j++)
		{
			xi+=handler[startPos+j].x*BinomialCoeffs(num,j)*pow((float)i/totalNum,j)*pow(1-(float)i/totalNum,num-j);
			yi+=handler[startPos+j].y*BinomialCoeffs(num,j)*pow((float)i/totalNum,j)*pow(1-(float)i/totalNum,num-j);
			zi+=handler[startPos+j].z*BinomialCoeffs(num,j)*pow((float)i/totalNum,j)*pow(1-(float)i/totalNum,num-j);
		}
		Mypoint te;
		te.x = xi;
		te.y = yi;
		te.z = zi;
		vec.push_back(te);
	}
	glColor3f(0.0,0.0,1.0);
	glLineWidth(1.5);

	float ar = 360.0/(2.0*3.14);
	for(int i=0;i!=vec.size();i++)
	{
		glPushMatrix();
		if(i>0)
		{
			glTranslatef(vec[i].x,vec[i].y,vec[i].z);
			glRotatef(-ar*atan((vec[i].x-vec[i-1].x)/(vec[i].y-vec[i-1].y)),0.0,0.0,1.0);
		}
		else
		{
			glTranslatef(vec[0].x,vec[0].y,vec[0].z);
			glRotatef(ar*atan(abs((vec[i].x-vec[i+1].x)/(vec[i].y-vec[i+1].y))),0.0,0.0,1.0);
		}
		glBegin(GL_LINE_STRIP);
		for(int j=0;j!=100;j++)
		{
			float a=circleR*cos((float)j/100.0*2.0*3.14);
			float b=0.0;
			float c=circleR*sin((float)j/100.0*2.0*3.14);
			glNormal3f(a,b,c);
			glVertex3f(a,b,c);
		}
		glEnd();
		glPopMatrix();
	}
}

//同上道理绘制壶嘴
void CMy3DModelingView::drawSpout()
{
	int totalNum = 200;
	vector<Mypoint> vec;
	int startPos = 0;
	int num = spout.size()-1;
	for(int i=0;i<=totalNum;i++)
	{
		float xi=0,yi=0,zi=0;
		for(int j=0;j!=num+1;j++)
		{
			xi+=spout[startPos+j].x*BinomialCoeffs(num,j)*pow((float)i/totalNum,j)*pow(1-(float)i/totalNum,num-j);
			yi+=spout[startPos+j].y*BinomialCoeffs(num,j)*pow((float)i/totalNum,j)*pow(1-(float)i/totalNum,num-j);
			zi+=spout[startPos+j].z*BinomialCoeffs(num,j)*pow((float)i/totalNum,j)*pow(1-(float)i/totalNum,num-j);
		}
		Mypoint te;
		te.x = xi;
		te.y = yi;
		te.z = zi;
		vec.push_back(te);
	}
	glColor3f(0.0,0.0,1.0);
	glLineWidth(1.5);

	float ar = 360.0/(2.0*3.14);
	for(int i=0;i!=vec.size();i++)
	{
		glPushMatrix();
		if(i>0)
		{
			glTranslatef(vec[i].x,vec[i].y,vec[i].z);
			glRotatef(-ar*atan((vec[i].x-vec[i-1].x)/(vec[i].y-vec[i-1].y)),0.0,0.0,1.0);
		}
		else
		{
			glTranslatef(vec[0].x,vec[0].y,vec[0].z);
			glRotatef(ar*atan(abs((vec[i].x-vec[i+1].x)/(vec[i].y-vec[i+1].y))),0.0,0.0,1.0);
		}
		glBegin(GL_LINE_STRIP);
		for(int j=0;j!=100;j++)
		{
			float a=circleR*cos((float)j/100.0*2.0*3.14);
			float b=0.0;
			float c=circleR*sin((float)j/100.0*2.0*3.14);
			glNormal3f(a,b,c);
			glVertex3f(a,b,c);
		}
		glEnd();
		glPopMatrix();
	}
}

//左键处理
void CMy3DModelingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnLButtonDown(nFlags, point);
	Mypoint temp;
	CRect t;
	this->GetClientRect(t);

	switch(this->selMode)
	{
	case selectTeapotPoint:
		temp.x = ((float) point.x/(float)t.Width()*20.0)-10.0;
		temp.y = 10.0-((float) point.y/(float)t.Height()*20.0);
		temp.z = 0.0;
		if(currentP.size()>3&&currentP.size()%3 == 1)
		{
			Mypoint g1;
			int j = currentP.size();
			g1.x = 2*currentP[j-1].x-currentP[j-2].x;
			g1.y = 2*currentP[j-1].y-currentP[j-2].y;
			currentP.push_back(g1);
		}
		this->currentP.push_back(temp);
		this->sceneMode = showTeapotEveryPoint;
		Invalidate(false);
		PostMessage(WM_PAINT);
		break;
	case selectHandlerPoint:
		temp.x = ((float) point.x/(float)t.Width()*20.0)-10.0;
		temp.y = 10.0-((float) point.y/(float)t.Height()*20.0);
		temp.z = 0.0;
		if(handler.size()>3&&handler.size()%3 == 1)
		{
			Mypoint g1;
			int j = handler.size();
			g1.x = 2*handler[j-1].x-handler[j-2].x;
			g1.y = 2*handler[j-1].y-handler[j-2].y;
			handler.push_back(g1);
		}
		this->handler.push_back(temp);
		this->sceneMode = showHandlerPoint;
		Invalidate(false);
		PostMessage(WM_PAINT);
		break;
	case selectSpoutPoint:
		temp.x = ((float) point.x/(float)t.Width()*20.0)-10.0;
		temp.y = 10.0-((float) point.y/(float)t.Height()*20.0);
		temp.z = 0.0;
		if(spout.size()>3&&spout.size()%3 == 1)
		{
			Mypoint g1;
			int j = spout.size();
			g1.x = 2*spout[j-1].x-spout[j-2].x;
			g1.y = 2*spout[j-1].y-spout[j-2].y;
			spout.push_back(g1);
		}
		this->spout.push_back(temp);
		this->sceneMode = showSpoutPoint;
		Invalidate(false);
		PostMessage(WM_PAINT);
		break;
	case dragTeapot:
		leftClick = point;
		break;
	default:
		break;
	}
}

//右键处理
void CMy3DModelingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnLButtonUp(nFlags, point);
	switch(this->selMode)
	{
		case dragTeapot:
			leftRelease = point;
			xh = (float)(leftRelease.x-leftClick.x)/700.0*360.0;
			yh = (float)(leftRelease.y-leftClick.y)/700.0*360.0;
			if(abs(xh)>abs(yh))
				xAxis += xh;
			else
				yAxis += yh;
			Invalidate();
			//PostMessage(WM_PAINT);
			break;
		default:
			break;
	}
}

//选择壶身的控制点，点数任意
void CMy3DModelingView::OnselectTeapotPoint()
{
	// TODO: 在此添加命令处理程序代码
	this->OnStopFlip();
	this->selMode = selectTeapotPoint;
	this->sceneMode = showNothing;
	
	this->currentP.clear();
	this->handler.clear();
	this->spout.clear();
	glDisable(GL_LIGHTING);
	Invalidate(TRUE);
	PostMessage(WM_PAINT);
}

//画出壶身的bezier曲线
void CMy3DModelingView::OnBèzier()
{
	// TODO: 在此添加命令处理程序代码  
	if(currentP.size() > 0)
	{
		this->sceneMode = showCurvedLine;
		this->selMode = noSelectMode;
		Invalidate(TRUE);
		PostMessage(WM_PAINT);
	}
	else
		::AfxMessageBox("你需要先选择点");
}


void CMy3DModelingView::OnDrawTeapot()
{
	// TODO: 在此添加命令处理程序代码
	this->sceneMode = showTeapot;
	this->selMode = noSelectMode;
	Invalidate(TRUE);
	PostMessage(WM_PAINT);
}


void CMy3DModelingView::OnSelectHandler()
{
	// TODO: 在此添加命令处理程序代码
	for(int i=0;i!=currentP.size();i++)
	{
		currentP[i].x =abs(currentP[i].x);
	}
	this->selMode = selectHandlerPoint;
	this->sceneMode = showCurvedLine;
	this->handler.clear();
	Invalidate(FALSE);
	PostMessage(WM_PAINT);
}


void CMy3DModelingView::OnDrawHandler()
{
	// TODO: 在此添加命令处理程序代码
	this->selMode = noSelectMode;
	this->sceneMode = showHandler;
	Invalidate(TRUE);
	PostMessage(WM_PAINT);
}


void CMy3DModelingView::OnSelectSpout()
{
	// TODO: 在此添加命令处理程序代码
	this->selMode = selectSpoutPoint;
	for(int i=0;i!=currentP.size();i++)
	{
		currentP[i].x=-abs(currentP[i].x);
	}
	this->sceneMode = showCurvedLine;
	this->spout.clear();
	Invalidate(FALSE);
	PostMessage(WM_PAINT);

}


void CMy3DModelingView::OnDrawSpout()
{
	// TODO: 在此添加命令处理程序代码
	this->selMode = noSelectMode;
	this->sceneMode = showSpout;
	Invalidate(TRUE);
	PostMessage(WM_PAINT);
}

//允许鼠标控制茶壶的角度
void CMy3DModelingView::OnRotate()
{
	// TODO: 在此添加命令处理程序代码
	this->selMode = dragTeapot;
}

//禁止鼠标控制茶壶的角度
void CMy3DModelingView::OnDisableRotate()
{
	// TODO: 在此添加命令处理程序代码
	this->selMode = noSelectMode;
}

//打开一个模型 文本文件
void CMy3DModelingView::OnOpenModel()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"模型文件(*.txt)|*.txt||");
	if(dlg.DoModal()==IDOK)
	{
		ifstream open(dlg.GetPathName());
		currentP.clear();
		handler.clear();
		spout.clear();
		string str;
		vector<Mypoint> *p = new vector<Mypoint>;
		while(getline(open,str))
		{
			if(str == "teapot")
			{
				p=&currentP;
			}
			else
			{
				if(str == "handler")
					p=&handler;
				else
				{
					if(str == "spout")
						p=&spout;
					else
					{
						Mypoint t;
						vector<float>vec;
						char* k= strtok(const_cast<char*>(str.c_str())," ");
						
						while(k !=NULL)
						{
							vec.push_back(atof(k));
							k = strtok(NULL," ");
						}
						t.x = vec[0];
						t.y = vec[1];
						t.z = vec[2];
						p->push_back(t);
					}
				}
			}
		}//读取完毕
		//强制绘图
		this->sceneMode = showFinalTeapot;
		Invalidate(false);
		PostMessage(WM_PAINT);
	}
}

//保存当前模型
void CMy3DModelingView::OnSaveModel()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"模型文件(*.txt)|*.txt||");
	if(dlg.DoModal()==IDOK)
	{
		ofstream open(dlg.GetPathName()+".txt");
		if(!open)
		{
			cerr<<"保存失败!"<<endl;
		}
		else
		{
			open<<"teapot"<<endl;
			for(int i=0;i!=currentP.size();i++)
			{
				open<<currentP[i].x<<" "<<currentP[i].y<<" "<<currentP[i].z<<endl;
			}
			open<<"handler"<<endl;
			for(int i=0;i!=handler.size();i++)
			{
				open<<handler[i].x<<" "<<handler[i].y<<" "<<handler[i].z<<endl;
			}
			open<<"spout"<<endl;
			for(int i=0;i!=spout.size();i++)
			{
				open<<spout[i].x<<" "<<spout[i].y<<" "<<spout[i].z<<endl;
			}
		}
	}
}


BOOL CMy3DModelingView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

//让壶盖从当前角度开始掉落
void CMy3DModelingView::OnStartSlide()
{
	// TODO: 在此添加命令处理程序代码
	if(currentP.size()>0)
	{
		if(abs(yAxis)>0)
			this->SetTimer(1,1000,NULL);
	}
	else
	{
		::AfxMessageBox("你必须先画出茶壶");
	}
}

//设置定时器，模拟壶盖掉落
void CMy3DModelingView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == 1)//确定是控制壶盖掉落的定时器
	{
		if(abs(yAxis)>=90)
		{
			X=X+sin(yAxis/180.0*3.14);
			Y=Y-cos(yAxis/180.0*3.14);
		}
		else
		{
			if(yAxis>0)
				X+=1.0;
			else
				X-=1.0;
			if(X>currentP[0].x)
			{
				angle+=20.0;
				Y-=0.3;
			}
		}
		if(X>10.0&&Y>10.0)
			this->OnStopFlip();
		this->selMode = dragTeapot;
		this->sceneMode = showFinalTeapot;
		PostMessage(WM_PAINT);
	}
	CView::OnTimer(nIDEvent);
}

//停止模拟，killtimer
void CMy3DModelingView::OnStopFlip()
{
	// TODO: 在此添加命令处理程序代码
	this->KillTimer(1);
	X=Y=Z=0.0;
	angle = 0.0;
	PostMessage(WM_PAINT);
}

//画出所有部件
void CMy3DModelingView::OnFinalTeapot()
{
	// TODO: 在此添加命令处理程序代码
	this->sceneMode = showFinalTeapot;
	this->selMode = noSelectMode;
	X = Y = Z = 0.0;
	angle = 0.0;
	xAxis = yAxis = 0.0;
	PostMessage(WM_PAINT);
}
