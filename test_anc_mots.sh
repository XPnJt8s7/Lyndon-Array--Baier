#!/usr/bin/env bash

mots=(graindraining tgtggagggg gaggctcgcg tgtttttctg gacgaggcgg acacggaacc acgaccccaa)

for mot in ${mots[*]}
do
  valgrind ./newLA_test $mot
    # printf "   %s\n" $mot
done
