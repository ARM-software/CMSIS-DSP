import math
import argparse
from sympy.combinatorics import Permutation

def bits_for_value(value):
  return int(math.log2(value))

def decompose(N, R):
  logN2 = bits_for_value(N)
  logR2 = []

  while (N >= R):
    logR2.append(bits_for_value(R))
    N = N // R

  if (N > 1):
    logR2.append(bits_for_value(N))

  return (logN2, logR2)

def reverse_bits(x, n, bits_list):
  result = 0
  for bits in bits_list:
    mask = (0xffffffff >> (32 - bits))
    result = (result << bits) | (x & mask)
    x = x >> bits
  return result

def create_transpositions(N, R):
  (logN2, logR2) = decompose(N, R)

  indexes = []
  for n in range(N):
    indexes.append(reverse_bits(n, logN2, logR2))

  # Create transpositions table
  tps = []
  for c in Permutation.from_sequence(indexes).cyclic_form:
    for i in range(len(c) - 1):
      tps.append([c[i] * 8, c[-1] * 8])

  return tps

def transpositions_stringify(N, R, tps,fixed):
  MAX_LINE_LEN   = 79
  MAX_FFT_IN_U16 = 4096

  index_type = 'uint16_t' if N <= MAX_FFT_IN_U16 else 'uint32_t'
  tps_elements_count = len(tps) * 2

  if fixed:
     out = '#define ARMBITREVINDEXTABLE_FIXED_{}_TABLE_LENGTH {}\n'.format(N, tps_elements_count)
     out += 'const {} armBitRevIndexTable_fixed_{}[ARMBITREVINDEXTABLE_FIXED_{}_TABLE_LENGTH] = {{\n'.format(index_type, N, N)
  else:
     out = '#define ARMBITREVINDEXTABLE_{}_TABLE_LENGTH {}\n'.format(N, tps_elements_count)
     out += 'const {} armBitRevIndexTable{}[ARMBITREVINDEXTABLE_{}_TABLE_LENGTH] = {{\n'.format(index_type, N, N)
 
  line = ''
  for tp in tps:
    entry = '{},{}'.format(tp[0], tp[1])

    # Append to line
    exp_line_len = len(line) + len(entry) + len(', ,')

    if (line == ''):
      line = '   ' + entry
    elif (exp_line_len >= MAX_LINE_LEN):
      out += line + ',\n'
      line = '   ' + entry
    else:
      line += ', ' + entry

  out += line + '\n};'
  return out

parser = argparse.ArgumentParser(description='Generate bits reversal tables',
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)

parser.add_argument('filename', metavar='out.c', nargs='?', help='output file name')
parser.add_argument('--size',  type=int, default=8192, help='size')
parser.add_argument('--radix', type=int, default=8, choices=[2, 8],
                               help='radix | use 2 for Radix 4 and 4x2 | use 8 for Radix 8, 8x4, 8x2')
parser.add_argument('-f', action='store_true', help="Fixed point table")

args = parser.parse_args()

tps = create_transpositions(args.size, args.radix)
out = transpositions_stringify(args.size, args.radix, tps,args.f)

if (args.filename == None):
  print(out)
else:
  f = open(args.filename, 'w')
  f.write(out)
  f.close()