#ifndef MESSAGELISTENERTASK_H_
#define MESSAGELISTENERTASK_H_

#include <TaskXn.h>
#include <mutex>
#include <condition_variable>
#include "znm-core_global.h"
class ZenomCore;

class MessageListenerTask: public TaskXn
{
public:
    MessageListenerTask(ZenomCore* pZenom);

    bool waitForInitMessage();

private:
    void run() override;
    ZenomCore* mZenom;
    std::condition_variable condvar;
    std::mutex mtx;
    int initMsgs;

};

#endif /* MESSAGELISTENERTASK_H_ */
