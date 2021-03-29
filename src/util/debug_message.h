#pragma once

#include<iostream>
#include<sstream>
#include<string>

namespace DAMCREM{
  namespace util{
    //! デバッグメッセージを出力するクラス
    class DebugMessage{
    public:
      DebugMessage(std::ostream &os=std::clog) : os_(os){}

      template<class T>
      DebugMessage& operator<<(const T &str){
#if defined(DAMCREM_DEBUG_MESSAGE)
        os_ << str;
#endif
        return *this;
      }
      
      DebugMessage& operator<<(std::ostream& (*__pf)(std::ostream&)){
#if defined(DAMCREM_DEBUG_MESSAGE)
        os_ << __pf;
#endif
        return *this;
      }

      void message(){ return; }
      
      template<class Head, class... Tail>
      void message(Head &&head, Tail&&... tail){
#if defined(DAMCREM_DEBUG_MESSAGE)
        os_ << head;
        message(std::forward<Tail>(tail)...);
#endif
        return;
      }
      
    private:
      std::ostream &os_;
    };

    inline DebugMessage dbg_msg;

  }
}

