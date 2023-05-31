//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// REQUIRES: has-unix-headers
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <mdspan>

// template<class Extents>
// class layout_left::mapping;

// If Extents is not a specialization of extents, then the program is ill-formed.

#include <mdspan>

void not_extents() {
  // expected-error-re@*:* {{{{(static_assert|static assertion)}} failed {{.*}}layout_right::mapping template argument must be a specialization of extents}}
  [[maybe_unused]] std::layout_right::mapping<void> mapping;
}
