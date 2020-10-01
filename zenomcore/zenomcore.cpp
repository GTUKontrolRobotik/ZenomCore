#include "zenomcore.h"

ZenomCore::ZenomCore(QObject *parent, QStringList args) : QObject(parent)
{
    for (int i=0; i<args.length(); i++) {
        if ( args[i].contains( ".znm" ) )
        {
            std::cout << args[i].toStdString() << std::endl;
        }
    }

    /* TODO List -----------------------------
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

    /* DONE List -----------------------------
     *  0. Qt argument parser Introduced @Qt5.2, we use old qt so use dummy method
     *
     * -------------------------------------*/
}
