#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stop_token>
 
template <typename T>
struct concurrent_queue
{ 
  T pop(std::stop_token const& st)
  {
    std::unique_lock<std::mutex> mlock(m_mutex);
    while (m_queue.empty())			
    {
      m_covar.wait(mlock);
			if (st.stop_requested()) {
				throw std::runtime_error("stop requested");
			}
		}
    const auto value = std::move(m_queue.front());
    m_queue.pop();
    return value;
  }
 
  void pop(T& value, std::stop_token const& st)
  {
    std::unique_lock<std::mutex> mlock(m_mutex);
    while (m_queue.empty())
    {
      m_covar.wait(mlock);
			if (st.stop_requested()) {
				throw std::runtime_error("stop requested");
			}
    }
    value = std::move(m_queue.front());
    m_queue.pop();
  }
 
  void push(const T& value)
  {
    std::unique_lock<std::mutex> mlock(m_mutex);
    m_queue.push(value);
    mlock.unlock();
    m_covar.notify_one();
  }
 
  void push(T&& value)
  {
    std::unique_lock<std::mutex> mlock(m_mutex);
    m_queue.push(std::move(value));
    mlock.unlock();
    m_covar.notify_one();
  }

  ~concurrent_queue()
	{		
		m_covar.notify_all();
	}
 
 private:
  std::queue<T>           m_queue;
  std::mutex              m_mutex;
  std::condition_variable m_covar;
};