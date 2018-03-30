#!/usr/bin/env bash

for taille_texte in 1 #5 10 50 100 200
do
  for taille_alph in 2 #4 10 20 50 70
  do
      # echo >> plot_${taille_texte}_${taille_alph}.csv
      nf=$(awk -F "," 'NR==1 { print NF }' plot_${taille_texte}_${taille_alph}.csv)
      # echo $nf
      nl=$(cat plot_${taille_texte}_${taille_alph}.csv | wc -l)
      echo -n ${taille_texte}'@'${taille_alph}',' >> plot_res.csv
      echo -n ${taille_alph}'@'${taille_texte}',' >> plot_inv_res.csv
      for (( i = 1; i <= nf; i++ )); do
        echo -n 'scale=4; ' > expr
        echo $(awk -F "," -vf=$i '{print $f }' plot_${taille_texte}_${taille_alph}.csv |\
        tr "\n" "+" | sed 's/.$//') >> expr
        # cat expr
        echo 'scale=4; '$(cat expr | bc)'/'${nl} | bc | tr '\n' ',' > res
        cat res >> plot_res.csv
        cat res >> plot_inv_res.csv
      done
      echo >> plot_res.csv
      echo >> plot_inv_res.cscv
      # echo $expr
  done
done
