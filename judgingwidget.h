#ifndef JUDGINGWIDGET_H
#define JUDGINGWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QMessageBox>
#include <QMutex>
#include "ctsettings.h"

class JudgingThread:public QObject {
    Q_OBJECT
public:
    explicit JudgingThread(QObject *parent = nullptr);
    ~JudgingThread();
    mutable QMutex mutex,amutex;
    class JudgeRunner {
    public:
        JudgeRunner(const Problem& _problem,
                    const QString& _participant,
                    const QMap<QString,QString>& _pa_files):
            problem(_problem),participant(_participant),pa_files(_pa_files) {}
        virtual ~JudgeRunner()=default;
        Problem problem;
        const QString participant;
        const QMap<QString,QString> pa_files;
        virtual void StartJud(JudgingThread* parent,bool abort=false) = 0;
        virtual bool cpl()=0;
    };
    class cplRunner:public JudgeRunner {
    public:
        const QString ctdir;
        cplRunner(const Problem& _problem,
                  const QString& _participant,
                  const QString& _ctdir,
                  const QMap<QString,QString>& _pa_files):
            JudgeRunner(_problem,_participant,_pa_files),ctdir(_ctdir)
        {}
        virtual void StartJud(JudgingThread* parent,bool abort=false);
        virtual bool cpl() {return true;}
    };
    class judRunner:public JudgeRunner {
    public:
        const int caseid;
        judRunner(const Problem& _problem,
                  const QString& _participant,
                  const QMap<QString,QString>& _pa_files,
                  const int _caseid):
            JudgeRunner(_problem,_participant,_pa_files),caseid(_caseid)
        {}
        virtual void StartJud(JudgingThread* parent,bool abort=false);
        virtual bool cpl() {return false;}
    };
    QList<JudgeRunner*> tasks;
    void addCplWaitlist(const Problem& problem,
                        const QString& participant,
                        const QString& ctdir,
                        const QMap<QString,QString>& pa_files);
    void addJudWaitlist(const Problem& problem,
                        const QString& participant,
                        const QMap<QString,QString>& pa_files,
                        int caseid);
    bool _abort=false;
    bool _pendingflag=false;
    bool _cplsuccess=false;
public slots:
    void startJud();
    void setAbort(bool abort);
signals:
    void CplResult(const QString& participant,
                    const QString& problem,
                    TResult verdict,
                    const QString& log);
    void JudgeResult(const QString& participant,
                      const QString& problem,
                      int caseid,
                      TResult verdict,
                      double score,
                      unsigned int time,
                      unsigned int mem,
                      const QString& log);
};



namespace Ui {
class JudgingWidget;
}

class JudgingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JudgingWidget(QWidget *parent = nullptr,bool controllable=true);
    ~JudgingWidget();
    QThread* judThreadContainer;
    JudgingThread* judThread;
public slots:
    void clrResult();
    void showCplResult(const QString& participant,
                    const QString& problem,
                    TResult verdict,
                    const QString& log);
    void showJudResult(const QString& participant,
                      const QString& problem,
                      int caseid,
                      TResult verdict,
                      double score,
                      unsigned int time,
                      unsigned int mem,
                      const QString& log);

public:
    void prepareJudge(const Problem& problem,
                        const QString& participant,
                        const QString& ctdir,
                        const QMap<QString,QString>& pa_files);
    void runJudge(const Problem& problem,
                    const QString& participant,
                    const QMap<QString,QString>& pa_files,
                    int caseid);
    void startJudge();
public: signals:
    void startJudgeSig();
    void cplComplete(const QString& participant,
                     const QString& problem,
                     TResult verdict,
                     const QString& log);
    void judgeComplete(const QString& participant,
                       const QString& problem,
                       int caseid,
                       TResult verdict,
                       double score,
                       unsigned int time,
                       unsigned int mem,
                       const QString& log);

private slots:
    void on_StopBTN_clicked();

    void on_ContinueBTN_clicked();

    void on_ClearBTN_clicked();

    void on_judgeTWid_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::JudgingWidget *ui;
};

#endif // JUDGINGWIDGET_H
