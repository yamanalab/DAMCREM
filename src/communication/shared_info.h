#pragma once


namespace DAMCREM{
namespace communication{




enum class ControlCode : uint64_t {
  /* Code for request packet: 0x0201-0x02ff*/

  kDataNewClientID               = 0x0401,  //!< server -> client
  kDataUpdateScheme              = 0x0411,  //!< client -> server
  kDataUpdateSchemeAlias         = 0x0412,  //!< client -> server
  kDataDeleteScheme              = 0x0413,  //!< client -> server
  kDataUpdateConstData           = 0x0414,  //!< client -> server
  kDataUpdateConstDataAlias      = 0x0415,  //!< client -> server
  kDataDeleteConstData           = 0x0416,  //!< client -> server
  kDataResult                    = 0x0422,  //!< server -> client

  kUpDownloadNewClientID         = 0x1001,  //!< client -> server
  kUpDownloadQuery               = 0x1021,  //!< client -> server  一方的に送るので，kDataQueryにすべき？
  kUpDownloadResult              = 0x1022,  //!< client -> server

};




}  // namespace communication
}  // namespace DAMCREM
