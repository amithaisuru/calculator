
// CalculatorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Calculator.h"
#include "CalculatorDlg.h"
#include "afxdialogex.h"
#include "math.h"

#include<iostream>
#include<stack>
#include<string>
using namespace std;

void printStack(stack<string>);
bool isOperator(char);
int prec(string c);
void infixToPostfix(stack<string>);
float operation(float, float, string);
float getRemainder(float a, float b);
float evaluate(stack<string>);

//global stack
stack<string> globalStack;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCalculatorDlg dialog



CCalculatorDlg::CCalculatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CALCULATOR_DIALOG, pParent)
	, btn_1(0)
	, btn_2(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	/*DDX_Control(pDX, IDC_BTN_1, btn_1);
	DDX_Control(pDX, IDC_BTN_2, btn_2);*/
	DDX_Control(pDX, IDC_MAINTXT, mainTxtVar);
}

BEGIN_MESSAGE_MAP(CCalculatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_MAINTXT, &CCalculatorDlg::OnEnChangeMaintxt)
	//number pad buttons
	ON_BN_CLICKED(IDC_BTN_0, &CCalculatorDlg::OnBnClickedBtn0)
	ON_BN_CLICKED(IDC_BTN_1, &CCalculatorDlg::OnBnClickedBtn1)
	ON_BN_CLICKED(IDC_BTN_2, &CCalculatorDlg::OnBnClickedBtn2)
	ON_BN_CLICKED(IDC_BTN_3, &CCalculatorDlg::OnBnClickedBtn3)
	ON_BN_CLICKED(IDC_BTN_4, &CCalculatorDlg::OnBnClickedBtn4)
	ON_BN_CLICKED(IDC_BTN_5, &CCalculatorDlg::OnBnClickedBtn5)
	ON_BN_CLICKED(IDC_BTN_6, &CCalculatorDlg::OnBnClickedBtn6)
	ON_BN_CLICKED(IDC_BTN_7, &CCalculatorDlg::OnBnClickedBtn7)
	ON_BN_CLICKED(IDC_BTN_8, &CCalculatorDlg::OnBnClickedBtn8)
	ON_BN_CLICKED(IDC_BTN_9, &CCalculatorDlg::OnBnClickedBtn9)
	//operation buttons
	ON_BN_CLICKED(IDC_BTN_ADD, &CCalculatorDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_SUB, &CCalculatorDlg::OnBnClickedBtnSub)
	ON_BN_CLICKED(IDC_BTN_MUL, &CCalculatorDlg::OnBnClickedBtnMul)
	ON_BN_CLICKED(IDC_BTN_DIV, &CCalculatorDlg::OnBnClickedBtnDiv)
	ON_BN_CLICKED(IDC_BTN_POWER, &CCalculatorDlg::OnBnClickedBtnPower)
	ON_BN_CLICKED(IDC_BTN_REM, &CCalculatorDlg::OnBnClickedBtnRem)
	ON_BN_CLICKED(IDC_BTN_CLR, &CCalculatorDlg::OnBnClickedBtnClr)
	ON_BN_CLICKED(IDC_BTN_DEL, &CCalculatorDlg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_EQUAL, &CCalculatorDlg::OnBnClickedBtnEqual)

	ON_BN_CLICKED(IDC_MFCMENUBUTTON1, &CCalculatorDlg::OnBnClickedMfcmenubutton1)
END_MESSAGE_MAP()


// CCalculatorDlg message handlers

BOOL CCalculatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCalculatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCalculatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCalculatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCalculatorDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here


}



void CCalculatorDlg::OnEnChangeMaintxt()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

}


void CCalculatorDlg::OnBnClickedBtn0(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"0");
}

void CCalculatorDlg::OnBnClickedBtn1(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"1");
}

void CCalculatorDlg::OnBnClickedBtn2(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"2");
}

void CCalculatorDlg::OnBnClickedBtn3(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"3");
}

void CCalculatorDlg::OnBnClickedBtn4(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"4");

}

void CCalculatorDlg::OnBnClickedBtn5(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"5");
}

void CCalculatorDlg::OnBnClickedBtn6(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"6");
}

void CCalculatorDlg::OnBnClickedBtn7(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"7");
}

void CCalculatorDlg::OnBnClickedBtn8(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"8");
}

void CCalculatorDlg::OnBnClickedBtn9(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"9");
}

void CCalculatorDlg::OnBnClickedBtnAdd(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"+");
}

void CCalculatorDlg::OnBnClickedBtnSub(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"-");
}

void CCalculatorDlg::OnBnClickedBtnMul(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"x");
}

void CCalculatorDlg::OnBnClickedBtnDiv(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"/");
}

void CCalculatorDlg::OnBnClickedBtnPower(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"^");
}

void CCalculatorDlg::OnBnClickedBtnRem(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end);
	mainTxtVar.ReplaceSel(L"%");
}

//clera all characters
void CCalculatorDlg::OnBnClickedBtnClr(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(0,- 1);
	mainTxtVar.Clear();
}

