// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//                        Kokkos v. 4.0
//       Copyright (2022) National Technology & Engineering
//               Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
//===---------------------------------------------------------------------===//

#ifndef _LIBCPP___MDSPAN_LAYOUT_RIGHT_MAPPING_H
#define _LIBCPP___MDSPAN_LAYOUT_RIGHT_MAPPING_H

#include <__assert>
#include <__config>
#include <__mdspan/extents.h>
#include <__mdspan/layouts.h>
#include <__type_traits/is_constructible.h>
#include <__type_traits/is_convertible.h>
#include <__type_traits/is_nothrow_constructible.h>
#include <__utility/integer_sequence.h>
#include <__utility/unreachable.h>
#include <cinttypes>
#include <cstddef>
#include <limits>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER >= 23

template <class _Extents>
class layout_right::mapping {
  static_assert(__mdspan_detail::__is_extents<_Extents>::value,
                "layout_right::mapping template argument must be a specialization of extents.");

public:
  using extents_type = _Extents;
  using index_type   = typename extents_type::index_type;
  using size_type    = typename extents_type::size_type;
  using rank_type    = typename extents_type::rank_type;
  using layout_type  = layout_right;

private:
  _LIBCPP_HIDE_FROM_ABI static constexpr bool __required_span_size_is_representable(extents_type __ext) {
    if constexpr (extents_type::rank() == 0)
      return true;

    constexpr index_type __num_max = numeric_limits<index_type>::max();
    index_type __prod              = __ext.extent(0);
    for (rank_type __r = 1; __r < extents_type::rank(); __r++) {
      if (__prod > (__num_max / __ext.extent(__r)))
        return false;
      __prod *= __ext.extent(__r);
    }
    return true;
  }

  static_assert((extents_type::rank_dynamic() > 0) || __required_span_size_is_representable(extents_type()),
                "layout_right::mapping product of static extents must be representable as index_type.");

public:
  // [mdspan.layout.right.cons], constructors
  _LIBCPP_HIDE_FROM_ABI constexpr mapping() noexcept               = default;
  _LIBCPP_HIDE_FROM_ABI constexpr mapping(const mapping&) noexcept = default;
  _LIBCPP_HIDE_FROM_ABI constexpr mapping(const extents_type& __ext) noexcept : __extents_(__ext) {
    _LIBCPP_ASSERT(__required_span_size_is_representable(__ext),
                   "layout_right::mapping extents ctor: product of extents must be representable as index_type.");
  }

  template <class _OtherExtents>
    requires(is_constructible_v<extents_type, _OtherExtents>)
  _LIBCPP_HIDE_FROM_ABI constexpr explicit(!is_convertible_v<_OtherExtents, extents_type>)
      mapping(const mapping<_OtherExtents>& __other) noexcept
      : __extents_(__other.extents()) {
    _LIBCPP_ASSERT(
        __mdspan_detail::__is_representable_as<index_type>(__other.required_span_size()),
        "layout_right::mapping converting ctor: other.required_span_size() must be representable as index_type.");
  }

// FIXME: add when we add other layouts
#  if 0
    template<class _OtherExtents>
      constexpr explicit(!is_convertible_v<_OtherExtents, extents_type>)
        mapping(const layout_left::mapping<_OtherExtents>&) noexcept {}

    template<class _OtherExtents>
      constexpr explicit(extents_type::rank() > 0)
        mapping(const layout_stride::mapping_<OtherExtents>&) noexcept;
#  endif

  _LIBCPP_HIDE_FROM_ABI constexpr mapping& operator=(const mapping&) noexcept = default;

  // [mdspan.layout.right.obs], observers
  _LIBCPP_HIDE_FROM_ABI constexpr const extents_type& extents() const noexcept { return __extents_; }

  _LIBCPP_HIDE_FROM_ABI constexpr index_type required_span_size() const noexcept {
    index_type __size = 1;
    for (size_t __r = 0; __r < extents_type::rank(); __r++)
      __size *= __extents_.extent(__r);
    return __size;
  }

private:
  // Helper functions to compute the offset
  // 3D: ((i0 * E1 + i1)*E2 + i2)*E3 + i3
  // This is not directly doable as a fold expression, so use recursive functions here

