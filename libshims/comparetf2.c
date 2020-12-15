//===-- lib/comparetf2.c - Quad-precision comparisons -------------*- C -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <limits.h>

typedef __uint128_t rep_t;
typedef __int128_t srep_t;
typedef long double fp_t;

#define REP_C (__uint128_t)
#define typeWidth (sizeof(rep_t) * CHAR_BIT)
#define significandBits 112
#define exponentBits (typeWidth - significandBits - 1)

#define implicitBit (REP_C(1) << significandBits)
#define significandMask (implicitBit - 1U)
#define signBit (REP_C(1) << (significandBits + exponentBits))
#define absMask (signBit - 1U)
#define exponentMask (absMask ^ significandMask)
#define infRep exponentMask

static __inline rep_t toRep(fp_t x) {
  const union {
    fp_t f;
    rep_t i;
  } rep = {.f = x};
  return rep.i;
}

enum LE_RESULT { LE_LESS = -1, LE_EQUAL = 0, LE_GREATER = 1, LE_UNORDERED = 1 };

enum LE_RESULT __lttf2(fp_t a, fp_t b) {

  const srep_t aInt = toRep(a);
  const srep_t bInt = toRep(b);
  const rep_t aAbs = aInt & absMask;
  const rep_t bAbs = bInt & absMask;

  // If either a or b is NaN, they are unordered.
  if (aAbs > infRep || bAbs > infRep)
    return LE_UNORDERED;

  // If a and b are both zeros, they are equal.
  if ((aAbs | bAbs) == 0)
    return LE_EQUAL;

  // If at least one of a and b is positive, we get the same result comparing
  // a and b as signed integers as we would with a floating-point compare.
  if ((aInt & bInt) >= 0) {
    if (aInt < bInt)
      return LE_LESS;
    else if (aInt == bInt)
      return LE_EQUAL;
    else
      return LE_GREATER;
  } else {
    // Otherwise, both are negative, so we need to flip the sense of the
    // comparison to get the correct result.  (This assumes a twos- or ones-
    // complement integer representation; if integers are represented in a
    // sign-magnitude representation, then this flip is incorrect).
    if (aInt > bInt)
      return LE_LESS;
    else if (aInt == bInt)
      return LE_EQUAL;
    else
      return LE_GREATER;
  }
}
