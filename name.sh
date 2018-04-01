#!/usr/bin/env bash

for taille_texte in 1 5 10 50 100 200
do
  for taille_alph in 2 4 10 20 50 70
  do
      echo "moyenne,algo3,decrement_group_count,order_suffs,group_moyenne,group_max,group_min" >> plot_${taille_texte}_${taille_alph}.csv
  done
done