  // Helper struct to count recursion depth and when to stop
  template <size_t _Rp, size_t _Rank>
  struct __rank_count {};

  // don't start: Rank-0
  _LIBCPP_HIDE_FROM_ABI constexpr index_type __compute_offset(__rank_count<0, 0>) const { return 0; }

  // start of recursion
  template <class _Ip, class... _Indices>
  _LIBCPP_HIDE_FROM_ABI constexpr index_type
  __compute_offset(__rank_count<0, extents_type::rank()>, const _Ip& __i, _Indices... __idx) const {
    return __compute_offset(__i, __rank_count<1, extents_type::rank()>(), __idx...);
  }

  // continue recursion
  template <size_t _Rp, size_t _Rank, class _Ip, class... _Indices>
  _LIBCPP_HIDE_FROM_ABI constexpr index_type
  __compute_offset(index_type __offset, __rank_count<_Rp, _Rank>, const _Ip& __i, _Indices... __idx) const {
    return __compute_offset(__offset * __extents_.extent(_Rp) + __i, __rank_count<_Rp + 1, _Rank>(), __idx...);
  }

  // end of recursion
  _LIBCPP_HIDE_FROM_ABI constexpr index_type
  __compute_offset(index_type __offset, __rank_count<extents_type::rank(), extents_type::rank()>) const {
    return static_cast<index_type>(__offset);
  }

public:
  template <class... _Indices>
    requires((sizeof...(_Indices) == extents_type::rank()) && (is_convertible_v<_Indices, index_type> && ...) &&
             (is_nothrow_constructible_v<index_type, _Indices> && ...))
  _LIBCPP_HIDE_FROM_ABI constexpr index_type operator()(_Indices... __idx) const noexcept {
    _LIBCPP_ASSERT(__mdspan_detail::__is_multidimensional_index_in(__extents_, __idx...),
                   "layout_right::mapping: out of bounds indexing");
    return __compute_offset(__rank_count<0, extents_type::rank()>(), static_cast<index_type>(__idx)...);
  }

  _LIBCPP_HIDE_FROM_ABI static constexpr bool is_always_unique() noexcept { return true; }
  _LIBCPP_HIDE_FROM_ABI static constexpr bool is_always_exhaustive() noexcept { return true; }
  _LIBCPP_HIDE_FROM_ABI static constexpr bool is_always_strided() noexcept { return true; }

  _LIBCPP_HIDE_FROM_ABI static constexpr bool is_unique() noexcept { return true; }
  _LIBCPP_HIDE_FROM_ABI static constexpr bool is_exhaustive() noexcept { return true; }
  _LIBCPP_HIDE_FROM_ABI static constexpr bool is_strided() noexcept { return true; }

  _LIBCPP_HIDE_FROM_ABI constexpr index_type stride(rank_type __r) const noexcept
    requires(extents_type::rank() > 0)
  {
    _LIBCPP_ASSERT(__r < extents_type::rank(), "layout_right::mapping::stride(): invalid rank index");
    if constexpr (extents_type::rank() > 0) {
      index_type __s = 1;
      for (rank_type __i = extents_type::rank() - 1; __i > __r; __i--)
        __s *= __extents_.extent(__i);
      return __s;
    }
    __libcpp_unreachable();
  }

  template <class _OtherExtents>
    requires(_OtherExtents::rank() == extents_type::rank())
  _LIBCPP_HIDE_FROM_ABI friend constexpr bool
  operator==(const mapping& __lhs, const mapping<_OtherExtents>& __rhs) noexcept {
    return __lhs.extents() == __rhs.extents();
  }

private:
  extents_type __extents_{}; // exposition only
};

#endif // _LIBCPP_STD_VER >= 23

_LIBCPP_END_NAMESPACE_STD

_LIBCPP_POP_MACROS

#endif // _LIBCPP___MDSPAN_LAYOUT_RIGHT_MAPPING_H
