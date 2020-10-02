#ifndef ZENOMCORE_H
#define ZENOMCORE_H

#include <QObject>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QSettings>

#include <iostream>

#include <datarepository.h>
#include <MsgQueue.h>
#include "messagelistenertask.h"
#include "utility/matlabstream.h"

class ZenomCore : public QObject
{
    Q_OBJECT
public:
    explicit ZenomCore(QObject *parent = nullptr, QStringList args = QStringList() );

    /**
     * Benzetim durumunu getirir.
     * @return benzetim durumunu getirir.
     */
    State simulationState();

    /**
     * Benzetimi baslatir
     */
    void start();

signals:

public slots:
    void stop();

private slots:

    /**
     * Control-Base process'te bir hata olustugunda cagrilir.
     * @param pError hata kodu
     */
    void controlBaseProcessError( QProcess::ProcessError pError );

    /**
     * Kullanici tarafindan girilen standart mesajlarini alir.
     * Control-Base tarafindan cout kullanilarak
     * yazilan mesajlar ele alinir.
     */
    void controlBaseReadyReadStandardOutput();

    /**
     * Kullanici tarafindan girilen hata mesajlarini alir.
     * Control-Base tarafindan cerr kullanilarak
     * yazilan mesajlar ele alinir.
     */
    void controlBaseReadyReadStandardError();


private:

    /**
     * Su an yuklu olan projenin adresini tutar.
     */
    QString mProjectPath;

    /**
     * znm projesi acmak istenildigi zaman cagrilir.
     * Haberlesme (MessageQueue) baglantilari, ana heap burada olusturulur.
     *
     * @param pProjectName znm dosyasini adresi
     */
    void openProject(const QString& pProjectName);

    /**
     * znm projesini sonlandirir. Haberlesme(MessageQueue) baglantilari kapatilir,
     * ana heap, var ise log variable alanlari sisteme geri verilir.
    */
    void terminateProject();

    /**
     * Control programini calistiran siniftir.
     */
    QProcess mControlBaseProcess;

    /**
     * Control programindan gelen mesajlari dinleyen siniftir.
     */
    MessageListenerTask* mMessageListenerTask;

    /**
     * Benzetim durumunu gunceller. Duruma gore arayuzdeki alanlar
     * aktif/pasif hale getilir.
     * @param pState yeni benzetim durumu
     */
    void setSimulationState( State pState );

    /**
     * Benzetim durumunu tutar.
     */
    State mSimState;

    /**
     * Ana heap, log variable ve control variable listesinin tutuldugu ana siniftir.
     */
    DataRepository* mDataRepository;

    /**
     * znm projesine kayit edilen veriler yuklenir. Kayit edilmis
     * control-variable degerleri,
     * frekans ve zaman degerleri arayuze yuklenir.
     *
     * @param pProjectName znm dosyasini adresi
     */
    void loadSettings(const QString& pProjectName);

    /**
     * Frekans degerini gunceller. Frekans degeri 0'dan buyuk degil ise
     * deger guncellenmez.
     * @param pFrequency yeni frekans degeri
     */
    void setFrequency(double pFrequency);

    /**
     * Sure degerini gunceller. Sure degeri 0'dan buyuk degil ise
     * deger guncellenmez.
     * @param pDuration yeni sure degeri
     */
    void setDuration(double pDuration);

    /**
     * Butun log degiskenlerini Matlab (.m) dosya formatina uygun
     * olarak kayit eder.
     */
    void exportMatlab();


    //control variables
    std::vector<ControlVariable*> cntrVariables;
};

#endif // ZENOMCORE_H
