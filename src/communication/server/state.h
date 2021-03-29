#pragma once

#include"stdsc/stdsc_state.hpp"

namespace DAMCREM{
namespace communication{

enum class ServerState : int32_t {
  kNil = 0,
  kReady = 1,
  kExit = 2,
};

enum class ServerEvent : uint64_t {
  kNil = 0,
  kUpdateSchemeInstance = 1,
  kUpdateSchemeAlias = 2,
  kDeleteScheme = 3,
  kQuery = 4,
  kResult = 5,
};

struct ServerStateReady : public stdsc::State {
  static std::shared_ptr<stdsc::State> create(){
    return std::shared_ptr<stdsc::State>(new ServerStateReady{});
  }

  ServerStateReady(){}
  ~ServerStateReady() = default;
  ServerStateReady(const ServerStateReady &src) noexcept = default;
  ServerStateReady(ServerStateReady &&src) noexcept = default;

  virtual void set(stdsc::StateContext &sc, uint64_t event) override;
  STDSC_STATE_DEFID(static_cast<int32_t>(ServerState::kReady));
  
};

}  // namespace communication
}  // namespace DAMCREM

