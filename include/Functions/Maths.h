#include "Data/GlobalDefinitions.h"

#ifndef MATHS_H
#define MATHS_H

u32 ToDecimal(u32 hexValue);
s32 Sign(s32 value);
u32 Absolute(s32 value);
u32 ToHex(u32 decimalValue);
u32 UnsignedDivide(u32 numerator, u32 denominator);
u32 UnsignedModulus(u32 numerator, u32 denominator);
s32 SignedDivide(s32 numerator, s32 denominator);
u32 UnsignedFractionalMultiplication(u32 numerator, u32 multiplier);
s32 SignedFractionalMultiplication(s32 numerator, s32 multiplier);
void UpdateRandomValue();
u32 GetRandom32BitValue();
u16 GetRandom16BitValue();
u32 GetDelimitedRandom32BitValue(u32 upperLimit);
u16 GetDelimitedRandom16BitValue(u32 upperLimit);
u32 WeightedRandom(u8* probabilities, u8 length);
void SeedRNG(u32 seed);
void ReseedRNG();

#endif