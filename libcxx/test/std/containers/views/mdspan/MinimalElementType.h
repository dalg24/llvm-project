//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef TEST_STD_CONTAINERS_MINIMAL_ELEMENT_TYPE_H
#define TEST_STD_CONTAINERS_MINIMAL_ELEMENT_TYPE_H

struct MinimalElementType {
  int val;
  constexpr MinimalElementType() = delete;
  constexpr MinimalElementType(const MinimalElementType&) = delete;
  constexpr explicit MinimalElementType(int v) noexcept : val(v){};
  constexpr MinimalElementType& operator=(const MinimalElementType&) = delete;
};

#endif // TEST_STD_CONTAINERS_MINIMAL_ELEMENT_TYPE_H
