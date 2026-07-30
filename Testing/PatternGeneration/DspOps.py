import os.path
import numpy as np
import itertools
import Tools

def sat_q63(r):
    if (r > 0x07FFF_FFFF_FFFF_FFFF):
      r = 0x07FFF_FFFF_FFFF_FFFF
    if (r < -0x0800_0000_0000_0000):
      r = -0x0800_0000_0000_0000
    return r

def sat_q31(r):
    if (r > 0x7FFF_FFFF):
      r = 0x7FFF_FFFF
    if (r < -0x8000_0000):
      r = -0x8000_0000
    return r

def sat_q15(r):
    if (r > 0x07FFF):
      r = 0x07FFF
    if (r < -0x08000):
      r = -0x08000
    return r

def sat_q7(r):
    if (r > 0x07F):
      r = 0x07F
    if (r < -0x080):
      r = -0x080
    return r

def q15_from_bits(r):
    # SMLALD treats each 16-bit lane as a signed two's-complement value.
    r = int(r) & 0xFFFF
    return r - 0x10000 if r & 0x8000 else r

def smlald_ref(x, y, acc):
    # Model the Cortex-M DSP SMLALD instruction semantics: multiply the
    # corresponding signed bottom and top halfwords, then add both products
    # to the 64-bit accumulator.  This architectural model is intentionally
    # independent of the C fallback implementation under test.
    x = int(x)
    y = int(y)
    acc = int(acc)
    return (q15_from_bits(x) * q15_from_bits(y) +
            q15_from_bits(x >> 16) * q15_from_bits(y >> 16) +
            acc)

def smlaldx_ref(x, y, acc):
    # SMLALDX exchanges the halfwords of the second operand before applying
    # the same signed dual multiply-accumulate operation as SMLALD.
    x = int(x)
    y = int(y)
    acc = int(acc)
    return (q15_from_bits(x) * q15_from_bits(y >> 16) +
            q15_from_bits(x >> 16) * q15_from_bits(y) +
            acc)

def signed_from_bits(value, bits):
    value = int(value) & ((1 << bits) - 1)
    sign = 1 << (bits - 1)
    return value - (1 << bits) if value & sign else value

def wrap_signed(value, bits):
    return signed_from_bits(value, bits)

def packed_lanes(value, bits):
    return [signed_from_bits(int(value) >> shift, bits)
            for shift in range(0, 32, bits)]

def pack_lanes(values, bits):
    mask = (1 << bits) - 1
    result = 0
    for index, value in enumerate(values):
        result |= (int(value) & mask) << (index * bits)
    return wrap_signed(result, 32)

def packed_binary_ref(x, y, bits, operation, saturate_result=False):
    result = []
    for a, b in zip(packed_lanes(x, bits), packed_lanes(y, bits)):
        value = operation(a, b)
        if saturate_result:
            value = max(-(1 << (bits - 1)),
                        min((1 << (bits - 1)) - 1, value))
        result.append(value)
    return pack_lanes(result, bits)

def packed_cross_ref(x, y, operation0, operation1, halve=False,
                     saturate_result=False):
    a0, a1 = packed_lanes(x, 16)
    b0, b1 = packed_lanes(y, 16)
    values = [operation0(a0, b1), operation1(a1, b0)]
    if halve:
        values = [value >> 1 for value in values]
    if saturate_result:
        values = [max(-0x8000, min(0x7FFF, value)) for value in values]
    return pack_lanes(values, 16)

def clz_ref(value):
    value = int(value) & 0xFFFFFFFF
    return 32 if value == 0 else 32 - value.bit_length()

def ssat_ref(value, sat):
    value = int(value)
    sat = int(sat) & 0xFFFFFFFF
    if 1 <= sat <= 32:
        return max(-(1 << (sat - 1)), min((1 << (sat - 1)) - 1, value))
    return value

def usat_ref(value, sat):
    value = int(value)
    sat = int(sat) & 0xFFFFFFFF
    if sat <= 31:
        return max(0, min((1 << sat) - 1, value))
    return wrap_signed(value, 32)

def ror_ref(value, shift):
    value = int(value) & 0xFFFFFFFF
    shift = int(shift) & 31
    if shift:
        value = (value >> shift) | ((value << (32 - shift)) & 0xFFFFFFFF)
    return wrap_signed(value, 32)

def mult32x64_ref(x, y):
    x = int(x)
    y = int(y)
    return wrap_signed((((x & 0xFFFFFFFF) * y) >> 32) +
                       ((x >> 32) * y), 64)

