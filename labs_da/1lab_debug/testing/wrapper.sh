if ! g++ -std=c++11 -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -g  main.cpp -o 1lab; then
    echo "ERROR: Failed to compile file."
    exit 1
fi

if ! python3 test_gen.py; then
    echo "ERROR: Failed to python generate tests."
    exit 1
fi

for test_file in `ls tests/*.t`; do
    echo "Execute ${test_file}"
    if ! ./1lab < $test_file > tmp ; then
        echo "ERROR"
        continue
    fi
    answer_file="${test_file%.*}"

    if ! diff -u "${answer_file}.a" tmp ; then 
        echo "Failed"
    else
        echo "OK"
    fi
done