#!/bin/bash
crono='/bin/date +%s%N'

echo "1000000,2000000,4000000,8000000,16000000,32000000,64000000,128000000,256000000" >> time_LA.csv

for (( i = 0; i < 100; i++ )); do
  for taille in 1000000 2000000 4000000 8000000 16000000 32000000 64000000 128000000 256000000
  do
    ./genere-texte $taille 4 > texte

    pre=$($crono)
    ./LA_prod -f texte
    sleep 1
    post=$($crono)
    res=$(echo $post-$pre | /usr/bin/bc)

    echo "LA($taille): $res"
    echo -n "$res," >> time_LA.csv

    # pre=$($crono)
    # # ./newLA_prod -f texte
    # sleep 1
    # post=$($crono)
    # res=$(echo $post-$pre | /usr/bin/bc)
    #
    # echo "newLA: $res"
    # # echo -n "$res," >> time_newLA.csv

    sleep 1

    #echo $taille
  done

  echo >> time_LA.csv
  # echo >> time_newLA.csv

done

rm texte
