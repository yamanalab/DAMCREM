#pragma once

#include<algorithm>
#include<iostream>
#include<unordered_map>

namespace DAMCREM{
  class Operation{
  public:
    enum class OP{
      COPY_PLAIN,  //!< 平文のコピー
      COPY,  //!< 暗号文のコピー
      ADD_PLAIN,  //!< 暗号文と平文の準同型加算
      ADD,  //!< 暗号文同士の準同型加算
      SUB_PLAIN,  //!< 暗号文と平文の準同型減算
      SUB,  //!< 暗号文同士の準同型減算
      MUL_PLAIN,  //!< 暗号文と平文の準同型乗算
      MUL,  //!< 暗号文同士の準同型乗算
      SQUARE,  //!< 暗号文の二乗
      RELINEARIZE,  //!< 暗号文に対するRelinearize
      RESCALE,  //!< 暗号文に対するRescale
      SMR_PLAIN,  //!< 平文に対するSimple Modulus Reduction
      SMR,  //!< 暗号文に対するSimple Modulus Reduction
      BOOTSTRAP,  //!< 暗号文に対するブートストラップ
      INVALID  //!< 番兵．無効な要素
    };

    static inline OP str2OP(const std::string &str){
      std::string upper_str;
      std::transform(str.cbegin(), str.cend(), std::back_inserter(upper_str), ::toupper);
      return STR2OP.at(upper_str);
    }

    static inline const std::string& OP2str(OP op){ return OP2STR.at(op); }
    
    Operation(OP op, int v0) : op_(op), v0_(v0){}
    Operation(OP op, int v0, int v1) : op_(op), v0_(v0), v1_(v1){}
    Operation(const std::vector<std::string> &str)
      : op_(str2OP(str.at(0))), v0_(std::stoi(str.at(1))), v1_(str.size()==3 ? std::stoi(str.at(2)) : -1){}
    
    OP op() const noexcept { return op_; }
    int v0() const noexcept { return v0_; }
    int v1() const noexcept { return v1_; }

  private:
    static inline const std::unordered_map<std::string, OP> STR2OP = {
      {"COPY_PLAIN", OP::COPY_PLAIN},
      {"COPY", OP::COPY},
      {"ADD_PLAIN", OP::ADD_PLAIN},
      {"ADD", OP::ADD},
      {"SUB_PLAIN", OP::SUB_PLAIN},
      {"SUB", OP::SUB},
      {"MUL_PLAIN", OP::MUL_PLAIN},
      {"MUL", OP::MUL},
      {"SQUARE", OP::SQUARE},
      {"RELINEARIZE", OP::RELINEARIZE},
      {"RESCALE", OP::RESCALE},
      {"SMR_PLAIN", OP::SMR_PLAIN},
      {"SMR", OP::SMR},
      {"BOOTSTRAP", OP::BOOTSTRAP},
    };

    static inline const std::unordered_map<OP, std::string> OP2STR = []() -> std::unordered_map<OP, std::string> {
      std::unordered_map<OP, std::string> tmp;
      std::transform(STR2OP.cbegin(), STR2OP.cend(), std::inserter(tmp, tmp.end()),
                     [](const std::pair<std::string, OP> &x){ return std::make_pair(x.second, x.first); });
      return tmp;
    }();
      
    //! 準同型演算の種類
    OP op_ = OP::INVALID;

    //! 準同型演算の入力．原則この値は暗号文を指すこと．また，演算結果の代入先であること．
    int v0_ = -1;

    //! 準同型演算の入力．
    int v1_ = -1;
  };
  
}

inline std::ostream& operator<<(std::ostream &stream, const DAMCREM::Operation &op){
  stream << "op: " << DAMCREM::Operation::OP2str(op.op()) << ", " << op.v0() << ", " << op.v1();
  return stream;
}

