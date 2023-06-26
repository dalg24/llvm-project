//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <mdspan>

// constexpr mapping(const extents_type&) noexcept;
//
// Preconditions: The size of the multidimensional index space e is representable
//                as a value of type index_type ([basic.fundamental]).
//
// Effects: Direct-non-list-initializes extents_ with e.

#include <mdspan>
#include <cassert>
#include <cstdint>

#include "test_macros.h"

template <class E>
constexpr void test_construction(E e, typename E::index_type expected_size) {
  using M = std::layout_left::mapping<E>;
  const M m(e);

  // check required_span_size()
  ASSERT_NOEXCEPT(m.required_span_size());
  assert(m.required_span_size() == expected_size);
}

constexpr bool test() {
  constexpr size_t D = std::dynamic_extent;
  test_construction(std::extents<int>(), 1);
  test_construction(std::extents<unsigned, D>(0), 0);
  test_construction(std::extents<unsigned, D>(1), 1);
  test_construction(std::extents<unsigned, D>(7), 7);
  test_construction(std::extents<unsigned, 7>(), 7);
  test_construction(std::extents<unsigned, 7, 8>(), 56);
  test_construction(std::extents<int64_t, D, 8, D, D>(7, 9, 10), 5040);
  test_construction(std::extents<int64_t, 1, 8, D, D>(9, 10), 720);
  test_construction(std::extents<int64_t, 1, 0, D, D>(9, 10), 0);
  return true;
}

int main(int, char**) {
  test();
  static_assert(test());
  return 0;
}
