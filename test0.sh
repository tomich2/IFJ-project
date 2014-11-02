#!/bin/bash

red='\e[0;32m';
NC='\e[0m';
green='\e[0;31m';

if [ -d "$DIRECTORY" ]; 
    then
        mkdir test0.outputs;
fi

./lexical > ./test0.outputs/nofile.output 2>&1;
value=$?;
if [ $value = 0 ];
    then
        echo -e "Testing no file                    ${red}PASS${NC}";
    else
        echo -e "Testing no file                    ${green}FAIL${NC}";
fi

./lexical file0 > ./test0.outputs/file0.output 2>&1;
value=$?;
if [ $value = 0 ];
    then
        echo -e "Testing file0                      ${red}PASS${NC}";
    else
        echo -e "Testing file0                      ${green}FAIL{NC}";
fi

./lexical file1 > test0.outputs/file1.output 2>&1;
value=$?;
if [ $value = 0 ];
    then
        echo -e "Testing file1                      ${red}PASS${NC}";
    else
        echo -e "Testing file1                      ${green}FAIL{NC}";
fi

./lexical file2 > test0.outputs/file2.output 2>&1;
value=$?;
if [ $value = 0 ];
    then
        echo -e "Testing file2                      ${red}PASS${NC}";
    else
        echo -e "Testing file2                      ${green}FAIL{NC}";
fi

./lexical file3 > test0.outputs/file3.output 2>&1;
value=$?;
if [ $value = 0 ];
    then
        echo -e "Testing file3                      ${red}PASS${NC}";
    else
        echo -e "Testing file3                      ${green}FAIL{NC}";
fi

./lexical file4 > test0.outputs/file4.output 2>&1;
value=$?;
if [ $value = 0 ];
    then
        echo -e "Testing file4                      ${red}PASS${NC}";
    else
        echo -e "Testing file4                      ${green}FAIL{NC}";
fi

./lexical file5 > test0.outputs/file5.output 2>&1;
value=$?;
if [ $value = 0 ];
    then
        echo -e "Testing file5                      ${red}PASS${NC}";
    else
        echo -e "Testing file5                      ${green}FAIL{NC}";
fi

exit 0;
