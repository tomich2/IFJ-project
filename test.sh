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
	if diff ./test0.outputs/file0.output ./test/file0.cmp >/dev/null ;
        then
	echo -e "Testing file0                      ${green}PASS${NC}";
	else
	echo -e "Testing file0                      ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file0                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file1 > test0.outputs/file1.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file1.output ./test/file1.cmp >/dev/null ;
        then
	echo -e "Testing file1                      ${green}PASS${NC}";
	else
	echo -e "Testing file1                      ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file1                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file2 > test0.outputs/file2.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file2.output ./test/file2.cmp >/dev/null ;
        then
	echo -e "Testing file2                      ${green}PASS${NC}";
	else
	echo -e "Testing file2                      ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file2                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file3 > test0.outputs/file3.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file3.output ./test/file3.cmp >/dev/null ;
        then
	echo -e "Testing file3                      ${green}PASS${NC}";
	else
	echo -e "Testing file3                      ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file3                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file4 > test0.outputs/file4.output 2>&1;

expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file4.output ./test/file4.cmp >/dev/null ;
        then
	echo -e "Testing file4                      ${green}PASS${NC}";
	else
	echo -e "Testing file4                      ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file4                      ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file5 > test0.outputs/file5.output 2>&1;
value=$?;
expected=0;
touch ./test/file5.cmp
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file5.output ./test/file5.cmp >/dev/null ;
        then
	echo -e "Testing file5                      ${green}PASS${NC}";
	else
	echo -e "Testing file5                      ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file5                      ${red}FAIL, return code $value, expected $expected${NC}";
fi
rm ./test/file5.cmp

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
touch ./test/file13.cmp
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file13.output ./test/file13.cmp >/dev/null ;
        then
	echo -e "Testing file13                     ${green}PASS${NC}";
	else
	echo -e "Testing file13                     ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file13                     ${red}FAIL, return code $value, expected $expected${NC}";
fi
rm ./test/file13.cmp

./parser ./test/file14 > test0.outputs/file14.output 2>&1;
value=$?;
expected=0;
touch ./test/file14.cmp
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file14.output ./test/file14.cmp >/dev/null ;
        then
	echo -e "Testing file14                     ${green}PASS${NC}";
	else
	echo -e "Testing file14                     ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file14                     ${red}FAIL, return code $value, expected $expected${NC}";
fi
rm ./test/file14.cmp

./parser ./test/file15 > test0.outputs/file15.output 2>&1;
value=$?;
expected=4;
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
	if diff ./test0.outputs/file16.output ./test/file16.cmp >/dev/null ;
        then
	echo -e "Testing file16                     ${green}PASS${NC}";
	else
	echo -e "Testing file16                     ${red}FAIL, different output${NC}";
	fi
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

