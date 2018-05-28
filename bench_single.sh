#!/bin/bash

./genere-texte $1 4 > texte &&\
./LA_gprof -f texte && \
gprof LA_gprof gmon.out > analysis_LA_$1.txt &&\
less analysis_LA_$1.txt
