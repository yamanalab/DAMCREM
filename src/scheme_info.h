#pragma once

#if defined(DO_USE_SEAL_CKKS)
#include"scheme_info/seal_ckks.h"

#elif defined(DO_USE_PALISADE)
#include"scheme_info/palisade.h"

#else
#include"scheme_info/dummy.h"
#endif

