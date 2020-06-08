#ifndef __ITRACE_H__
#define __ITRACE_H__
#include<stdio.h>
#include"ShareDef.h"

struct ITraceSink
{
    enum {ERRORTYPE_0,ERRORTYPE_1,ERRORTYPE_2};
    virtual void OnTrace(uchar btErrorType,const stchar* pStr)=0;
};

struct ITrace{
    virtual void Release(void)=0;
    virtual bool TraceLn(const stchar* lpszFormat,...)=0;
    virtual bool TraceErrorLn(const stchar* lpszFormat,...)=0;
    virtual bool TraceColorLn(COLORREF color,const stchar* lpszFormat,...)=0;
    virtual void SaveLocalLog(const stchar* szFileName,const stchar* lpszFormat,...)=0;
};

class CTraceCreateHelper{
    //typedef bool (__cdecl *CreateTraceProc)(ITrace **ppTrace,void *hRiceEdit,const stchar* szFileName,ITraceSink,*pTSink);
public:
    CTraceCreateHelper();
    ~CTraceCreateHelper(){
        Close();
    }
    bool Create(void* hRichEdit=nullptr,const stchar* szFileName=NULL,ITraceSink* pTSink=NULL){
        if(m_hDll!=nullptr){
            return false;
        }
        try{
            //m_hDll=::Loadlibrary(TEXT("Trace.dee"));
            if(nullptr==m_hDll){
                throw ("Cann't load Trace.dll");
            }
       /*     CreateTraceProc pProc=nullptr;
            pProc=(CreateTraceProc)::GetProcAddress(m_hDll,"CreateObject");
            if(nullptr==pProc){

            }
            if(false==pProc(&m_pTrace,hRichEdit,szFileName,pTSink)){

            }*/
        }
        catch (const stchar* szMsg){

        }
        catch (...){

        }
        return true;
    }

    void Close(){
        if(m_pTrace!=nullptr){
            m_pTrace->Release();
            m_pTrace=nullptr;
        }
        if(m_hDll!=nullptr){
            //::FreeLibrary(m_hDll);
            m_hDll=nullptr;
        }
    }

public:
    ITrace *m_pTrace;
private:
    int * m_hDll;

};
#endif