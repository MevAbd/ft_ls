#!/bin/bash

# Generate all combinations of options
options=("a" "r" "R" "t" "l")
TMP_DIR=$(mktemp -d)
test_num=1
failed_tests=0
total_tests=0

# Function to generate all combinations of a given size
generate_combinations() {
    local size=$1
    local result=()
    
    # Generate all combinations using bit manipulation
    local total=$((1 << ${#options[@]}))
    local i=0
    
    while [ $i -lt $total ]; do
        local combo=""
        local count=0
        local j=0
        
        while [ $j -lt ${#options[@]} ]; do
            if [ $((i & (1 << j))) -ne 0 ]; then
                combo="${combo}${options[j]}"
                count=$((count + 1))
            fi
            j=$((j + 1))
        done
        
        if [ $count -eq $size ] && [ -n "$combo" ]; then
            result+=("$combo")
        fi
        i=$((i + 1))
    done
    
    printf '%s\n' "${result[@]}"
}

# Test function
run_test() {
    local opts=$1
    local test_name="test $test_num"
    
    ./ft_ls -$opts > "$TMP_DIR/mine" 2>&1
    LC_ALL=C ls -$opts > "$TMP_DIR/real" 2>&1
    
    if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
        echo "$test_name reussis (-$opts)"
    else
        echo "$test_name erreur"
        echo "il y a une difference avec : ls -$opts"
        failed_tests=$((failed_tests + 1))
    fi
    
    test_num=$((test_num + 1))
    total_tests=$((total_tests + 1))
}

# Test without options first
echo "Test $test_num: no options"
./ft_ls > "$TMP_DIR/mine" 2>&1
LC_ALL=C ls > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo "test $test_num reussis"
else
    echo "test $test_num erreur"
    echo "il y a une difference avec : ls"
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))

# Test all combinations from 1 to 5 options
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            run_test "$combo"
        fi
    done < <(generate_combinations $size)
done

# Cleanup
rm -rf "$TMP_DIR"

# Summary
echo ""
echo "Total tests: $total_tests"
if [ $failed_tests -eq 0 ]; then
    echo "Tous les tests ont reussi !"
    exit 0
else
    echo "Tests echoues: $failed_tests"
    exit 1
fi
