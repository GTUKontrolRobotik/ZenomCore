#include "zenomcore.h"

ZenomCore::ZenomCore(QObject *parent, QStringList args)
    : QObject(parent)
    , mMessageListenerTask(nullptr)
{

    mDataRepository = DataRepository::instance();

    connect( &mControlBaseProcess, SIGNAL( error(QProcess::ProcessError) ),
             SLOT( controlBaseProcessError(QProcess::ProcessError) ));
    connect( &mControlBaseProcess, SIGNAL( readyReadStandardOutput() ),
             SLOT( controlBaseReadyReadStandardOutput() ));
    connect( &mControlBaseProcess, SIGNAL( readyReadStandardError() ),
             SLOT( controlBaseReadyReadStandardError() ));

    setSimulationState( TERMINATED );

    for (int i=0; i<args.length(); i++) {
        if ( args[i].contains( ".znm" ) )
        {
            openProject(args[i]);
            start();
        }
    }

    /* TODO List -----------------------------
     * 12. Add Target support
     *
     * -------------------------------------*/

    /* DONE List -----------------------------
     *  0. Qt argument parser Introduced @Qt5.2, we use old qt so use dummy method
     *  1. Open a project, read cnt,log variables
     *  2. Open shared memory
     *  3. mDataRepository
     *  4. setSimulationState
     *  5. mMessageListenerTask
     *  6. mControlBaseProcess
     *  7. start the task
     *  8. save the results on a matlab file
     *  9. mControlBaseProcess SIGNAL( error )
     * 10. mControlBaseProcess SIGNAL( readyReadStandardOutput )
     * 11. mControlBaseProcess SIGNAL( readyReadStandardError )
     *
     * -------------------------------------*/
}

void ZenomCore::openProject(const QString& pProjectPath)
{
    terminateProject();     // close project if already a project was opened.

    // TODO catch blogu ve return kontrol et hersey silindigine emin ol
    try
    {
        QString projectAbsolutePath = QFileInfo( pProjectPath ).absoluteFilePath();

        QFileInfo fileInfo( projectAbsolutePath );
        if ( !fileInfo.exists() )
        {
            std::cerr << "Error: Failed opening project: " << pProjectPath.toStdString() << " does not exist.";
            return;
        }

        QDir::setCurrent( fileInfo.path() );
        const QString projectName = fileInfo.baseName();
        const QString controlBaseProgram =
                fileInfo.dir().filePath( fileInfo.baseName() );

        mDataRepository->setProjectName( projectName.toStdString() );

        mDataRepository->createMessageQueues();

        setSimulationState( STOPPED );
        mMessageListenerTask = new MessageListenerTask(this);
        mMessageListenerTask->runTask();

        //  This is where control base process is started
        mControlBaseProcess.start( controlBaseProgram, QStringList() << projectName );
        if ( mControlBaseProcess.waitForStarted() )
        {
            // Controlbase'den mesaj gelene kadar beklenir.
            if ( !mMessageListenerTask->waitForInitMessage() )
            {
                std::cerr << "Error1: Failed connecting program: The program "
                           << "does not implemented specified format.";
                return;
            }

            if( !mDataRepository->readVariablesFromFile() )
            {
                std::cerr << "Error2: Failed connecting program: The program"
                          << " does not implemented specified format.";
                return;
            }

            std::cerr << "Creating shared memories" << std::endl;

            mDataRepository->createMainControlHeap();

            std::cerr << "Created shared memories" << std::endl;

            mDataRepository->sendStateRequest( R_INIT );

            // Controlbase main control heap'e baglanmasi ve
            // control variable degerlerini main control heap'e yazmasi beklenir.
            if ( !mMessageListenerTask->waitForInitMessage() )
            {
                std::cerr << "Error3: Failed connecting program: The program "
                          << "does not implemented specified format.";
                return;
            }

            loadSettings( projectAbsolutePath );

            mProjectPath = projectAbsolutePath;

            //ui->output->appendMessage( QString("%1 succesfully loaded.").arg(projectName) );

            std::cerr << "Open finished" << std::endl;
        }
    }
    catch( std::system_error e )
    {
        // TODO file exist ise ne olacak
        std::cout << "openProject" << std::string( e.what() ) << std::endl;
    }
}

