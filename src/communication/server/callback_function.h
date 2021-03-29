#pragma once

#include"util/alias.h"

#include"stdsc/stdsc_callback_function.hpp"
#include"communication/stdsc_all.h"

namespace DAMCREM{
namespace communication{
DECLARE_DATA_CLASS(CallbackFunctionForUpdateScheme);
DECLARE_DATA_CLASS(CallbackFunctionForDeleteScheme);
DECLARE_DATA_CLASS(CallbackFunctionForUpdateConstData);
DECLARE_DATA_CLASS(CallbackFunctionForDeleteConstData);

DECLARE_UPDOWNLOAD_CLASS(CallbackFunctionForNewClientID);
DECLARE_UPDOWNLOAD_CLASS(CallbackFunctionForQuery);
DECLARE_UPDOWNLOAD_CLASS(CallbackFunctionForResult);


}  // namespace communication
}  // namespace DAMCREM