//delete last character
void CCalculatorDlg::OnBnClickedBtnDel(){
	int end = mainTxtVar.GetWindowTextLength();
	mainTxtVar.SetSel(end, end-1);
	mainTxtVar.Clear();
}

void CCalculatorDlg::OnBnClickedBtnEqual()//equal sign
{
	UpdateData();
	CString inputString;
	mainTxtVar.GetWindowText(inputString);
	CT2A ct(inputString);
	string text(ct);
	text = '0' + text;

	stack<string> stack1;
	stack<string> stackReverse;
	stack<string> stackPostfix;

	int digit = 0;
	int count = 0;

	//inser input in to stack
	for (int i = 0;i < text.length();i++) {
		if (isOperator(text.at(i))) {
			stack1.push(text.substr(digit, count));
			stack1.push(text.substr(i, 1));
			digit = i + 1;
			count = -1;
		}
		if (i == text.length() - 1) {
			stack1.push(text.substr(digit, count + 1));
		}
		count++;
	}

	//make inputstack reverse
	while (!stack1.empty()) {
		stackReverse.push(stack1.top());
		stack1.pop();
	}

	infixToPostfix(stackReverse);

	string ans = to_string((evaluate(globalStack)));
	CString output(ans.c_str(), ans.length());
	SetDlgItemText(IDC_MAINTXT, output);

	while (!globalStack.empty())
		globalStack.pop();
}

bool isOperator(char character) {

	if (character == '+' || character == '-' || character == '/' || character == 'x' || character == '^' || character == '%' || character == '(' || character == ')')
		return true;
	return false;
}

// Function to return precedence of operators
int prec(string c)
{
	if (c == "^")
		return 3;
	else if (c == "/" || c == "x" || c == "%")
		return 2;
	else if (c == "+" || c == "-")
		return 1;
	else
		return -1;
}

// to postfix expression
void infixToPostfix(stack<string> infixStack)
{
	stack<string> st;
	stack<string> postfixStack;
	string result;
	int count = infixStack.size();
	for (int i = 0; i < count; i++) {
		string c = infixStack.top();

		if (c == "(") {
			st.push("(");
			infixStack.pop();
		}

		// If the scanned character is an ‘)’,
		// pop and to output string from the stack
		// until an ‘(‘ is encountered.
		else if (c == ")") {
			while (st.top() != "(") {
				postfixStack.push(st.top());
				st.pop();
			}
			st.pop();
			infixStack.pop();
		}

		// If an operator is scanned
		else if (infixStack.top() == "+" || infixStack.top() == "-" || infixStack.top() == "/" || infixStack.top() == "x" || infixStack.top() == "^" || infixStack.top() == "*" || infixStack.top() == "%") {
			while (!st.empty() && prec(infixStack.top()) <= prec(st.top())) {
				postfixStack.push(st.top());
				st.pop();
			}
			st.push(c);
			infixStack.pop();
		}

		else {
			postfixStack.push(infixStack.top());
			infixStack.pop();
		}

	}

	// Pop all the remaining elements from the stack
	while (!st.empty()) {
		postfixStack.push(st.top());
		st.pop();
	}

	//assignt created stack to global stack
	while (!postfixStack.empty()) {
		globalStack.push(postfixStack.top());
		postfixStack.pop();
	}
}

float operation(float a, float b, string op) {

	if (op == "+")
		return b + a;
	else if (op == "-")
		return b - a;
	else if (op == "x")
		return b * a;
	else if (op == "/")
		return b / a;
	else if (op == "^")
		return pow(b, a);
	else if (op == "%")
		return getRemainder(b, a);
	else
		return NULL;
}

float getRemainder(float a, float b)
{
	double mod;
	// Handling negative values
	if (a < 0)
		mod = -a;
	else
		mod = a;
	if (b < 0)
		b = -b;

	// Finding mod by repeated subtraction

	while (mod >= b)
		mod = mod - b;

	// Sign of result typically depends
	// on sign of a.
	if (a < 0)
		return -mod;

	return mod;
}

float evaluate(stack<string> postfixStack) {

	float a, b, ans;
	stack<float> stk;
	while (!postfixStack.empty()) {
		//read elements and perform postfix evaluation
		if (isOperator(postfixStack.top().at(0))) {
			a = stk.top();
			stk.pop();
			b = stk.top();
			stk.pop();
			ans = operation(a, b, postfixStack.top());
			stk.push(ans);
		}
		else {
			stk.push(stof(postfixStack.top()));
		}
		postfixStack.pop();
	}
	return stk.top();
}

void CCalculatorDlg::OnBnClickedMfcmenubutton1()
{
	MessageBox(_T("1.MADUSHANKA R.K.A.I.\t20/ENG/084\n2.INDUMINA M.A\t\t20/ENG/185\n3.JAYASURIYA K.A.K.D.J.P.\t20/ENG/062\n"));
}
