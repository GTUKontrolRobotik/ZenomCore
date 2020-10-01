#include "zenomcore.h"

ZenomCore::ZenomCore(QObject *parent, int argc, char *argv[]) : QObject(parent)
{
    std::cout << argc << " " << std::endl;
    for (int i=0; i<argc; i++) {
        std::cout << argv[i] << std::endl;
    }

    /* TODO List -----------------------------
     *  0. Qt argument parser
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
