#!/bin/bash

./genere-texte $1 4 > texte &&\
./newLA_gprof -f texte && \
gprof newLA_gprof gmon.out > analysis_newLA_$1.txt &&\
less analysis_newLA_$1.txt
