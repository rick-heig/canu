
/******************************************************************************
 *
 *  This file is part of canu, a software program that assembles whole-genome
 *  sequencing reads into contigs.
 *
 *  This software is based on:
 *    'Celera Assembler' (http://wgs-assembler.sourceforge.net)
 *    the 'kmer package' (http://kmer.sourceforge.net)
 *  both originally distributed by Applera Corporation under the GNU General
 *  Public License, version 2.
 *
 *  Canu branched from Celera Assembler at its revision 4587.
 *  Canu branched from the kmer project at its revision 1994.
 *
 *  Modifications by:
 *
 *  File 'README.licenses' in the root directory of this distribution contains
 *  full conditions and disclaimers for each license.
 */

#include "positionDB.H"


void
positionDB::dump(char *name) {
  uint64  sizs[4] = {_chckWidth, _pptrWidth, 1, _sizeWidth};
  uint64  vals[4] = {0, 0, 0, 0};
  FILE   *F = fopen(name, "w");

  for (uint64 h=0; h<_tableSizeInEntries; h++) {
    uint64 st, ed;

    if (_hashTable_BP) {
      st = getDecodedValue(_hashTable_BP, h * _hashWidth,              _hashWidth);
      ed = getDecodedValue(_hashTable_BP, h * _hashWidth + _hashWidth, _hashWidth);
    } else {
      st = _hashTable_FW[h];
      ed = _hashTable_FW[h+1];
    }

    fprintf(F, "B "F_U64" "F_U64"-"F_U64"\n", h, st, ed);

    while (st < ed) {
      uint64     cb = st * _wFin;

      getDecodedValues(_buckets, cb, (_sizeWidth == 0) ? 3 : 4, sizs, vals);

      fprintf(F, "%c chk="F_X64" pos="F_U64" siz="F_U64,
              (vals[2] == 0) ? 'D' : 'U', vals[0], vals[1], vals[3]);

      if (vals[2] == 0) {
        uint64 pos = vals[1] * _posnWidth;
        uint64 len = getDecodedValue(_positions, pos, _posnWidth);

        for (pos += _posnWidth; len > 0; pos += _posnWidth, len--)
          fprintf(F, " "F_U64, getDecodedValue(_positions, pos, _posnWidth));
      }

      fprintf(F, "\n");

      st++;
    }
  }

  fclose(F);
}
