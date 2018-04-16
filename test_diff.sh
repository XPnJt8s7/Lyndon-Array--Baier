#!/usr/bin/env bash

./genere-texte 10 4 > texte
valgrind ./LA_test -f texte > resLA
valgrind ./newLA_test -f texte > resnewLA
diff resLA resnewLA
#rm resLA resnewLA