void ZenomCore::terminateProject()
{
    std::cout<<"void Zenom::terminateProject()"<<std::endl;

    if (simulationState() == TERMINATED)
        return;

    // if the controlbase is running, stop it and then terminate.
    if( simulationState() == RUNNING || simulationState() == PAUSED )
    {
        stop();
    }


    if ( mControlBaseProcess.state() != QProcess::NotRunning )
    {
        mDataRepository->sendStateRequest( R_TERMINATE );

        if ( !mControlBaseProcess.waitForFinished(5000) )    // Finish the process
        {
            std::cerr << "Controlbase process did not finish!" << std::endl;
            //appendTextToOutput("Terminating unsuccesful");
            mControlBaseProcess.kill(); // TODO process bir sure sonlanmaz
            // ise kill etsek mi? kill edince crashed oluyor.
        }
    }

    setSimulationState( TERMINATED );

    if(mMessageListenerTask != nullptr)
        delete mMessageListenerTask;

    mDataRepository->deleteMessageQueues();
    std::cerr << "deleted message queues" << std::endl;
    mDataRepository->deleteMainControlHeap();
    std::cerr << "deleted main control heap" << std::endl;
    mDataRepository->deleteLogVariablesHeap();
    std::cerr << "deleted log variables heap" << std::endl;

    mDataRepository->clear();

}

State ZenomCore::simulationState()
{
    return mSimState;
}

void ZenomCore::setSimulationState(State pState)
{
    mSimState = pState;
}

void ZenomCore::stop()
{
    setSimulationState( STOPPED );
    mDataRepository->sendStateRequest( R_STOP );

    std::cout << "Simulation stopped." << std::endl;

    exportMatlab();
}

void ZenomCore::loadSettings(const QString &pProjectName)
{
    QSettings settings( pProjectName , QSettings::IniFormat);

    settings.beginGroup("zenom");
    setFrequency( settings.value("frequency", 1).toDouble() );
    setDuration( settings.value("duration", 100).toDouble() );
    settings.endGroup();
}

void ZenomCore::setFrequency(double pFrequency)
{
    if ( pFrequency > 0 )
    {
        mDataRepository->setFrequency( pFrequency );
        for ( unsigned int i = 0; i < mDataRepository->logVariables().size(); ++i )
        {
            DataRepository::instance()->logVariables().at(i)->setFrequency(pFrequency);
        }
    }
}

void ZenomCore::setDuration(double pDuration)
{
    if ( pDuration > 0 )
    {
        mDataRepository->setDuration( pDuration );
    }
}

void ZenomCore::controlBaseProcessError( QProcess::ProcessError pError )
{
    if( pError == QProcess::FailedToStart )
    {
        std::cerr << "Error: Failed opening project: The program does not exist."  << std::endl;
    }
    else if( pError == QProcess::Crashed )
    {
        std::cerr << "Error: The program has unexpectedly finished. Please, reload the project to start."  << std::endl;
    }
    else
    {
        std::cerr << "Error: An unknown error occurred. Please, reload the project to start."  << std::endl;
    }

    setSimulationState( CRASHED );
}

void ZenomCore::controlBaseReadyReadStandardOutput()
{
    std::cerr << QString(mControlBaseProcess.readAllStandardOutput()).toStdString() << std::endl;
}

void ZenomCore::controlBaseReadyReadStandardError()
{
    std::cerr << QString(mControlBaseProcess.readAllStandardError()).toStdString() << std::endl;
}

void ZenomCore::start()
{
    cntrVariables =mDataRepository->controlVariables();
    if( simulationState() == STOPPED )	// Start
    {
        setSimulationState( RUNNING );

        mDataRepository->deleteLogVariablesHeap();
        mDataRepository->createLogVariablesHeap();
        mDataRepository->sendStateRequest( R_START );

    }
    else if( simulationState() == RUNNING )
    {
        setSimulationState( PAUSED );
        mDataRepository->sendStateRequest( R_PAUSE );

        std::cout << "Simulation paused." << std::endl;
    }
    else if( simulationState() == PAUSED )	// Resume
    {
        setSimulationState( RUNNING );
        mDataRepository->sendStateRequest( R_RESUME );

        std::cout << "Simulation resumed." << std::endl;
    }
}

void ZenomCore::exportMatlab()
{
    //TODO: fix this file name
    QString fileName =  "test.m";

    if ( !fileName.isEmpty() )
    {
        if ( !fileName.endsWith(".m") )
            fileName += ".m";

        QFile file( fileName );
        if (!file.open(QIODevice::WriteOnly))
            return;

        // Log variable listesi dosyaya yazilir.
        MatlabStream out(&file);
        for ( unsigned int i = 0; i < DataRepository::instance()->logVariables().size(); ++i )
        {
            out.writeLogVariable( DataRepository::instance()->logVariables().at(i) );
        }
    }
}
