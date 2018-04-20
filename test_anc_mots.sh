#!/usr/bin/env bash

mots=(graindraining tgtggagggg gaggctcgcg tgtttttctg gacgaggcgg acacggaacc acgaccccaa aeeeaaeabceeedee)

for mot in ${mots[*]}
do
  valgrind ./newLA_test $mot
    # printf "   %s\n" $mot
done
