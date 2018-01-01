#ifndef _NET_EVENT_LOOP_HPP
#define _NET_EVENT_LOOP_HPP

#include "CurrentThread.hpp"
#include "Timestamp.hpp"
#include <mutex>
#include <vector>
#include <cassert>
#include <atomic>
#include <memory>

namespace net
{

class Channel;
class PollerBase;


using Functor = std::function<void()>;


class EventLoop
{
public:
EventLoop();
~EventLoop();

void loop();

 /* If in the same loop thread, cb is run immediately.
 otherwise store in the task queue.
Safe to call from any threads.*/
void runInLoop(Functor&& cb);

void assertInLoopThread() const
{
  if(!isInLoopThread()) assert(0);
}

private:

void queueInLoop( Functor&& cb);
bool isInLoopThread() const
{
  return threadId_ == currentThread::tid();
}

void doPendingFunctors();

  EventLoop(const EventLoop&) = delete;
  EventLoop& operator=(const EventLoop&) = delete;


 const pid_t threadId_;
 
 std::mutex mutex_;
 std::vector<Functor> pendingFunctors_;//guarded by mutex_, because accessed by multi-threads;

 std::unique_ptr<PollerBase> poller_;

 std::atomic<bool> quit_;

 using ChannelList = std::vector<Channel*>;
 ChannelList activeChannels_;
 
};


}


#endif
