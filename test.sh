#!/bin/bash

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Generate all combinations of options
options=("a" "r" "R" "t" "l")
TMP_DIR=$(mktemp -d)
test_num=1
failed_tests=0
total_tests=0

# Section counters
section_passed=0
section_total=0

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

# Print section summary
print_section_summary() {
    local section_name=$1
    local passed=$section_passed
    local total=$section_total
    
    echo ""
    if [ $passed -eq $total ]; then
        echo -e "${GREEN}=== $section_name: $passed/$total reussis ===${NC}"
    else
        echo -e "${RED}=== $section_name: $passed/$total reussis ===${NC}"
    fi
    echo ""
    
    section_passed=0
    section_total=0
}

# Test function without arguments
run_test() {
    local opts=$1
    local test_name="test $test_num"
    
    ./ft_ls -$opts > "$TMP_DIR/mine" 2>&1
    LC_ALL=C /bin/bash -c "ls -$opts" > "$TMP_DIR/real" 2>&1
    
    if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
        echo -e "${GREEN}$test_name reussis (-$opts)${NC}"
        section_passed=$((section_passed + 1))
    else
        echo -e "${RED}$test_name erreur (-$opts)${NC}"
        echo -e "${RED}Difference:${NC}"
        diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
        failed_tests=$((failed_tests + 1))
    fi
    
    test_num=$((test_num + 1))
    total_tests=$((total_tests + 1))
    section_total=$((section_total + 1))
}

# Test function with arguments
run_test_with_args() {
    local opts=$1
    local args=$2
    local test_name="test $test_num"
    
    ./ft_ls -$opts $args > "$TMP_DIR/mine" 2>&1
    LC_ALL=C /bin/bash -c "ls -$opts $args" > "$TMP_DIR/real" 2>&1
    
    if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
        echo -e "${GREEN}$test_name reussis (-$opts $args)${NC}"
        section_passed=$((section_passed + 1))
    else
        echo -e "${RED}$test_name erreur (-$opts $args)${NC}"
        echo -e "${RED}Difference:${NC}"
        diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
        failed_tests=$((failed_tests + 1))
    fi
    
    test_num=$((test_num + 1))
    total_tests=$((total_tests + 1))
    section_total=$((section_total + 1))
}

# Test without options first
echo -e "${YELLOW}=== Tests sans arguments ===${NC}"
echo "Test $test_num: no options"
./ft_ls > "$TMP_DIR/mine" 2>&1
LC_ALL=C /bin/bash -c "ls" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test all combinations from 1 to 5 options
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            run_test "$combo"
        fi
    done < <(generate_combinations $size)
done

print_section_summary "Tests sans arguments"

# Create test directory and files
TEST_DIR="$TMP_DIR/test_dir"
TEST_FILE1="$TMP_DIR/test_file1"
TEST_FILE2="$TMP_DIR/test_file2"
mkdir -p "$TEST_DIR"
touch "$TEST_FILE1" "$TEST_FILE2"
touch "$TEST_DIR/file_in_dir1" "$TEST_DIR/file_in_dir2"

# Test with a directory
echo -e "${YELLOW}=== Tests avec un dossier ===${NC}"
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
LC_ALL=C /bin/bash -c "ls $TEST_DIR" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (with directory)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (with directory)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

print_section_summary "Tests avec un dossier"

# Test with files
echo -e "${YELLOW}=== Tests avec des fichiers ===${NC}"
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
LC_ALL=C /bin/bash -c "ls $TEST_FILE1 $TEST_FILE2" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (with files)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (with files)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

print_section_summary "Tests avec des fichiers"

# Test with non-existent files
echo -e "${YELLOW}=== Tests avec fichiers inexistants ===${NC}"
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
LC_ALL=C /bin/bash -c "ls $NONEXISTENT_FILE" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (with non-existent file)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (with non-existent file)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

print_section_summary "Tests avec fichiers inexistants"

# Test with non-existent directory
echo -e "${YELLOW}=== Tests avec dossiers inexistants ===${NC}"
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
LC_ALL=C /bin/bash -c "ls $NONEXISTENT_DIR" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (with non-existent directory)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (with non-existent directory)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

print_section_summary "Tests avec dossiers inexistants"

# Cleanup
rm -rf "$TMP_DIR"

# Summary
echo ""
echo -e "${YELLOW}=== RESUME FINAL ===${NC}"
echo "Total tests: $total_tests"
if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}Tous les tests ont reussi !${NC}"
    exit 0
else
    echo -e "${RED}Tests echoues: $failed_tests${NC}"
    exit 1
fi
