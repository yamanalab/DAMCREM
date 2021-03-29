#pragma once

#include"util/alias.h"

namespace DAMCREM{

namespace util{
//! 差分を[us]単位で返す関数
/*!
 * @param[in] st 始点時刻
 * @param[in] et 終点時刻
 */
inline Time get_diff_time(const ClockTimePoint &st, const ClockTimePoint &et){
  return std::chrono::duration_cast<ClockDuration>(et-st).count();
}
    
//! 差分を指定された単位(scale=1.0の場合は秒単位)で返す関数
/*!
 * @param[in] st 始点時刻
 * @param[in] et 終点時刻
 * @param[in] scale 単位を決めるための引数．1.0で秒単位，0.001でms単位
 */
inline double get_diff_time(const ClockTimePoint &st, const ClockTimePoint &et, double scale){
  return get_diff_time(st, et) / scale / 1000000;
}

}  // namespace util
}  // namespace DAMCREM