./parser ./test/file34 > test0.outputs/file34.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file34                     ${green}PASS${NC}";
    else
        echo -e "Testing file34                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file35 > test0.outputs/file35.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file35                     ${green}PASS${NC}";
    else
        echo -e "Testing file35                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file36 > test0.outputs/file36.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file36                     ${green}PASS${NC}";
    else
        echo -e "Testing file36                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file37 > test0.outputs/file37.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file37                     ${green}PASS${NC}";
    else
        echo -e "Testing file37                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file38 > test0.outputs/file38.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file38                     ${green}PASS${NC}";
    else
        echo -e "Testing file38                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file39 > test0.outputs/file39.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file39                     ${green}PASS${NC}";
    else
        echo -e "Testing file39                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file40 > test0.outputs/file40.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file40                     ${green}PASS${NC}";
    else
        echo -e "Testing file40                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file41 > test0.outputs/file41.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file41                     ${green}PASS${NC}";
    else
        echo -e "Testing file41                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file42 > test0.outputs/file42.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file42                     ${green}PASS${NC}";
    else
        echo -e "Testing file42                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file43 > test0.outputs/file43.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file43                     ${green}PASS${NC}";
    else
        echo -e "Testing file43                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file44 > test0.outputs/file44.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file44                     ${green}PASS${NC}";
    else
        echo -e "Testing file44                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file45 > test0.outputs/file45.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file45                     ${green}PASS${NC}";
    else
        echo -e "Testing file45                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file46 > test0.outputs/file46.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file46                     ${green}PASS${NC}";
    else
        echo -e "Testing file46                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file47 > test0.outputs/file47.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file47                     ${green}PASS${NC}";
    else
        echo -e "Testing file47                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file48 > test0.outputs/file48.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file48                     ${green}PASS${NC}";
    else
        echo -e "Testing file48                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file49 > test0.outputs/file49.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file49                     ${green}PASS${NC}";
    else
        echo -e "Testing file49                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file50 > test0.outputs/file50.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file50                     ${green}PASS${NC}";
    else
        echo -e "Testing file50                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file51 > test0.outputs/file51.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file51                     ${green}PASS${NC}";
    else
        echo -e "Testing file51                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file52 > test0.outputs/file52.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file52                     ${green}PASS${NC}";
    else
        echo -e "Testing file52                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file53 > test0.outputs/file53.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file53                     ${green}PASS${NC}";
    else
        echo -e "Testing file53                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file54 > test0.outputs/file54.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file54                     ${green}PASS${NC}";
    else
        echo -e "Testing file54                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file55 > test0.outputs/file55.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file55                     ${green}PASS${NC}";
    else
        echo -e "Testing file55                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file56 > test0.outputs/file56.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file56                     ${green}PASS${NC}";
    else
        echo -e "Testing file56                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file57 > test0.outputs/file57.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file57                     ${green}PASS${NC}";
    else
        echo -e "Testing file57                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file58 > test0.outputs/file58.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file58                     ${green}PASS${NC}";
    else
        echo -e "Testing file58                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file59 > test0.outputs/file59.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file59                     ${green}PASS${NC}";
    else
        echo -e "Testing file59                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file60 > test0.outputs/file60.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file60                     ${green}PASS${NC}";
    else
        echo -e "Testing file60                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file61 > test0.outputs/file61.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file61                     ${green}PASS${NC}";
    else
        echo -e "Testing file61                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file62 > test0.outputs/file62.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file62                     ${green}PASS${NC}";
    else
        echo -e "Testing file62                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file63 > test0.outputs/file63.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file63                     ${green}PASS${NC}";
    else
        echo -e "Testing file63                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file64 > test0.outputs/file64.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file64                     ${green}PASS${NC}";
    else
        echo -e "Testing file64                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file65 > test0.outputs/file65.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file65                     ${green}PASS${NC}";
    else
        echo -e "Testing file65                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file66 > test0.outputs/file66.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file66                     ${green}PASS${NC}";
    else
        echo -e "Testing file66                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file67 > test0.outputs/file67.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file67                     ${green}PASS${NC}";
    else
        echo -e "Testing file67                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file68 > test0.outputs/file68.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file68                     ${green}PASS${NC}";
    else
        echo -e "Testing file68                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file69 > test0.outputs/file69.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file69                     ${green}PASS${NC}";
    else
        echo -e "Testing file69                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file70 > test0.outputs/file70.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file70                     ${green}PASS${NC}";
    else
        echo -e "Testing file70                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file71 > test0.outputs/file71.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file71                     ${green}PASS${NC}";
    else
        echo -e "Testing file71                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file72 > test0.outputs/file72.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file72                     ${green}PASS${NC}";
    else
        echo -e "Testing file72                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file73 > test0.outputs/file73.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file73                     ${green}PASS${NC}";
    else
        echo -e "Testing file73                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file74 > test0.outputs/file74.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file74                     ${green}PASS${NC}";
    else
        echo -e "Testing file74                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file75 > test0.outputs/file75.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file75                     ${green}PASS${NC}";
    else
        echo -e "Testing file75                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file76 > test0.outputs/file76.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file76                     ${green}PASS${NC}";
    else
        echo -e "Testing file76                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file77 > test0.outputs/file77.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file77                     ${green}PASS${NC}";
    else
        echo -e "Testing file77                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file78 > test0.outputs/file78.output 2>&1;
