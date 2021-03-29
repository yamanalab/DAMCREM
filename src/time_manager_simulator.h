#pragma once

#include<queue>

#include"time_manager.h"

namespace DAMCREM{
class TimeManagerSimulator : public TimeManager {
public:
  TimeManagerSimulator(){}
  virtual ~TimeManagerSimulator() = default;
  TimeManagerSimulator(const TimeManagerSimulator &src) noexcept = delete;
  TimeManagerSimulator(TimeManagerSimulator &&src) noexcept = default;

  Time get_current_time() const override final { return current_time_; }

  Time update_time() override final {
    if( next_event_occuring_time_queue_.size() > 0 ){
      current_time_ = next_event_occuring_time_queue_.top();
      next_event_occuring_time_queue_.pop();
    }
    return current_time_;
  }

  void push_next_event_occuring_time(Time t){ next_event_occuring_time_queue_.push(t); }
  
private:
 std::priority_queue<Time, std::vector<Time>, std::greater<Time>> next_event_occuring_time_queue_;
  
};

}

