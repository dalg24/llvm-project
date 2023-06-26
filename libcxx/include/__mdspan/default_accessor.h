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

#ifndef _LIBCPP___MDSPAN_DEFAULT_ACCESSOR_H
#define _LIBCPP___MDSPAN_DEFAULT_ACCESSOR_H

#include <__config>
#include <__type_traits/is_convertible.h>
#include <cinttypes>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_PUSH_MACROS
#include <__undef_macros>

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER >= 23

template<class ElementType>
struct default_accessor {
  using offset_policy = default_accessor;
  using element_type = ElementType;
  using reference = ElementType&;
  using data_handle_type = ElementType*;

  _LIBCPP_HIDE_FROM_ABI constexpr default_accessor() noexcept = default;
  template<class OtherElementType>
    requires(is_convertible_v<OtherElementType(*)[], element_type(*)[]>)
  _LIBCPP_HIDE_FROM_ABI constexpr default_accessor(default_accessor<OtherElementType>) noexcept {}
  _LIBCPP_HIDE_FROM_ABI constexpr reference access(data_handle_type p, size_t i) const noexcept { return p[i]; }
  _LIBCPP_HIDE_FROM_ABI constexpr data_handle_type offset(data_handle_type p, size_t i) const noexcept { return p+i; }
};

#endif // _LIBCPP_STD_VER >= 23

_LIBCPP_END_NAMESPACE_STD

_LIBCPP_POP_MACROS

#endif // _LIBCPP___MDSPAN_DEFAULT_ACCESSOR_H
