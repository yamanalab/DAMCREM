#include"communication/server/state.h"

namespace DAMCREM{
namespace communication{

void ServerStateReady::set(stdsc::StateContext &sc, uint64_t event){
  switch( static_cast<ServerEvent>(event) ){
    default:
      break;
  }
  return;
}

}  // namespace communication
}  // namespace DAMCREM
