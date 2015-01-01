#pragma once

#include "Utilities/XAssert.h"

void assertCheck(bool val);

#undef REFLECT_ASSERT
#define REFLECT_ASSERT assertCheck


#include <iostream>
#define DEBUG(op) std::cout << op << std::endl

