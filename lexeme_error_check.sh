#!/bin/sh
./main resources/binary_search.c | grep ERROR
./main resources/bst.c | grep ERROR
./main resources/keyword1.c         | grep ERROR
./main resources/keyword2.c         | grep ERROR
./main resources/keyword3.c         | grep ERROR
./main resources/keyword4.c         | grep ERROR
./main resources/maximum_element.c  | grep ERROR
./main resources/operator.c         | grep ERROR
