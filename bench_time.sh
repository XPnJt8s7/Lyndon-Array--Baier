#!/bin/bash
crono='/bin/date +%s%N'

for (( i = 0; i < 100; i++ )); do
  for taille in 10 100 1000 10000 100000 1000000
  do
    ./genere-texte $taille 4 > texte

    pre=$($crono)
    ./LA_prod -f texte
    post=$($crono)
    res=$(echo $post-$pre | /usr/bin/bc)

    echo "LA: $res"
    echo -n "$res," >> time_LA.csv

    pre=$($crono)
    ./newLA_prod -f texte
    post=$($crono)
    res=$(echo $post-$pre | /usr/bin/bc)

    echo "newLA: $res"
    echo -n "$res," >> time_newLA.csv

    sleep 1

    #echo $taille
  done

  echo >> time_LA.csv
  echo >> time_newLA.csv

done

rm texte
