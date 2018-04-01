# head -n 23 analysis.txt | \
# tail -n 18 | \
# tr -s " " | \
# awk -F " " '{print $3,$7}' |\
# sort -t " " -k 2 | \
# awk -F " " '{print $1}' | \
# tr "\n" ","
gprof LA_gprof gmon.out > analysis.txt &&\
awk -F " " 'NR==23 {print $2}' analysis.txt | tr "\n" "," #>> plot_${taille_texte}_${taille_alph}.csv &&\
head -n 23 analysis.txt | tail -n 18 | tr -s " " | awk -F " " '{print $3,$7}' | tr " " "," | sort -t "," -k 2 | tr "\n" "," #>> plot_${taille_texte}_${taille_alph}.csv &&\
# echo >> plot_${taille_texte}_${taille_alph}

# si s est une fuille alors returne (s)
# sinon
#   si s est de type max alors
#     debut
#       i <- 1
#       tanque (i<=k et alfa < beta) faire
#         debut
#           alfa <- max{alfa, alphabeta(gamma,fi,alfa,beta)}
#           i++
#         fintanque
#         returner(alfa)
#     finsi
#   sinon
#     debut
#       i<-1
#       tantque (i<=k et alfa < beta)
#         debut
#           beta <- max{beta,alfabeta(gamma,fi,alfa,beta)}
#           i ++
#         fintanque
#     finsinon
#   finalgo
