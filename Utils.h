/***************************************************************************************************************************************************************************************************************************************************************************
 *  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
 *  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
 *  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
 *  You may obtain a copy of Mulan PSL v2 at:																																																							   *
 *            http://license.coscl.org.cn/MulanPSL2																																																						   *
 *   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
 *   See the Mulan PSL v2 for more details.  																																																							   *
 ***************************************************************************************************************************************************************************************************************************************************************************/
#ifndef _UTILS_H_
#define _UTILS_H_
#include "include.h"
/* C++ABI的定义 */
#define far
#define near
#define huge

VOID huge *operator new(unsigned long int size) noexcept;
VOID huge *operator new[](unsigned long int cb) noexcept;
VOID operator delete(VOID far *address) noexcept;
VOID operator delete[](VOID far *address) noexcept;

#endif // !_UTILS_H_