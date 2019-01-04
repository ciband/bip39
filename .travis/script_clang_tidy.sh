# run clang tidy
cmake -DENABLE_CLANG_TIDY=ON .
make tidy > output.txt
#if [[ -n $(grep "warning: " output.txt) ]] || [[ -n $(grep "error: " output.txt) ]]; then
# for now only fail the test on errors.  Change this as project matures
if [[ -n $(grep "error: " output.txt) ]]; then
    echo "You must pass the clang tidy checks before submitting a pull request"
    echo ""
    grep --color -E '^|warning: |error: ' output.txt
    exit -1;
else
    # still output the file to show warnings
    echo ""
    grep --color -E '^|warning: |error: ' output.txt
    echo -e "\033[1;32m\xE2\x9C\x93 passed:\033[0m $1";
fi
