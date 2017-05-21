#!/bin/bash
declare -a lex_test_files

main_prog="20141570"
resource_dir="resources"
lex_test_files=( "binary_search.c" "keyword1.c" "keyword2.c" "keyword3.c" "operator.c" )

for i in "${lex_test_files[@]}"
do
    ./$main_prog $resource_dir/$i | grep ERROR
done