def reciprocal_ref(value, bits):
    reciprocal = float(1 << (bits - 1)) / int(value)
    shift = 0
    while reciprocal > 1.0:
        reciprocal /= 2.0
        shift += 1
    scaled = int(round(reciprocal * (1 << (bits - 1))))
    scaled = min((1 << (bits - 1)) - 1, scaled)
    return scaled, shift

def norm_64_to_32u_ref(value):
    value = int(value) & 0xFFFFFFFFFFFFFFFF
    if value == 0:
        return 0, 0
    norm = 31 - value.bit_length()
    normalized = value << norm if norm >= 0 else value >> -norm
    return wrap_signed(normalized, 32), norm

def c_div_ref(num, den):
    num = int(num)
    den = int(den)
    if num == -0x8000000000000000 and den == -1:
        return 0x7FFFFFFF
    quotient = abs(num) // abs(den)
    if (num < 0) != (den < 0):
        quotient = -quotient
    return sat_q31(quotient)


def writeTests(config,format):
    inputAQ31 = np.array([
        0x7FFF_FFFF, 0x4CCC_CCCD, 0x4000_0000, 0x2666_6666,
        0x2000_0000, 0x0CCC_CCCD, 0x0000_0001, 0x0000_0000,
        0xFFFF_FFFF, 0x8000_0000, 0x7FFF_7FFF, 0x8000_8000,
        0x8000_7FFF, 0x0001_0002, 0xFFFF_0001, 0x0001_0001,
        0x4000_C000, 0x1234_FEDC, 0x7F80_7F80, 0x8080_8080,
        0x7F7F_7F7F, 0x0102_0304, 0xFF00_AA55, 0x7FFF_FFFE,
        0x8000_0001, 0xC000_0000, 0x0000_7FFF, 0x0000_8000,
        0xFFFF_7FFF, 0xFFFF_8000, 0x0001_0010, 0xFFFF_007F
    ], dtype=np.uint32).view(np.int32)

    inputBQ31 = np.array([
        0x0000_0001, 0x8000_8000, 0x7FFF_7FFF, 0x7FFF_7FFF,
        0xFFFF_FFFF, 0x7FFF_8000, 0x0003_0004, 0x0001_FFFF,
        0x0001_0001, 0xFFFF_FFFF, 0x4000_C000, 0xFEDC_1234,
        0x0000_0002, 0x7FFF_FFFF, 0x8000_0000, 0x0101_0101,
        0x7F7F_7F7F, 0x8080_8080, 0x7F80_807F, 0x00FF_55AA,
        0x0000_001F, 0x0000_0020, 0x0000_0008, 0x0000_0010,
        0x0000_0002, 0xFFFF_FFFE, 0x0001_0001, 0xFFFF_0001,
        0x1234_5678, 0x8765_4321, 0x0000_0003, 0x7FFF_FFFE
    ], dtype=np.uint32).view(np.int32)

    inputCQ31 = np.array([
        1, 8, 16, 31, 32, 1, 8, 16, 31, 32, 1, 8, 16, 31, 32, 1,
        0x7FFF_FFFF, 0x8000_0000, 0xFFFF_FFFF, 5,
        0x4000_0000, 0xC000_0000, 0x0001_0000, 0xFFFF_0000,
        6, 9, 13, 17, 21, 25, 29, 10
    ], dtype=np.uint32).view(np.int32)

    inputAQ63 = np.array([
        0xFFFF_FFFF_FFFF_FFFF, 0, 1, 0x7FFF_FFFF_FFFF_FFFF,
        0x8000_0000_0000_0000, 0xFFFF_FFFE_FFFF_FFFF,
        0xFFFF_FFFF_8000_0000, 0xFFFF_FFFF_8000_0001,
        0x0000_0001_1000_0000, 0xFFFF_FFFF_7FFF_FFFF,
        0x0000_0000_7FFF_FFFE, 0x0000_0000_7FFF_FFFF,
        0x0000_0000_8000_0000, 0x0000_0000_FFFF_FFFF,
        0x0000_0000_7FFF_0000, 0xFFFF_FFFF_7FFF_0000,
        0x1000_0000_8000_0000, 0x0000_0000_2000_0000,
        0x4000_0000_0000_0000, 0xC000_0000_0000_0000,
        0x2000_0000_0000_0000, 0xE000_0000_0000_0000,
        0x0000_0000_4000_0000, 0xFFFF_FFFF_C000_0000,
        0x0000_0001_0000_0000, 0xFFFF_FFFF_0000_0000,
        0x0123_4567_89AB_CDEF, 0xFEDC_BA98_7654_3210,
        0x0000_0000_0001_0000, 0xFFFF_FFFF_FFFF_0000,
        0x0000_0000_0000_0002, 0x7FFF_FFFF_0000_0000
    ], dtype=np.uint64).view(np.int64)

    inputBQ63 = np.array([
        0x0000_0000_0000_0000, 0x0000_0000_0000_0001,
        0x0000_0000_0000_0002, 0x0000_0000_2000_0000,
        0x0000_0000_4000_0000, 0x0000_0000_7FFF_FFFF,
        0x0000_0000_8000_0000, 0x0000_0000_FFFF_FFFF,
        0x0000_0001_0000_0000, 0x1000_0000_8000_0000,
        0x4000_0000_0000_0000, 0x7FFF_FFFF_FFFF_FFFE,
        0x7FFF_FFFF_FFFF_FFFF
    ], dtype=np.uint64).view(np.int64)

    inputAQ15 = np.array([0x7FFF, 0x4CCD, 0x4000, 0x2666,
                          0x2000, 0x0CCD, 0x0001], dtype=np.int16)

    a = [int(value) for value in inputAQ31]
    b = [int(value) for value in inputBQ31]
    c = [int(value) for value in inputCQ31]
    a64 = [int(value) for value in inputAQ63]

    config.writeInputS32(1, inputAQ31, "Input")
    config.writeInputS32(2, inputBQ31, "Input")
    config.writeInputS32(3, inputCQ31, "Input")
    config.writeInputS64(1, inputAQ63, "Input")
    config.writeInputS64(2, inputBQ63, "Input")
    config.writeInputS16(1, inputAQ15, "Input")

    config.writeReferenceS32(1, np.array([sat_q31(x) for x in a64], dtype=np.int32), "Ref")
    config.writeReferenceS16(1, np.array([sat_q15(x) for x in a], dtype=np.int16), "Ref")
    config.writeReferenceS64(2, np.array([wrap_signed(smlald_ref(x, y, z), 64)
                                          for x, y, z in zip(a, b, a64)], dtype=np.int64), "Ref")
    config.writeReferenceS64(3, np.array([wrap_signed(smlaldx_ref(x, y, z), 64)
                                          for x, y, z in zip(a, b, a64)], dtype=np.int64), "Ref")

    q31_refs = [
        (2, [clz_ref(x) for x in a]),
        (3, [ssat_ref(x, z) for x, z in zip(a, c)]),
        (4, [usat_ref(x, z) for x, z in zip(a, c)]),
        (5, [ror_ref(x, y) for x, y in zip(a, b)]),
        (6, [packed_binary_ref(x, y, 8, lambda u, v: u + v, True) for x, y in zip(a, b)]),
        (7, [packed_binary_ref(x, y, 8, lambda u, v: u - v, True) for x, y in zip(a, b)]),
        (8, [packed_binary_ref(x, y, 16, lambda u, v: u + v, True) for x, y in zip(a, b)]),
        (9, [packed_binary_ref(x, y, 16, lambda u, v: (u + v) >> 1) for x, y in zip(a, b)]),
        (10, [packed_binary_ref(x, y, 16, lambda u, v: u - v, True) for x, y in zip(a, b)]),
        (11, [packed_binary_ref(x, y, 16, lambda u, v: (u - v) >> 1) for x, y in zip(a, b)]),
        (12, [packed_cross_ref(x, y, lambda u, v: u - v, lambda u, v: u + v,
                               saturate_result=True) for x, y in zip(a, b)]),
        (13, [packed_cross_ref(x, y, lambda u, v: u - v, lambda u, v: u + v,
                               halve=True) for x, y in zip(a, b)]),
        (14, [packed_cross_ref(x, y, lambda u, v: u + v, lambda u, v: u - v,
                               saturate_result=True) for x, y in zip(a, b)]),
        (15, [packed_cross_ref(x, y, lambda u, v: u + v, lambda u, v: u - v,
                               halve=True) for x, y in zip(a, b)]),
        (16, [wrap_signed(packed_lanes(x, 16)[0] * packed_lanes(y, 16)[1] -
                          packed_lanes(x, 16)[1] * packed_lanes(y, 16)[0], 32)
              for x, y in zip(a, b)]),
        (17, [wrap_signed(packed_lanes(x, 16)[0] * packed_lanes(y, 16)[1] +
                          packed_lanes(x, 16)[1] * packed_lanes(y, 16)[0], 32)
              for x, y in zip(a, b)]),
        (18, [sat_q31(x + y) for x, y in zip(a, b)]),
        (19, [sat_q31(x - y) for x, y in zip(a, b)]),
        (20, [wrap_signed(smlald_ref(x, y, z), 32) for x, y, z in zip(a, b, c)]),
        (21, [wrap_signed(smlaldx_ref(x, y, z), 32) for x, y, z in zip(a, b, c)]),
        (22, [wrap_signed(packed_lanes(x, 16)[0] * packed_lanes(y, 16)[1] -
                          packed_lanes(x, 16)[1] * packed_lanes(y, 16)[0] + z, 32)
              for x, y, z in zip(a, b, c)]),
        (23, [wrap_signed(packed_lanes(x, 16)[0] * packed_lanes(y, 16)[0] +
                          packed_lanes(x, 16)[1] * packed_lanes(y, 16)[1], 32)
              for x, y in zip(a, b)]),
        (24, [wrap_signed(packed_lanes(x, 16)[0] * packed_lanes(y, 16)[0] -
                          packed_lanes(x, 16)[1] * packed_lanes(y, 16)[1], 32)
              for x, y in zip(a, b)]),
        (25, [pack_lanes([signed_from_bits(x, 8), signed_from_bits(x >> 16, 8)], 16)
              for x in a]),
        (26, [wrap_signed(z + ((x * y) >> 32), 32) for x, y, z in zip(a, b, c)])
    ]
    for ref_id, values in q31_refs:
        config.writeReferenceS32(ref_id, np.array(values, dtype=np.int32), "Ref")

    config.writeReferenceS64(4, np.array([mult32x64_ref(x, y)
                                          for x, y in zip(a64, a)], dtype=np.int64), "Ref")

    recip_q31 = [reciprocal_ref(value, 32) for value in a[:7]]
    recip_q15 = [reciprocal_ref(value, 16) for value in inputAQ15]
    config.writeReferenceS32(27, np.array([value for value, _ in recip_q31], dtype=np.int32), "Ref")
    config.writeReferenceS16(2, np.array([shift for _, shift in recip_q31], dtype=np.int16), "Ref")
    config.writeReferenceS16(3, np.array([value for value, _ in recip_q15], dtype=np.int16), "Ref")
    config.writeReferenceS16(4, np.array([shift for _, shift in recip_q15], dtype=np.int16), "Ref")

    norm_refs = [norm_64_to_32u_ref(value) for value in inputBQ63]
    config.writeReferenceS32(28, np.array([value for value, _ in norm_refs], dtype=np.int32), "Ref")
    config.writeReferenceS32(29, np.array([norm for _, norm in norm_refs], dtype=np.int32), "Ref")
    config.writeReferenceS32(30, np.array([c_div_ref(x, y) for x, y in zip(a64, b)], dtype=np.int32), "Ref")

    # The accumulating macros form a signed 64-bit intermediate. Use the
    # prefix where input C is deliberately small so the reference exercises
    # rounding and signs without relying on signed-overflow behavior.
    accumulator_samples = 16
    config.writeReferenceS32(31, np.array([
        wrap_signed(((z << 32) + x * y + 0x80000000) >> 32, 32)
        for x, y, z in zip(a[:accumulator_samples],
                           b[:accumulator_samples],
                           c[:accumulator_samples])], dtype=np.int32), "Ref")
    config.writeReferenceS32(32, np.array([
        wrap_signed(((z << 32) - x * y + 0x80000000) >> 32, 32)
        for x, y, z in zip(a[:accumulator_samples],
                           b[:accumulator_samples],
                           c[:accumulator_samples])], dtype=np.int32), "Ref")
    config.writeReferenceS32(33, np.array([
        wrap_signed((x * y + 0x80000000) >> 32, 32)
        for x, y in zip(a, b)], dtype=np.int32), "Ref")
    config.writeReferenceS32(34, np.array([
        wrap_signed(z + ((x * y) >> 32), 32)
        for x, y, z in zip(a[:accumulator_samples],
                           b[:accumulator_samples],
                           c[:accumulator_samples])], dtype=np.int32), "Ref")
    config.writeReferenceS32(35, np.array([
        wrap_signed(z - ((x * y) >> 32), 32)
        for x, y, z in zip(a[:accumulator_samples],
                           b[:accumulator_samples],
                           c[:accumulator_samples])], dtype=np.int32), "Ref")
    config.writeReferenceS32(36, np.array([
        wrap_signed((x * y) >> 32, 32)
        for x, y in zip(a, b)], dtype=np.int32), "Ref")


def generatePatterns():
    PATTERNDIR = os.path.join("Patterns","DSP","DspOps","DspOps")
    PARAMDIR = os.path.join("Parameters","DSP","DspOps","DspOps")
    
    config=Tools.Config(PATTERNDIR,PARAMDIR,"")


    config.setOverwrite(True)
    writeTests(config,7)


if __name__ == '__main__':
  generatePatterns()
