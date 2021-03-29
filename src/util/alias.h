#pragma once

#include<chrono>
#include<cstdint>
#include<unordered_map>

namespace DAMCREM{

//! 時間やシステム起動からの時刻を表すのに使用する．単位は[us]．
using Time = uint64_t;

//! アプリケーションID
/*!
 * This is a public value. Structure of an application is also public.
 */
using AppID = uint64_t;

//! クライアントID
/*!
 * The server assign unique value to a client.
 */
using ClientID = uint64_t;

//! クエリID
/*!
 * A client can set this to determine the corresponding inputs and outputs.
 */
using QueryID = uint64_t;

//! ジョブID
/*!
 * The server assing unique value to a job.
 */
using JobID = uint64_t;

//! マクロタスクID
/*!
 * An application builder assign unique value to a macrotask.
 */
using MacrotaskID = uint64_t;

namespace util{
//! 時刻取得に使用するクロック
using Clock = std::chrono::steady_clock;

//! クロックに対応するtime_pointへのエイリアス
using ClockTimePoint = Clock::time_point;

//! chronoのdurationに変換する場合に使用する
using ClockDuration = std::chrono::microseconds;


}

namespace alias_for_generate_type{
template<class T>
T genNestedUnorderedMap();

template<class T, class U, class... Tail>
// std::unordered_map<T, decltype(genNestedUnorderedMap<U, Tail...>())> genNestedUnorderedMap();
auto genNestedUnorderedMap(){ return std::unordered_map<T, decltype(genNestedUnorderedMap<U, Tail...>())>(); }

}

template<class T, class U, class... Tail>
using NestedUnorderedMap = decltype(alias_for_generate_type::genNestedUnorderedMap<T, U, Tail...>());


}

