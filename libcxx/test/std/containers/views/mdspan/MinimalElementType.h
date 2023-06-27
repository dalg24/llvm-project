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

template<class T>
struct MinimalElementTypeDataHelper {
  T ptr[128];
  constexpr MinimalElementTypeDataHelper() = default;
  constexpr T* get_ptr() { return ptr; }
};

template<>
struct MinimalElementTypeDataHelper<MinimalElementType> {
  MinimalElementType* ptr;
  char data_ptr[512];
  MinimalElementTypeDataHelper() {
      ptr = reinterpret_cast<MinimalElementType*>(data_ptr);
  }
  constexpr MinimalElementType* get_ptr() { return ptr; }
};

#endif // TEST_STD_CONTAINERS_MINIMAL_ELEMENT_TYPE_H