value=$?;
expected=2;
if [ $value = $expected ];
    then
        echo -e "Testing file78                     ${green}PASS${NC}";
    else
        echo -e "Testing file78                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file79 > test0.outputs/file79.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file79                     ${green}PASS${NC}";
    else
        echo -e "Testing file79                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file80 > test0.outputs/file80.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file80                     ${green}PASS${NC}";
    else
        echo -e "Testing file80                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file81 > test0.outputs/file81.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file81                     ${green}PASS${NC}";
    else
        echo -e "Testing file81                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file82 > test0.outputs/file82.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file82                     ${green}PASS${NC}";
    else
        echo -e "Testing file82                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file83 > test0.outputs/file83.output 2>&1;
value=$?;
expected=5;
if [ $value = $expected ];
    then
        echo -e "Testing file83                     ${green}PASS${NC}";
    else
        echo -e "Testing file83                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file84 > test0.outputs/file84.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file84                     ${green}PASS${NC}";
    else
        echo -e "Testing file84                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file85 > test0.outputs/file85.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file85                     ${green}PASS${NC}";
    else
        echo -e "Testing file85                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file86 > test0.outputs/file86.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file86                     ${green}PASS${NC}";
    else
        echo -e "Testing file86                     ${red}FAIL, return code $value, expected $expected${NC}";
fi



./parser ./test/file87 > test0.outputs/file87.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file87                     ${green}PASS${NC}";
    else
        echo -e "Testing file87                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file88 > test0.outputs/file88.output 2>&1;
value=$?;
expected=3;
if [ $value = $expected ];
    then
        echo -e "Testing file88                     ${green}PASS${NC}";
    else
        echo -e "Testing file88                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file89 > test0.outputs/file89.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file89                     ${green}PASS${NC}";
    else
        echo -e "Testing file89                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file90 > test0.outputs/file90.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file90                     ${green}PASS${NC}";
    else
        echo -e "Testing file90                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file91 > test0.outputs/file91.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file91                     ${green}PASS${NC}";
    else
        echo -e "Testing file91                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file92 > test0.outputs/file92.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file92                     ${green}PASS${NC}";
    else
        echo -e "Testing file92                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file93 > test0.outputs/file93.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file93                     ${green}PASS${NC}";
    else
        echo -e "Testing file93                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file94 > test0.outputs/file94.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file94                     ${green}PASS${NC}";
    else
        echo -e "Testing file94                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file95 > test0.outputs/file95.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file95                     ${green}PASS${NC}";
    else
        echo -e "Testing file95                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file96 > test0.outputs/file96.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file96                     ${green}PASS${NC}";
    else
        echo -e "Testing file96                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file97 > test0.outputs/file97.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file97                     ${green}PASS${NC}";
    else
        echo -e "Testing file97                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file98 > test0.outputs/file98.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file98                     ${green}PASS${NC}";
    else
        echo -e "Testing file98                     ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file99 > test0.outputs/file99.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file99                     ${green}PASS${NC}";
    else
        echo -e "Testing file99                     ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file100 > test0.outputs/file100.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file100                    ${green}PASS${NC}";
    else
        echo -e "Testing file100                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file101 > test0.outputs/file101.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file101                    ${green}PASS${NC}";
    else
        echo -e "Testing file101                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file102 > test0.outputs/file102.output 2>&1;
value=$?;
expected=4;
if [ $value = $expected ];
    then
        echo -e "Testing file102                    ${green}PASS${NC}";
    else
        echo -e "Testing file102                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file103 > test0.outputs/file103.output 2>&1;
