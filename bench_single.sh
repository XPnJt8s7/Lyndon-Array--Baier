#!/bin/bash

./genere-texte 10000000 4 > texte &&\
./LA_gprof -f texte && \
gprof LA_gprof gmon.out > analysis.txt &&\
less analysis.txt

# safeRunCommand() {
#   typeset cmnd="$*"
#   typeset ret_code
#
#   #echo cmnd=$cmnd
#   eval $cmnd
#   ret_code=$?
#   if [ $ret_code != 0 ]; then
#     printf "Error : [%d] when executing command: '$cmnd'\n" $ret_code
#     exit $ret_code
#   else
#     printf "done\n"
#   fi
# }
#
# # A POSIX variable
# OPTIND=1         # Reset in case getopts has been used previously in the shell.
#
# # Initialize our own variables:
# output_file=""
# verbose=0
#
# while getopts "h?vf:" opt; do
#     case "$opt" in
#     h|\?)
#         man ls
#         exit 0
#         ;;
#     v)  verbose=1
#         ;;
#     f)  output_file=$OPTARG
#         ;;
#     esac
# done
#
# shift $((OPTIND-1))
#
# [ "$1" = "--" ] && shift
#
# command="./LA_gprof -f $output_file"
# safeRunCommand "$command" && \
# #./LA_gprof -f $output_file && \
# gprof LA_gprof gmon.out > analysis.txt &&\
# less analysis.txt

#echo "verbose=$verbose, output_file='$output_file', Leftovers: $@"

# End of file
