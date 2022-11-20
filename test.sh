echo "------------------------------------------------------------------------------------------------";
echo "####################################### EXPECTED SUCCESS #######################################";
for file in ./testfiles/PASS/*.php
do  
    echo "------------------------------------------------------------------------------------------------";
    echo "TESTFILE: ${file}";
    echo "";
    echo "RESULT:"
    cat "$file" | ./parser
    err=`echo $?`;
    echo -n "Error code: ${err} - ";

    case "$err" in
    0)  echo "SUCCESS - EXPECTED";;
    1)  echo "Lexical Error - UNEXPECTED";;
    2)  echo "SYNTAX Error - UNEXPECTED";;
    3)  ;;
    4)  ;;
    5)  ;;
    6)  ;;
    7)  ;;
    8)  ;;
    99) ;;
    esac

done

echo "------------------------------------------------------------------------------------------------";
echo "######################################## EXPECTED ERROR ########################################";

for file in ./testfiles/FAIL/*.php
do  
    echo "------------------------------------------------------------------------------------------------";
    echo "TESTFILE: ${file}";
    echo "";
    echo "RESULT:"
    cat "$file" | ./parser
    err=`echo $?`;
    echo -n "Error code: ${err} - ";

    case "$err" in
    0)  echo "SUCCESS - UNEXPECTED";;
    1)  echo "Lexical Error - EXPECTED";;
    2)  echo "SYNTAX Error - EXPECTED";;
    3)  ;;
    4)  ;;
    5)  ;;
    6)  ;;
    7)  ;;
    8)  ;;
    99) ;;
    esac

done