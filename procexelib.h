#ifndef PROCEXELIB_H
#define PROCEXELIB_H

#include <qglobal.h>
#include <QString>
#include <QVector>
#include <QMap>
#include <QIODevice>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "Psapi.lib")

class THandle {
    HANDLE handle;
public:
    THandle(HANDLE data=INVALID_HANDLE_VALUE):handle(data) {}
    THandle(const THandle&)=delete;
    THandle(THandle&& that):handle(that.handle) {
        if(this!=&that)
        that.handle=INVALID_HANDLE_VALUE;
    }
    ~THandle() {
        if(isValid()) {
            CloseHandle(handle);
            handle=INVALID_HANDLE_VALUE;
        }
    }
    THandle& operator=(const THandle&) =delete;
    THandle& operator=(THandle&& that) {
        if(this==&that) return *this;
        handle=that.handle;
        that.handle=INVALID_HANDLE_VALUE;
        return *this;
    }
    bool isValid() const {
        if(handle==INVALID_HANDLE_VALUE) return false;
        if(handle==NULL) return false;
        if(handle==nullptr) return false;
        return true;
    }
    void close() {
        if(isValid()) {
            CloseHandle(handle);
            handle=INVALID_HANDLE_VALUE;
        }
    }
    operator bool() const {
        return isValid();
    }
    operator HANDLE() {
        return handle;
    }
    operator const HANDLE() const {
        return handle;
    }
    bool setInheritFlag(bool inheritable = true) {
        if (!isValid()) return FALSE;
        if (SetHandleInformation(handle, HANDLE_FLAG_INHERIT, inheritable ? HANDLE_FLAG_INHERIT : 0)) return TRUE;
        else return FALSE;
    }
};
static THandle NullHandle(NULL);

class TPipe {
public:
    THandle hRead,hWrite;
    TPipe() {
        connect();
    }
    ~TPipe() {
        close();
    }
    TPipe(const TPipe&) = delete;
    TPipe(TPipe&& other) :
        hRead(std::move(other.hRead)),
        hWrite(std::move(other.hWrite)) {
    }
    TPipe& operator=(const TPipe&) = delete;
    TPipe& operator=(TPipe&& other) {
        hRead = std::move(other.hRead);
        hWrite = std::move(other.hWrite);
        return (*this);
    }
    bool connect() {
        close();
        SECURITY_ATTRIBUTES _sa = { sizeof(_sa), NULL, FALSE };
        HANDLE _hRead(INVALID_HANDLE_VALUE), _hWrite(INVALID_HANDLE_VALUE);
        if (CreatePipe(&_hRead, &_hWrite, &_sa, 0)) {
            hRead = THandle(_hRead);
            hWrite = THandle(_hWrite);
            return true;
        }
        return false;
    }
    void close() {
        hRead.close();
        hWrite.close();
    }
    operator bool() const {
        return hRead&&hWrite;
    }
};

class TFile {
public:
    THandle hFile;
    TFile() {}
    TFile(const QString& path,bool read_only=false) {
        open(path,read_only);
    }
    ~TFile() {
        close();
    }
    bool open(const QString& path,bool read_only=true) {
        close();
        SECURITY_ATTRIBUTES _sa = { sizeof(_sa), NULL, FALSE };
        HANDLE _hFile = CreateFileW(path.toStdWString().c_str(),
                                    (read_only ? GENERIC_READ : GENERIC_WRITE),
                                    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                    &_sa,
                                    (read_only ? OPEN_EXISTING : CREATE_ALWAYS),
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL
                                    );
        if (_hFile != INVALID_HANDLE_VALUE) {
            hFile = THandle(_hFile);
            return true;
        }
        return false;
    }
    TFile(const TFile&) = delete;
    TFile(TFile&& other) :
        hFile(std::move(other.hFile)) {
    }
    TFile& operator=(const TFile&) = delete;
    TFile& operator=(TFile&& other) {
        hFile = std::move(other.hFile);
        return (*this);
    }
    void close() {
        hFile.close();
    }
    operator bool() {
        return hFile;
    }
};

#define _MAX_TL 20000UL //20sec
#define _MAX_ML 4096UL  //4GiB

class TProcess {
    QString _cmdlet,_dir;
    STARTUPINFOW _si{ sizeof(STARTUPINFOW)};
    THandle * _PStdin, * _PStdout, * _PStderr;
    PROCESS_INFORMATION _pi{};
    DWORD _exit_code{0};
    ULARGE_INTEGER timeUsed{{0,0}};
    SIZE_T memUsed{0};
public:
    THandle hProcess,hThread;
    enum ExitStatus { NormalExit, CrashExit };
    enum ProcessState { NotRunning, Running } _state=NotRunning;
    TProcess()
        :_PStdin(&NullHandle), _PStdout(&NullHandle), _PStderr(&NullHandle)
    {}
    TProcess(const QString& command,const QStringList params={},const QString interpreter="")
        :_PStdin(&NullHandle), _PStdout(&NullHandle), _PStderr(&NullHandle)
    {
        setCmd(command,params,interpreter);
    }
    ~TProcess() {
        stop();
    }
    void setWorkingDirectory(const QString& dir) {
        _dir=dir;
    }
    void setCmd(const QString& command,const QStringList params={},const QString interpreter="") {
        _cmdlet="";
        if(!interpreter.isEmpty()) _cmdlet+=interpreter+" ";
        _cmdlet+="\""+command+"\"";
        if(!params.isEmpty()) {
            for(const QString& str:params) {
                _cmdlet+=" \""+str+"\"";
            }
        }
    }
    void setStandardInputHandle(THandle& handle) {
        _PStdin=&handle;
        _si.hStdInput=handle;
    }
    void setStandardOutputHandle(THandle& handle) {
        _PStdout=&handle;
        _si.hStdOutput=handle;
    }
    void setStandardErrorHandle(THandle& handle) {
        _PStderr=&handle;
        _si.hStdError=handle;
    }

