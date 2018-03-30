#!/bin/bash

safeRungenere-texte() {
  typeset cmnd="$*"
  typeset ret_code

  #echo cmnd=$cmnd
  eval $cmnd > texte
  ret_code=$?
  if [ $ret_code != 0 ]; then
    printf "Error : [%d] when executing command: '$cmnd'" $ret_code
    exit $ret_code
  # else
  #   printf "done\n"
  fi
}

safeRunCommand() {
  typeset cmnd="$*"
  typeset ret_code

  #echo cmnd=$cmnd
  eval $cmnd > g_avg_max
  ret_code=$?
  if [ $ret_code != 0 ]; then
    printf "Error : [%d] when executing command: '$cmnd'" $ret_code
    exit $ret_code
  # else
  #   printf "done\n"
  fi
}

#1M,5M,10M,50M,100M,200M
#2,4,10,20,50,70
#plot (taille_alph,t)
#plot per taille_texte

M=1000000

# for taille_texte in 1 5 10 50 100 200
# do
#   for taille_alph in 2 4 10 20 50 70
#   do
#     file="plot_${taille_texte}_${taille_alph}"
#     test -f $file
#     ret=$?
#     if [ $ret == 1 ]; then
#       echo "moyenne,algo3,decrement_group_count,order_suffs,group_moyenne,group_max,group_min" >> plot_${taille_texte}_${taille_alph}.csv && echo hello!
#     fi
#   done
# done



for taille_texte in 2 #5 10 20 40 80 160 320
do
  for taille_alph in 2 4 8 #16 32 64 128
  do
    for (( loops = 0; loops < 2; loops++ )); do
      #printf "%'.3f\n" 1000000 -> 1,000,000
      echo "(taille_texte,taille_alph) = ($taille_texte,$taille_alph)"
      command="./genere-texte $(($taille_texte*$M)) $taille_alph"
      safeRungenere-texte "$command"
      #sleep 1
      # head -c 10 texte # read up to 10 bytes
      # echo
      command="./LA_gprof -f texte"
      safeRunCommand "$command"
      gprof -z LA_gprof gmon.out > analysis.txt &&\
      awk -F " " 'NR==24 {print $2}' analysis.txt | tr "\n" "," >> plot_${taille_texte}_${taille_alph}.csv &&\
      head -n 24 analysis.txt | tail -n 19 | tr -s " " | awk -F " " '{print $3,$7}' | sort -t " " -k 2 | sed -n -e 2p -e 4p -e 12p | awk -F " " '{print $1}' | tr "\n" "," >> plot_${taille_texte}_${taille_alph}.csv &&\
      cat g_avg_max >> plot_${taille_texte}_${taille_alph}.csv
      #echo >> plot_${taille_texte}_${taille_alph}.csv
    done
  done
done

trash texte

for taille_texte in 2 #5 10 20 40 80 160 320
do
  for taille_alph in 2 4 8 #16 32 64 128
  do
      # echo >> plot_${taille_texte}_${taille_alph}.csv
      nf=$(awk -F "," 'NR==1 { print NF }' plot_${taille_texte}_${taille_alph}.csv)
      # echo $nf
      nl=$(cat plot_${taille_texte}_${taille_alph}.csv | wc -l)
      echo -n ${taille_texte}'@'${taille_alph}',' >> plot_res.csv
      echo -n ${taille_alph}'@'${taille_texte}',' >> plot_inv_res.csv
      echo -n ${taille_texte}'@'${taille_alph}','
      for (( i = 1; i <= nf; i++ )); do
        echo -n 'scale=4; ' > expr
        echo $(awk -F "," -vf=$i '{print $f }' plot_${taille_texte}_${taille_alph}.csv |\
        tr "\n" "+" | sed 's/.$//') >> expr
        # cat expr
        echo 'scale=4; '$(cat expr | bc)'/'${nl} | bc | tr '\n' ',' > res
        cat res
        cat res >> plot_res.csv
        cat res >> plot_inv_res.csv
      done
      echo >> plot_res.csv
      echo >> plot_inv_res.cscv
      echo
      # echo $expr
  done
done


# command="./LA_gprof -f $output_file"
# safeRunCommand "$command"&& \
# #./LA_gprof -f $output_file && \
# gprof LA_gprof gmon.out > analysis.txt &&\
# less analysis.txt

#echo "verbose=$verbose, output_file='$output_file', Leftovers: $@"

# End of file
