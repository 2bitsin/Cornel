#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stop_token>
#include <exception>
 
struct stop_requested_error: std::exception
{
	stop_requested_error(std::string_view message): m_message(message) {}
	const char* what() const noexcept override { return m_message.c_str(); }
private:
	std::string m_message;
};

template <typename T>
struct concurrent_queue
{
  T pop(std::stop_token const& st)
  {
		std::stop_callback please_stop(st, [this] () {
			m_covar.notify_all();
		});
		
    std::unique_lock<std::mutex> mlock(m_mutex);
    while (m_queue.empty())			
    {
      m_covar.wait(mlock);
			if (st.stop_requested() || m_cease.load()) {
				throw stop_requested_error("stop requested");
			}
		}
    const auto value = std::move(m_queue.front());
    m_queue.pop();
    return value;
  } 
	
  void pop(T& value, std::stop_token const& st)
  {
		std::stop_callback please_stop(st, [this] () {
			m_covar.notify_all();
		});
    std::unique_lock<std::mutex> mlock(m_mutex);
    while (m_queue.empty())
    {
      m_covar.wait(mlock);
			if (st.stop_requested() || m_cease.load()) {
				throw stop_requested_error("stop requested");
			}
    }
    value = std::move(m_queue.front());
    m_queue.pop();
  }
	
  T pop()
  {
    std::unique_lock<std::mutex> mlock(m_mutex);
    while (m_queue.empty())			
    {
      m_covar.wait(mlock);
			if (m_cease.load()) {
				throw std::runtime_error("stop requested");
			}
		}
    const auto value = std::move(m_queue.front());
    m_queue.pop();
    return value;
  }

  void pop(T& value)
  {
    std::unique_lock<std::mutex> mlock(m_mutex);
    while (m_queue.empty())
    {
      m_covar.wait(mlock);
			if (m_cease.load()) {
				throw std::runtime_error("stop requested");
			}
    }
    value = std::move(m_queue.front());
    m_queue.pop();
  } 

	bool try_pop(T& value)
	{
		std::unique_lock<std::mutex> mlock(m_mutex);
		if (m_queue.empty())
			return false;
		value = std::move(m_queue.front());
		m_queue.pop();
		return true;
	}

	T try_pop()
	{
		std::unique_lock<std::mutex> mlock(m_mutex);
		if (m_queue.empty())
			throw std::runtime_error("empty");
		const auto value = std::move(m_queue.front());
		m_queue.pop();
		return value;
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

	template<typename...Q>
	void emplace(Q&&... args)
	{
		std::unique_lock<std::mutex> mlock(m_mutex);
		m_queue.emplace(std::forward<Q>(args)...);
		mlock.unlock();
		m_covar.notify_one();
	}
	
	bool empty() const
	{
		std::unique_lock<std::mutex> mlock(m_mutex);
		return m_queue.empty();
	}

	concurrent_queue()
	:	m_cease{ false }
	{}
	
  ~concurrent_queue()
	{		
		m_cease.store(true);
		m_covar.notify_all();		
	}
 
 private:
  std::queue<T>           m_queue;
  std::mutex              m_mutex;
  std::condition_variable m_covar;
	std::atomic_bool				m_cease;
};