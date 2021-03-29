#pragma once

#include"computational_resource.h"
#include"operand_data.h"
#include"operation.h"
#include"scheme_info.h"

namespace DAMCREM{
//! 使用する準同型暗号ライブラリに依存せずに，準同型演算を使用できるようにするためのラッパクラス
/*!
 * 
 */
class Scheme{
public:
  static const std::string& scheme_name() noexcept;
    
  Scheme() = default;
  virtual ~Scheme() = default;
  Scheme(const SchemeInfo &scheme_info) noexcept : scheme_info_(scheme_info){}
  Scheme(SchemeInfo &&scheme_info) noexcept : scheme_info_(std::move(scheme_info)){}
  Scheme(const Scheme &src) noexcept = default;
  Scheme(Scheme &&src) noexcept = default;

  Scheme(std::filesystem::path file_path_prefix) : scheme_info_(file_path_prefix){}

  const SchemeInfo& scheme_info() const noexcept { return scheme_info_; }

  void encode(Plaintext &v0, const std::vector<double> &v1, size_t nt=1) const;
  void decode(std::vector<double> &v0, const Plaintext &v1, size_t nt=1) const;
  void encrypt(Ciphertext &v0, const Plaintext &v1, size_t nt=1) const;
  void encrypt(Ciphertext &v0, const std::vector<double> &v1, size_t nt=1) const;
  void decrypt(Plaintext &v0, const Ciphertext &v1, size_t nt=1) const;

  //! @note 他のストリーム系と合わせるなら，ストリームが先では？
  template<class T>
  bool load_data(T &t, std::istream &stream) const;
  
  template<class T>
  bool load_data(T &t, const std::filesystem::path &file_path) const;

  template<class T>
  T load_data(const std::filesystem::path &file_path) const;

  template<class T>
  bool save_data(const T &t, std::ostream &stream) const;
  
  template<class T>
  bool save_data(const T &t, const std::filesystem::path &file_path) const;
    
  void copy(Plaintext &v0, const Plaintext &v1, size_t nt=1) const;
  void copy(Ciphertext &v0, const Ciphertext &v1, size_t nt=1) const;
  void add(Ciphertext &v0, const Plaintext &v1, size_t nt) const;
  void add(Ciphertext &v0, const Ciphertext &v1, size_t nt) const;
  void sub(Ciphertext &v0, const Plaintext &v1, size_t nt) const;
  void sub(Ciphertext &v0, const Ciphertext &v1, size_t nt) const;
  void mul(Ciphertext &v0, const Plaintext &v1, size_t nt) const;
  void mul(Ciphertext &v0, const Ciphertext &v1, size_t nt) const;
  void square(Ciphertext &v0, size_t nt) const;

  void relinearize(Ciphertext &v0, size_t nt) const;

  void rescale(Ciphertext &v0, size_t nt) const;

  void simple_modulus_reduction(Plaintext &v0, size_t nt=1) const;
  void simple_modulus_reduction(Ciphertext &v0, size_t nt=1) const;

  void bootstrap(Ciphertext &v0, size_t nt) const;

  //! Operationクラスのインスタンスや平文，暗号文，使用スレッド数の情報を与えることで，適切な関数を呼び出して実行する．
  /*!
   * @param[in] op 実行する準同型演算の情報
   * @param[in] pt 平文への参照のvector
   * @param[in] ct 暗号文への参照のvector
   * @param[in] cr 使用する計算機資源量
   * @note この関数はscheme.cppで実装済み
   */
  void execute(const Operation &op, OperandData<Plaintext> &pt, OperandData<Ciphertext> &ct, const ComputationalResource &cr) const{
    switch( op.op() ){
    case Operation::OP::COPY_PLAIN:
      copy(pt.dst(op.v0()), pt.src(op.v1()), cr.num_threads()); break;
    case Operation::OP::COPY:
      copy(ct.dst(op.v0()), ct.src(op.v1()), cr.num_threads()); break;
    case Operation::OP::ADD_PLAIN:
      add(ct.dst(op.v0()), pt.src(op.v1()), cr.num_threads()); break;
    case Operation::OP::ADD:
      add(ct.dst(op.v0()), ct.src(op.v1()), cr.num_threads()); break;
    case Operation::OP::SUB_PLAIN:
      sub(ct.dst(op.v0()), pt.src(op.v1()), cr.num_threads()); break;
    case Operation::OP::SUB:
      sub(ct.dst(op.v0()), ct.src(op.v1()), cr.num_threads()); break;
    case Operation::OP::MUL_PLAIN:
      mul(ct.dst(op.v0()), pt.src(op.v1()), cr.num_threads()); break;
    case Operation::OP::MUL:
      mul(ct.dst(op.v0()), ct.src(op.v1()), cr.num_threads()); break;
    case Operation::OP::SQUARE:
      square(ct.dst(op.v0()), cr.num_threads()); break;
    case Operation::OP::RELINEARIZE:
      relinearize(ct.dst(op.v0()), cr.num_threads()); break;
    case Operation::OP::RESCALE:
      rescale(ct.dst(op.v0()), cr.num_threads()); break;
    case Operation::OP::SMR_PLAIN:
      simple_modulus_reduction(pt.dst(op.v0()), cr.num_threads()); break;
    case Operation::OP::SMR:
      simple_modulus_reduction(ct.dst(op.v0()), cr.num_threads()); break;
    case Operation::OP::BOOTSTRAP:
      bootstrap(ct.dst(op.v0()), cr.num_threads()); break;
    default:
      throw std::invalid_argument("Invalid operation @ Scheme::execute"); break;
    }
    return;
  }
    
private:
  SchemeInfo scheme_info_;
};
}  // namespace DAMCREM

#if defined(DO_USE_SEAL_CKKS)
#include"scheme/seal_ckks.h"

#elif defined(DO_USE_PALISADE)
#include"scheme/palisade.h"

#else
#include"scheme/dummy.h"
#endif
