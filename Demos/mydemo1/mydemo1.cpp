/*
*       
*            demo1
*            最简单的一个duilib ui界面
* https://github.com/nmgwddj/duilib_tutorial/tree/master/02_%E5%88%B6%E4%BD%9C%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E7%95%8C%E9%9D%A2
* 
* 
* 
*/


#include "framework.h"
#include "mydemo1.h"

#include "..\..\DuiLib\UIlib.h"

#include <thread>
#include <string>

using namespace DuiLib;

class mywindow : public WindowImplBase {
public:

    friend void worka(mywindow* ptr);
    void SetDpi(int Dpi) {
        m_pm.SetDPI(Dpi);
    }

    void InitWindow() override {
        
        richedit = reinterpret_cast<CRichEditUI*>(m_pm.FindControl(L"richedit"));
    }
protected:
    //这两个是必须重写的函数
    virtual LPCTSTR GetWindowClassName(void) const override { return L"mainwindow"; }
    virtual CDuiString GetSkinFile() override { return L"mainwindow.xml"; } //窗口如何设计由xml决定
    //

    // 需要覆写这个
    // 因为duilib默认的关闭按钮是用来隐藏程序的，并不是关闭程序
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (uMsg == WM_CLOSE)
        {
            PostQuitMessage(0L);
        }

        return __super::OnClose(uMsg, wParam, lParam, bHandled);
    }

    virtual void Notify(TNotifyUI& msg) override;
    virtual void OnClick(TNotifyUI& msg) override;

private:
    CRichEditUI* richedit;
};

void worka(mywindow* ptr);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    ::CoUninitialize();
    CPaintManagerUI::SetInstance(hInstance);

    // 设置皮肤的默认路径
    CPaintManagerUI::SetCurrentPath(CPaintManagerUI::GetInstancePath());
    // 设置资源文件夹路径
    // 上面那个mainwindow.xml路径就变为res\mainwindow.xml
    CPaintManagerUI::SetResourcePath(_T("res"));            

    mywindow mainwindow;
    mainwindow.Create(nullptr, L"sbb_demo", UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 600);
    mainwindow.CenterWindow();
    mainwindow.SetDpi(CDPI::GetMainMonitorDPI());
    ShowWindow(mainwindow, SW_SHOW);

    std::thread t1(worka,&mainwindow);
    t1.detach();

    CPaintManagerUI::MessageLoop(); //一般不会返回
    ::CoUninitialize();                 

    return 0;
}

void mywindow::Notify(TNotifyUI& msg) {

    // 最基本的功能,窗口能够最小化和关闭
    if (msg.sType == DUI_MSGTYPE_CLICK) {

        CDuiString sCtrlName = msg.pSender->GetName(); 

        if (sCtrlName == _T("closebtn"))
        {
            PostMessage(WM_CLOSE, 0L, 0L);
            return;
        }
        else if (sCtrlName == _T("minbtn"))
        {
            SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
            return;
        }
        else if (sCtrlName == _T("maxbtn"))
        {
            SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
            return;
        }
        else if (sCtrlName == _T("restorebtn"))
        {
            SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
            return;
        }
        

    }
    __super::Notify(msg);
}

// 收不到信息不知道为啥,直接在上面的Notify里搞这些也行
void mywindow::OnClick(TNotifyUI& msg) {
    
    CDuiString sCtrlName = msg.pSender->GetName();  //判断点击的是哪个控件

    //只要控件名不变,这些可以直接抄
    if (sCtrlName == _T("closebtn"))
    {
        PostMessage(WM_CLOSE, 0L, 0L);
        return;
    }
    else if (sCtrlName == _T("minbtn"))
    {
        SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
        return;
    }
    else if (sCtrlName == _T("maxbtn"))
    {
        SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
        return;
    }
    else if (sCtrlName == _T("restorebtn"))
    {
        SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
        return;
    }

}

static void worka(mywindow* ptr) {
    static std::wstring str;
    while (1) {
        ptr->richedit->SetText(str.append(L"123\n").c_str());
        ptr->richedit->LineDown();
        ptr->richedit->LineScroll(ptr->richedit->GetLineCount() - 1);
        ptr->richedit->NeedUpdate();
        ptr->richedit->Invalidate();
        Sleep(1000);
    }
}
