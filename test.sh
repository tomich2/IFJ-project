#!/bin/bash

red='\e[0;31m';
NC='\e[0m';
green='\e[0;32m';

DIRECTORY=test0.outputs;

if [ ! -d "$DIRECTORY" ];
    then
        mkdir test0.outputs;
fi

./parser > ./test0.outputs/nofile.output 2>&1;
value=$?;
expected=99;
if [ $value = $expected ];
    then
        echo -e "Testing no file                    ${green}PASS${NC}";
    else
        echo -e "Testing no file                    ${red}FAIL, return code $value, expected $expected${NC}" ;
fi

./parser ./test/file0 > ./test0.outputs/file0.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
        echo -e "Testing file0                      ${green}PASS${NC}";
    else
        echo -e "Testing file0                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file1 > test0.outputs/file1.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
        echo -e "Testing file1                      ${green}PASS${NC}";
    else
        echo -e "Testing file1                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file2 > test0.outputs/file2.output 2>&1;
value=$?;
expected=0;
if [ $value = 0 ];
    then
        echo -e "Testing file2                      ${green}PASS${NC}";
    else
        echo -e "Testing file2                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file3 > test0.outputs/file3.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
        echo -e "Testing file3                      ${green}PASS${NC}";
    else
        echo -e "Testing file3                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file4 > test0.outputs/file4.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
        echo -e "Testing file4                      ${green}PASS${NC}";
    else
        echo -e "Testing file4                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file5 > test0.outputs/file5.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
        echo -e "Testing file5                      ${green}PASS${NC}";
    else
        echo -e "Testing file5                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file6 > test0.outputs/file6.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file6                      ${green}PASS${NC}";
    else
        echo -e "Testing file6                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file7 > test0.outputs/file7.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file7                      ${green}PASS${NC}";
    else
        echo -e "Testing file7                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file8 > test0.outputs/file8.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file8                      ${green}PASS${NC}";
    else
        echo -e "Testing file8                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file9 > test0.outputs/file9.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file9                      ${green}PASS${NC}";
    else
        echo -e "Testing file9                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file10 > test0.outputs/file10.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file10                     ${green}PASS${NC}";
    else
        echo -e "Testing file10                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file11 > test0.outputs/file11.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file11                     ${green}PASS${NC}";
    else
        echo -e "Testing file11                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file12 > test0.outputs/file12.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file12                     ${green}PASS${NC}";
    else
        echo -e "Testing file12                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file13 > test0.outputs/file13.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
        echo -e "Testing file13                     ${green}PASS${NC}";
    else
        echo -e "Testing file13                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file14 > test0.outputs/file14.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
        echo -e "Testing file14                     ${green}PASS${NC}";
    else
        echo -e "Testing file14                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file15 > test0.outputs/file15.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
        echo -e "Testing file15                     ${green}PASS${NC}";
    else
        echo -e "Testing file15                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file16 > test0.outputs/file16.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
        echo -e "Testing file16                     ${green}PASS${NC}";
    else
        echo -e "Testing file16                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file17 > test0.outputs/file17.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file17                     ${green}PASS${NC}";
    else
        echo -e "Testing file17                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file18 > test0.outputs/file18.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file18                     ${green}PASS${NC}";
    else
        echo -e "Testing file18                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file19 > test0.outputs/file19.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file19                     ${green}PASS${NC}";
    else
        echo -e "Testing file19                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file20 > test0.outputs/file20.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file20                     ${green}PASS${NC}";
    else
        echo -e "Testing file20                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file21 > test0.outputs/file21.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file21                     ${green}PASS${NC}";
    else
        echo -e "Testing file21                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file22 > test0.outputs/file22.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file22                     ${green}PASS${NC}";
    else
        echo -e "Testing file22                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file23 > test0.outputs/file23.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file23                     ${green}PASS${NC}";
    else
        echo -e "Testing file23                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file24 > test0.outputs/file24.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file24                     ${green}PASS${NC}";
    else
        echo -e "Testing file24                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file25 > test0.outputs/file25.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file25                     ${green}PASS${NC}";
    else
        echo -e "Testing file25                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file26 > test0.outputs/file26.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file26                     ${green}PASS${NC}";
    else
        echo -e "Testing file26                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file27 > test0.outputs/file27.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file27                     ${green}PASS${NC}";
    else
        echo -e "Testing file27                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file28 > test0.outputs/file28.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file28                     ${green}PASS${NC}";
    else
        echo -e "Testing file28                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file29 > test0.outputs/file29.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file29                     ${green}PASS${NC}";
    else
        echo -e "Testing file29                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file30 > test0.outputs/file30.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file30                     ${green}PASS${NC}";
    else
        echo -e "Testing file30                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file31 > test0.outputs/file31.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file31                     ${green}PASS${NC}";
    else
        echo -e "Testing file31                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file32 > test0.outputs/file32.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file32                     ${green}PASS${NC}";
    else
        echo -e "Testing file32                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file33 > test0.outputs/file33.output 2>&1;
value=$?;
expected=1;
if [ $value = $expected ];
    then
        echo -e "Testing file33                     ${green}PASS${NC}";
    else
        echo -e "Testing file33                     ${red}FAIL, return code $value, expected $expected${NC}";
fi



exit 0;