value=$?;
expected=8;
if [ $value = $expected ];
    then
        echo -e "Testing file103                    ${green}PASS${NC}";
    else
        echo -e "Testing file103                    ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file104 > test0.outputs/file104.output 2>&1;
value=$?;
expected=8;
if [ $value = $expected ];
    then
        echo -e "Testing file104                    ${green}PASS${NC}";
    else
        echo -e "Testing file104                    ${red}FAIL, return code $value, expected $expected${NC}";
fi


./parser ./test/file105 > test0.outputs/file105.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file105.output ./test/file105.cmp >/dev/null ;
        then
	echo -e "Testing file105                    ${green}PASS${NC}";
	else
	echo -e "Testing file105                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file105                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file106 > test0.outputs/file106.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file106.output ./test/file106.cmp >/dev/null ;
        then
	echo -e "Testing file106                    ${green}PASS${NC}";
	else
	echo -e "Testing file106                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file106                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file107 > test0.outputs/file107.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file107.output ./test/file107.cmp >/dev/null ;
        then
	echo -e "Testing file107                    ${green}PASS${NC}";
	else
	echo -e "Testing file107                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file107                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file108 > test0.outputs/file108.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file108.output ./test/file108.cmp >/dev/null ;
        then
	echo -e "Testing file108                    ${green}PASS${NC}";
	else
	echo -e "Testing file108                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file108                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file109 > test0.outputs/file109.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file109.output ./test/file109.cmp >/dev/null ;
        then
	echo -e "Testing file109                    ${green}PASS${NC}";
	else
	echo -e "Testing file109                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file109                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file110 > test0.outputs/file110.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file110.output ./test/file110.cmp >/dev/null ;
        then
	echo -e "Testing file110                    ${green}PASS${NC}";
	else
	echo -e "Testing file110                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file110                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file111 > test0.outputs/file111.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file111.output ./test/file111.cmp >/dev/null ;
        then
	echo -e "Testing file111                    ${green}PASS${NC}";
	else
	echo -e "Testing file111                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file111                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file112 > test0.outputs/file112.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file112.output ./test/file112.cmp >/dev/null ;
        then
	echo -e "Testing file112                    ${green}PASS${NC}";
	else
	echo -e "Testing file112                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file112                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file113 > test0.outputs/file113.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file113.output ./test/file113.cmp >/dev/null ;
        then
	echo -e "Testing file113                    ${green}PASS${NC}";
	else
	echo -e "Testing file113                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file113                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file114 > test0.outputs/file114.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file114.output ./test/file114.cmp >/dev/null ;
        then
	echo -e "Testing file114                    ${green}PASS${NC}";
	else
	echo -e "Testing file114                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file114                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

./parser ./test/file115 > test0.outputs/file115.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file115.output ./test/file115.cmp >/dev/null ;
        then
	echo -e "Testing file115                    ${green}PASS${NC}";
	else
	echo -e "Testing file115                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file115                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

echo "4" | ./parser ./test/file116 > test0.outputs/file116.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file116.output ./test/file116.cmp >/dev/null ;
        then
	echo -e "Testing file116                    ${green}PASS${NC}";
	else
	echo -e "Testing file116                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file116                    ${red}FAIL, return code $value, expected $expected${NC}";
fi

echo "7" | ./parser ./test/file117 > test0.outputs/file117.output 2>&1;
value=$?;
expected=0;
if [ $value = $expected ];
    then
	if diff ./test0.outputs/file117.output ./test/file117.cmp >/dev/null ;
        then
	echo -e "Testing file117                    ${green}PASS${NC}";
	else
	echo -e "Testing file117                    ${red}FAIL, different output${NC}";
	fi
    else
        echo -e "Testing file117                    ${red}FAIL, return code $value, expected $expected${NC}";
fi



exit 0;
