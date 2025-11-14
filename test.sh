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

# Test function without arguments
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

# Test function with arguments
run_test_with_args() {
    local opts=$1
    local args=$2
    local test_name="test $test_num"
    
    ./ft_ls -$opts $args > "$TMP_DIR/mine" 2>&1
    LC_ALL=C ls -$opts $args > "$TMP_DIR/real" 2>&1
    
    if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
        echo "$test_name reussis (-$opts $args)"
    else
        echo "$test_name erreur"
        echo "il y a une difference avec : ls -$opts $args"
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

# Create test directory and files
TEST_DIR="$TMP_DIR/test_dir"
TEST_FILE1="$TMP_DIR/test_file1"
TEST_FILE2="$TMP_DIR/test_file2"
mkdir -p "$TEST_DIR"
touch "$TEST_FILE1" "$TEST_FILE2"
touch "$TEST_DIR/file_in_dir1" "$TEST_DIR/file_in_dir2"

# Test with a directory
echo ""
echo "=== Tests avec un dossier ==="
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            run_test_with_args "$combo" "$TEST_DIR"
        fi
    done < <(generate_combinations $size)
done

# Test without options with directory
echo "Test $test_num: no options with directory"
./ft_ls "$TEST_DIR" > "$TMP_DIR/mine" 2>&1
LC_ALL=C ls "$TEST_DIR" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo "test $test_num reussis (with directory)"
else
    echo "test $test_num erreur"
    echo "il y a une difference avec : ls $TEST_DIR"
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))

# Test with files
echo ""
echo "=== Tests avec des fichiers ==="
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            run_test_with_args "$combo" "$TEST_FILE1 $TEST_FILE2"
        fi
    done < <(generate_combinations $size)
done

# Test without options with files
echo "Test $test_num: no options with files"
./ft_ls "$TEST_FILE1" "$TEST_FILE2" > "$TMP_DIR/mine" 2>&1
LC_ALL=C ls "$TEST_FILE1" "$TEST_FILE2" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo "test $test_num reussis (with files)"
else
    echo "test $test_num erreur"
    echo "il y a une difference avec : ls $TEST_FILE1 $TEST_FILE2"
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))

# Test with non-existent files
echo ""
echo "=== Tests avec fichiers inexistants ==="
NONEXISTENT_FILE="$TMP_DIR/nonexistent_file_12345"
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            run_test_with_args "$combo" "$NONEXISTENT_FILE"
        fi
    done < <(generate_combinations $size)
done

# Test without options with non-existent file
echo "Test $test_num: no options with non-existent file"
./ft_ls "$NONEXISTENT_FILE" > "$TMP_DIR/mine" 2>&1
LC_ALL=C ls "$NONEXISTENT_FILE" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo "test $test_num reussis (with non-existent file)"
else
    echo "test $test_num erreur"
    echo "il y a une difference avec : ls $NONEXISTENT_FILE"
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))

# Test with non-existent directory
echo ""
echo "=== Tests avec dossiers inexistants ==="
NONEXISTENT_DIR="$TMP_DIR/nonexistent_dir_12345"
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            run_test_with_args "$combo" "$NONEXISTENT_DIR"
        fi
    done < <(generate_combinations $size)
done

# Test without options with non-existent directory
echo "Test $test_num: no options with non-existent directory"
./ft_ls "$NONEXISTENT_DIR" > "$TMP_DIR/mine" 2>&1
LC_ALL=C ls "$NONEXISTENT_DIR" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo "test $test_num reussis (with non-existent directory)"
else
    echo "test $test_num erreur"
    echo "il y a une difference avec : ls $NONEXISTENT_DIR"
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))

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
