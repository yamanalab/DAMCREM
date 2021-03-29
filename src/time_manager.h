#pragma once

#include"util/time.h"

namespace DAMCREM{
class TimeManager{
public:
  TimeManager(){}
  virtual ~TimeManager() = default;
  TimeManager(const TimeManager &src) = delete;
  TimeManager(TimeManager &&src) = default;

  virtual void init_system_start_time(){ system_start_time_ = util::Clock::now(); }

  //! 現在時刻を返す関数
  virtual Time get_current_time() const { return util::get_diff_time(system_start_time_, util::Clock::now()); }

  //! current_time_を更新する関数
  virtual Time update_time(){ return (current_time_ = get_current_time()); }

  Time convert_to_relative_time(const util::ClockTimePoint &t) const { return util::get_diff_time(system_start_time_, t); }
  
  /*! @name Getter */
  /*! @{ */
  util::ClockTimePoint system_start_time() const noexcept { return system_start_time_; }
  Time current_time() const noexcept { return current_time_; }
  /*! @} */
  
protected:
  util::ClockTimePoint system_start_time_ = util::ClockTimePoint::min();
  
  Time current_time_;
};


}  // namespace DAMCREM