    ExitStatus getExitStatus() {
        if(getExitCode()<0) return CrashExit;
        return NormalExit;
    }
    int getExitCode() {
        if(hProcess) {
            GetExitCodeProcess(hProcess, &_exit_code);
        }
        return _exit_code;
    }
    ProcessState getProcessState() {
        if(hProcess) {
            getExitCode();
            if(_exit_code==STILL_ACTIVE) _state=Running;
            else _state=NotRunning;
        } else _state=NotRunning;
        return _state;
    }
    unsigned int getTime_ms() {
        if(hProcess) {
            FILETIME creationTime, exitTime, kernelTime, userTime;
            if (GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime)) {
                timeUsed.LowPart = userTime.dwLowDateTime;
                timeUsed.HighPart = userTime.dwHighDateTime;
                timeUsed.QuadPart += (ULARGE_INTEGER{ {kernelTime.dwLowDateTime, kernelTime.dwHighDateTime} }.QuadPart);
            }
        }
        ULONGLONG cpuTime=timeUsed.QuadPart;
        return static_cast<DWORD>(cpuTime / 10000);
    }
    unsigned int getMemory_MiB() {
        if(hProcess) {
            PROCESS_MEMORY_COUNTERS _pmc;
            if (GetProcessMemoryInfo(hProcess, &_pmc, sizeof(_pmc))) {
                memUsed=_pmc.PeakWorkingSetSize;
            }
        }
        return memUsed/1048576;
    }

    bool start() {
        _PStdin->setInheritFlag();
        _PStdout->setInheritFlag();
        _PStderr->setInheritFlag();
        BOOL success = CreateProcessW(
            NULL,
            _cmdlet.toStdWString().data(),
            NULL,
            NULL,
            TRUE,
            NORMAL_PRIORITY_CLASS,
            NULL,
            _dir.isNull()?NULL:_dir.toStdWString().c_str(),
            &_si,
            &_pi
            );
        if (!success) return false;
        _PStdin->close();
        _PStdout->close();
        _PStderr->close();
        hProcess=THandle(_pi.hProcess);
        hThread=THandle(_pi.hThread);
        _state=Running;
        return true;
    }
    bool start(const QString& command,const QStringList params={}) {
        setCmd(command,params);
        return start();
    }
    bool stop() {
        bool success=true;
        if(getProcessState()==Running) success=TerminateProcess(hProcess, 0);
        _exit_code=-1;
        hProcess.close();
        hThread.close();
        return success;
    }
};



class THandleDevice : public QIODevice {//Written by DeepSeek, to be checked
    Q_OBJECT
public:
    explicit THandleDevice(THandle* handle,
                           QIODevice::OpenMode mode,
                           QObject* parent = nullptr)
        : QIODevice(parent),
        m_handle(handle),
        m_fileType(GetFileType(handle))
    {
        open(mode | QIODevice::Unbuffered);
    }

    ~THandleDevice() override {
        close();
    }

protected:
    // 实现QIODevice核心接口
    qint64 readData(char* data, qint64 maxSize) override {
        if (!isReadable() || !m_handle->isValid()) return -1;

        DWORD bytesRead = 0;
        BOOL ret = ::ReadFile(*m_handle, data,
                              static_cast<DWORD>(maxSize),
                              &bytesRead, nullptr);

        if (ret) return bytesRead;

        const DWORD err = ::GetLastError();
        if (err == ERROR_BROKEN_PIPE ||
            err == ERROR_NO_DATA) return 0;

        setErrorString(QString("Read failed (0x%1)").arg(err, 0, 16));
        return -1;
    }

    qint64 writeData(const char* data, qint64 maxSize) override {
        if (!isWritable() || !m_handle->isValid()) return -1;

        DWORD bytesWritten = 0;
        BOOL ret = ::WriteFile(*m_handle, data,
                               static_cast<DWORD>(maxSize),
                               &bytesWritten, nullptr);

        if (ret) return bytesWritten;

        setErrorString(QString("Write failed (0x%1)")
                           .arg(::GetLastError(), 0, 16));
        return -1;
    }

    bool isSequential() const override {
        return m_fileType != FILE_TYPE_DISK;
    }

    qint64 bytesAvailable() const override {
        if (!m_handle->isValid()) return 0;

        if (m_fileType == FILE_TYPE_PIPE) {
            DWORD bytesAvail = 0;
            ::PeekNamedPipe(*m_handle, nullptr, 0,
                            nullptr, &bytesAvail, nullptr);
            return bytesAvail;
        }
        return QIODevice::bytesAvailable();
    }

private:
    THandle* m_handle;      // 句柄指针
    DWORD m_fileType;       // 句柄类型

    // 获取文件类型辅助函数
    static DWORD GetFileType(THandle* h) {
        return h && h->isValid() ?
                   (::GetFileType(*h) & ~FILE_TYPE_REMOTE) :
                   FILE_TYPE_UNKNOWN;
    }
};

#else
#   error unsupported_system
#endif

#endif // PROCEXELIB_H
