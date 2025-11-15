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
        echo -e "${YELLOW}Commande ft_ls: ./ft_ls -$opts $args${NC}"
        echo -e "${YELLOW}Commande ls: LC_ALL=C /bin/bash -c \"ls -$opts $args\"${NC}"
        echo -e "${RED}Difference:${NC}"
        echo "--- ft_ls output ---"
        cat "$TMP_DIR/mine"
        echo "--- ls output ---"
        cat "$TMP_DIR/real"
        echo "--- diff ---"
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

# Create files with special names for -- testing
SPECIAL_FILE1="$TMP_DIR/-special_file1"
SPECIAL_FILE2="$TMP_DIR/-special_file2"
touch "$SPECIAL_FILE1" "$SPECIAL_FILE2"

# Test with -- (end of options marker)
echo -e "${YELLOW}=== Tests avec -- (fin des options) ===${NC}"

# Test -- alone
echo "Test $test_num: -- alone"
./ft_ls -- > "$TMP_DIR/mine" 2>&1
LC_ALL=C /bin/bash -c "ls --" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (-- alone)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (-- alone)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test -- with directory
echo "Test $test_num: -- with directory"
./ft_ls -- "$TEST_DIR" > "$TMP_DIR/mine" 2>&1
LC_ALL=C /bin/bash -c "ls -- $TEST_DIR" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (-- with directory)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (-- with directory)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test -- with files
echo "Test $test_num: -- with files"
./ft_ls -- "$TEST_FILE1" "$TEST_FILE2" > "$TMP_DIR/mine" 2>&1
LC_ALL=C /bin/bash -c "ls -- $TEST_FILE1 $TEST_FILE2" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (-- with files)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (-- with files)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test -- with special files (starting with -)
echo "Test $test_num: -- with special files starting with -"
./ft_ls -- "$SPECIAL_FILE1" "$SPECIAL_FILE2" > "$TMP_DIR/mine" 2>&1
LC_ALL=C /bin/bash -c "ls -- $SPECIAL_FILE1 $SPECIAL_FILE2" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (-- with special files)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (-- with special files)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test options before --
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            echo "Test $test_num: -$combo --"
            ./ft_ls -$combo -- > "$TMP_DIR/mine" 2>&1
            LC_ALL=C /bin/bash -c "ls -$combo --" > "$TMP_DIR/real" 2>&1
            if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
                echo -e "${GREEN}test $test_num reussis (-$combo --)${NC}"
                section_passed=$((section_passed + 1))
            else
                echo -e "${RED}test $test_num erreur (-$combo --)${NC}"
                echo -e "${RED}Difference:${NC}"
                diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
                failed_tests=$((failed_tests + 1))
            fi
            test_num=$((test_num + 1))
            total_tests=$((total_tests + 1))
            section_total=$((section_total + 1))
        fi
    done < <(generate_combinations $size)
done

# Test options before -- with directory
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            echo "Test $test_num: -$combo -- with directory"
            ./ft_ls -$combo -- "$TEST_DIR" > "$TMP_DIR/mine" 2>&1
            LC_ALL=C /bin/bash -c "ls -$combo -- $TEST_DIR" > "$TMP_DIR/real" 2>&1
            if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
                echo -e "${GREEN}test $test_num reussis (-$combo -- with directory)${NC}"
                section_passed=$((section_passed + 1))
            else
                echo -e "${RED}test $test_num erreur (-$combo -- with directory)${NC}"
                echo -e "${RED}Difference:${NC}"
                diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
                failed_tests=$((failed_tests + 1))
            fi
            test_num=$((test_num + 1))
            total_tests=$((total_tests + 1))
            section_total=$((section_total + 1))
        fi
    done < <(generate_combinations $size)
done

# Test options before -- with files
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            echo "Test $test_num: -$combo -- with files"
            ./ft_ls -$combo -- "$TEST_FILE1" "$TEST_FILE2" > "$TMP_DIR/mine" 2>&1
            LC_ALL=C /bin/bash -c "ls -$combo -- $TEST_FILE1 $TEST_FILE2" > "$TMP_DIR/real" 2>&1
            if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
                echo -e "${GREEN}test $test_num reussis (-$combo -- with files)${NC}"
                section_passed=$((section_passed + 1))
            else
                echo -e "${RED}test $test_num erreur (-$combo -- with files)${NC}"
                echo -e "${RED}Difference:${NC}"
                diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
                failed_tests=$((failed_tests + 1))
            fi
            test_num=$((test_num + 1))
            total_tests=$((total_tests + 1))
            section_total=$((section_total + 1))
        fi
    done < <(generate_combinations $size)
done

# Test options before -- with special files
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            echo "Test $test_num: -$combo -- with special files"
            ./ft_ls -$combo -- "$SPECIAL_FILE1" "$SPECIAL_FILE2" > "$TMP_DIR/mine" 2>&1
            LC_ALL=C /bin/bash -c "ls -$combo -- $SPECIAL_FILE1 $SPECIAL_FILE2" > "$TMP_DIR/real" 2>&1
            if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
                echo -e "${GREEN}test $test_num reussis (-$combo -- with special files)${NC}"
                section_passed=$((section_passed + 1))
            else
                echo -e "${RED}test $test_num erreur (-$combo -- with special files)${NC}"
                echo -e "${RED}Difference:${NC}"
                diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
                failed_tests=$((failed_tests + 1))
            fi
            test_num=$((test_num + 1))
            total_tests=$((total_tests + 1))
            section_total=$((section_total + 1))
        fi
    done < <(generate_combinations $size)
done

print_section_summary "Tests avec -- (fin des options)"

# Test with --- (should be treated as a regular argument)
echo -e "${YELLOW}=== Tests avec --- (trois tirets) ===${NC}"

# Test --- alone
echo "Test $test_num: --- alone"
./ft_ls --- > "$TMP_DIR/mine" 2>&1
LC_ALL=C /bin/bash -c "ls ---" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (--- alone)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (--- alone)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test --- with directory
echo "Test $test_num: --- with directory"
./ft_ls --- "$TEST_DIR" > "$TMP_DIR/mine" 2>&1
LC_ALL=C /bin/bash -c "ls --- $TEST_DIR" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (--- with directory)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (--- with directory)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test --- with files
echo "Test $test_num: --- with files"
./ft_ls --- "$TEST_FILE1" "$TEST_FILE2" > "$TMP_DIR/mine" 2>&1
LC_ALL=C /bin/bash -c "ls --- $TEST_FILE1 $TEST_FILE2" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (--- with files)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (--- with files)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test options before ---
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            echo "Test $test_num: -$combo ---"
            ./ft_ls -$combo --- > "$TMP_DIR/mine" 2>&1
            LC_ALL=C /bin/bash -c "ls -$combo ---" > "$TMP_DIR/real" 2>&1
            if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
                echo -e "${GREEN}test $test_num reussis (-$combo ---)${NC}"
                section_passed=$((section_passed + 1))
            else
                echo -e "${RED}test $test_num erreur (-$combo ---)${NC}"
                echo -e "${RED}Difference:${NC}"
                diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
                failed_tests=$((failed_tests + 1))
            fi
            test_num=$((test_num + 1))
            total_tests=$((total_tests + 1))
            section_total=$((section_total + 1))
        fi
    done < <(generate_combinations $size)
done

# Test options before --- with directory
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            echo "Test $test_num: -$combo --- with directory"
            ./ft_ls -$combo --- "$TEST_DIR" > "$TMP_DIR/mine" 2>&1
            LC_ALL=C /bin/bash -c "ls -$combo --- $TEST_DIR" > "$TMP_DIR/real" 2>&1
            if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
                echo -e "${GREEN}test $test_num reussis (-$combo --- with directory)${NC}"
                section_passed=$((section_passed + 1))
            else
                echo -e "${RED}test $test_num erreur (-$combo --- with directory)${NC}"
                echo -e "${RED}Difference:${NC}"
                diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
                failed_tests=$((failed_tests + 1))
            fi
            test_num=$((test_num + 1))
            total_tests=$((total_tests + 1))
            section_total=$((section_total + 1))
        fi
    done < <(generate_combinations $size)
done

# Test options before --- with files
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            echo "Test $test_num: -$combo --- with files"
            ./ft_ls -$combo --- "$TEST_FILE1" "$TEST_FILE2" > "$TMP_DIR/mine" 2>&1
            LC_ALL=C /bin/bash -c "ls -$combo --- $TEST_FILE1 $TEST_FILE2" > "$TMP_DIR/real" 2>&1
            if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
                echo -e "${GREEN}test $test_num reussis (-$combo --- with files)${NC}"
                section_passed=$((section_passed + 1))
            else
                echo -e "${RED}test $test_num erreur (-$combo --- with files)${NC}"
                echo -e "${RED}Difference:${NC}"
                diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
                failed_tests=$((failed_tests + 1))
            fi
            test_num=$((test_num + 1))
            total_tests=$((total_tests + 1))
            section_total=$((section_total + 1))
        fi
    done < <(generate_combinations $size)
done

# Test mixed: options, --, then ---
echo "Test $test_num: -a -- ---"
./ft_ls -a -- --- > "$TMP_DIR/mine" 2>&1
LC_ALL=C /bin/bash -c "ls -a -- ---" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (-a -- ---)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (-a -- ---)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test mixed: options, --, then files
echo "Test $test_num: -a -- --- with files"
./ft_ls -a -- --- "$TEST_FILE1" > "$TMP_DIR/mine" 2>&1
LC_ALL=C /bin/bash -c "ls -a -- --- $TEST_FILE1" > "$TMP_DIR/real" 2>&1
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "${GREEN}test $test_num reussis (-a -- --- with files)${NC}"
    section_passed=$((section_passed + 1))
else
    echo -e "${RED}test $test_num erreur (-a -- --- with files)${NC}"
    echo -e "${RED}Difference:${NC}"
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

print_section_summary "Tests avec --- (trois tirets)"

# Test function for invalid options (should exit with code 2)
run_test_invalid_option() {
    local opts=$1
    local test_name="test $test_num"
    
    ./ft_ls -$opts > "$TMP_DIR/mine" 2>&1
    local ft_exit=$?
    LC_ALL=C /bin/bash -c "ls -$opts" > "$TMP_DIR/real" 2>&1
    local ls_exit=$?
    
    # Check exit codes match
    if [ $ft_exit -eq $ls_exit ]; then
        # Check error messages match (both should fail with invalid option)
        if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
            echo -e "${GREEN}$test_name reussis (-$opts, exit code: $ft_exit)${NC}"
            section_passed=$((section_passed + 1))
        else
            echo -e "${RED}$test_name erreur (-$opts)${NC}"
            echo -e "${YELLOW}Exit codes: ft_ls=$ft_exit, ls=$ls_exit${NC}"
            echo -e "${RED}Difference:${NC}"
            diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
            failed_tests=$((failed_tests + 1))
        fi
    else
        echo -e "${RED}$test_name erreur (-$opts) - codes de sortie differents${NC}"
        echo -e "${YELLOW}Exit codes: ft_ls=$ft_exit, ls=$ls_exit${NC}"
        echo -e "${RED}ft_ls output:${NC}"
        cat "$TMP_DIR/mine"
        echo -e "${RED}ls output:${NC}"
        cat "$TMP_DIR/real"
        failed_tests=$((failed_tests + 1))
    fi
    
    test_num=$((test_num + 1))
    total_tests=$((total_tests + 1))
    section_total=$((section_total + 1))
}

# Test function for invalid options with arguments
run_test_invalid_option_with_args() {
    local opts=$1
    local args=$2
    local test_name="test $test_num"
    
    ./ft_ls -$opts $args > "$TMP_DIR/mine" 2>&1
    local ft_exit=$?
    LC_ALL=C /bin/bash -c "ls -$opts $args" > "$TMP_DIR/real" 2>&1
    local ls_exit=$?
    
    if [ $ft_exit -eq $ls_exit ]; then
        if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
            echo -e "${GREEN}$test_name reussis (-$opts $args, exit code: $ft_exit)${NC}"
            section_passed=$((section_passed + 1))
        else
            echo -e "${RED}$test_name erreur (-$opts $args)${NC}"
            echo -e "${YELLOW}Exit codes: ft_ls=$ft_exit, ls=$ls_exit${NC}"
            echo -e "${RED}Difference:${NC}"
            diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
            failed_tests=$((failed_tests + 1))
        fi
    else
        echo -e "${RED}$test_name erreur (-$opts $args) - codes de sortie differents${NC}"
        echo -e "${YELLOW}Exit codes: ft_ls=$ft_exit, ls=$ls_exit${NC}"
        echo -e "${RED}ft_ls output:${NC}"
        cat "$TMP_DIR/mine"
        echo -e "${RED}ls output:${NC}"
        cat "$TMP_DIR/real"
        failed_tests=$((failed_tests + 1))
    fi
    
    test_num=$((test_num + 1))
    total_tests=$((total_tests + 1))
    section_total=$((section_total + 1))
}

# Test function to verify that invalid options after -- are treated as files
run_test_invalid_after_dashdash() {
    local file_path=$1
    local opts=$2
    local test_name="test $test_num"
    
    ./ft_ls -- "$file_path" > "$TMP_DIR/mine" 2>&1
    local ft_exit=$?
    LC_ALL=C /bin/bash -c "ls -- $file_path" > "$TMP_DIR/real" 2>&1
    local ls_exit=$?
    
    if [ $ft_exit -eq $ls_exit ]; then
        if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
            echo -e "${GREEN}$test_name reussis (-- -$opts, exit code: $ft_exit)${NC}"
            section_passed=$((section_passed + 1))
        else
            echo -e "${RED}$test_name erreur (-- -$opts)${NC}"
            echo -e "${YELLOW}Exit codes: ft_ls=$ft_exit, ls=$ls_exit${NC}"
            echo -e "${RED}Difference:${NC}"
            diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
            failed_tests=$((failed_tests + 1))
        fi
    else
        echo -e "${RED}$test_name erreur (-- -$opts) - codes de sortie differents${NC}"
        echo -e "${YELLOW}Exit codes: ft_ls=$ft_exit, ls=$ls_exit${NC}"
        failed_tests=$((failed_tests + 1))
    fi
    
    test_num=$((test_num + 1))
    total_tests=$((total_tests + 1))
    section_total=$((section_total + 1))
}

# Test with invalid options
echo -e "${YELLOW}=== Tests avec options invalides ===${NC}"

# Single invalid options
invalid_options=("x" "z" "X" "Z" "1" "9" "?" "@" "#" "$")
for opt in "${invalid_options[@]}"; do
    run_test_invalid_option "$opt"
done

# Invalid options in combinations (should fail on first invalid)
run_test_invalid_option "ax"
run_test_invalid_option "lx"
run_test_invalid_option "xR"
run_test_invalid_option "artx"
run_test_invalid_option "xlRta"

# Invalid options with valid directory
for opt in "${invalid_options[@]}"; do
    run_test_invalid_option_with_args "$opt" "$TEST_DIR"
done

# Invalid options with valid files
for opt in "${invalid_options[@]}"; do
    run_test_invalid_option_with_args "$opt" "$TEST_FILE1 $TEST_FILE2"
done

# Invalid options with non-existent files
for opt in "${invalid_options[@]}"; do
    run_test_invalid_option_with_args "$opt" "$NONEXISTENT_FILE"
done

print_section_summary "Tests avec options invalides"

# Test that invalid-looking options after -- are treated as files (not options)
echo -e "${YELLOW}=== Tests: options invalides apres -- (doivent etre traitees comme fichiers) ===${NC}"

# Create a file that looks like an invalid option
INVALID_OPT_FILE="$TMP_DIR/-x"
touch "$INVALID_OPT_FILE"

# Test that -x after -- is treated as a file name, not an option
echo "Test $test_num: -- -x (should be treated as file)"
./ft_ls -- "$INVALID_OPT_FILE" > "$TMP_DIR/mine" 2>&1
ft_exit=$?
LC_ALL=C /bin/bash -c "ls -- $INVALID_OPT_FILE" > "$TMP_DIR/real" 2>&1
ls_exit=$?
if [ $ft_exit -eq $ls_exit ]; then
    if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
        echo -e "${GREEN}test $test_num reussis (-- -x treated as file)${NC}"
        section_passed=$((section_passed + 1))
    else
        echo -e "${RED}test $test_num erreur (-- -x)${NC}"
        echo -e "${RED}Difference:${NC}"
        diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
        failed_tests=$((failed_tests + 1))
    fi
else
    echo -e "${RED}test $test_num erreur (-- -x) - codes de sortie differents${NC}"
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test with valid options before -- and invalid-looking option after
echo "Test $test_num: -a -- -x (should treat -x as file)"
./ft_ls -a -- "$INVALID_OPT_FILE" > "$TMP_DIR/mine" 2>&1
ft_exit=$?
LC_ALL=C /bin/bash -c "ls -a -- $INVALID_OPT_FILE" > "$TMP_DIR/real" 2>&1
ls_exit=$?
if [ $ft_exit -eq $ls_exit ]; then
    if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
        echo -e "${GREEN}test $test_num reussis (-a -- -x)${NC}"
        section_passed=$((section_passed + 1))
    else
        echo -e "${RED}test $test_num erreur (-a -- -x)${NC}"
        echo -e "${RED}Difference:${NC}"
        diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
        failed_tests=$((failed_tests + 1))
    fi
else
    echo -e "${RED}test $test_num erreur (-a -- -x) - codes de sortie differents${NC}"
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test multiple invalid-looking options after --
for opt in "${invalid_options[@]}"; do
    # Create file with that name
    INVALID_FILE="$TMP_DIR/-$opt"
    touch "$INVALID_FILE"
    run_test_invalid_after_dashdash "$INVALID_FILE" "$opt"
done

# Test with valid options before -- and invalid option after
for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            echo "Test $test_num: -$combo -- -x"
            ./ft_ls -$combo -- "$INVALID_OPT_FILE" > "$TMP_DIR/mine" 2>&1
            ft_exit=$?
            LC_ALL=C /bin/bash -c "ls -$combo -- $INVALID_OPT_FILE" > "$TMP_DIR/real" 2>&1
            ls_exit=$?
            if [ $ft_exit -eq $ls_exit ]; then
                if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
                    echo -e "${GREEN}test $test_num reussis (-$combo -- -x)${NC}"
                    section_passed=$((section_passed + 1))
                else
                    echo -e "${RED}test $test_num erreur (-$combo -- -x)${NC}"
                    echo -e "${RED}Difference:${NC}"
                    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
                    failed_tests=$((failed_tests + 1))
                fi
            else
                echo -e "${RED}test $test_num erreur (-$combo -- -x) - codes de sortie differents${NC}"
                failed_tests=$((failed_tests + 1))
            fi
            test_num=$((test_num + 1))
            total_tests=$((total_tests + 1))
            section_total=$((section_total + 1))
        fi
    done < <(generate_combinations $size)
done

print_section_summary "Tests: options invalides apres --"

# Test edge cases: empty option string, just a dash, etc.
echo -e "${YELLOW}=== Tests cas limites (options bizarres) ===${NC}"

# Create a file named "-" for testing
DASH_FILE="$TMP_DIR/-"
touch "$DASH_FILE"

# Test just "-" (should be treated as a file)
echo "Test $test_num: - (just a dash)"
./ft_ls "$DASH_FILE" > "$TMP_DIR/mine" 2>&1
ft_exit=$?
LC_ALL=C /bin/bash -c "ls $DASH_FILE" > "$TMP_DIR/real" 2>&1
ls_exit=$?
if [ $ft_exit -eq $ls_exit ]; then
    if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
        echo -e "${GREEN}test $test_num reussis (-)${NC}"
        section_passed=$((section_passed + 1))
    else
        echo -e "${RED}test $test_num erreur (-)${NC}"
        echo -e "${RED}Difference:${NC}"
        diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
        failed_tests=$((failed_tests + 1))
    fi
else
    echo -e "${RED}test $test_num erreur (-) - codes de sortie differents${NC}"
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test "-" with options before
echo "Test $test_num: -a - (dash after options)"
./ft_ls -a "$DASH_FILE" > "$TMP_DIR/mine" 2>&1
ft_exit=$?
LC_ALL=C /bin/bash -c "ls -a $DASH_FILE" > "$TMP_DIR/real" 2>&1
ls_exit=$?
if [ $ft_exit -eq $ls_exit ]; then
    if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
        echo -e "${GREEN}test $test_num reussis (-a -)${NC}"
        section_passed=$((section_passed + 1))
    else
        echo -e "${RED}test $test_num erreur (-a -)${NC}"
        echo -e "${RED}Difference:${NC}"
        diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
        failed_tests=$((failed_tests + 1))
    fi
else
    echo -e "${RED}test $test_num erreur (-a -) - codes de sortie differents${NC}"
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

# Test "--" with "-" after (should treat "-" as file)
echo "Test $test_num: -- - (dash after --)"
./ft_ls -- "$DASH_FILE" > "$TMP_DIR/mine" 2>&1
ft_exit=$?
LC_ALL=C /bin/bash -c "ls -- $DASH_FILE" > "$TMP_DIR/real" 2>&1
ls_exit=$?
if [ $ft_exit -eq $ls_exit ]; then
    if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
        echo -e "${GREEN}test $test_num reussis (-- -)${NC}"
        section_passed=$((section_passed + 1))
    else
        echo -e "${RED}test $test_num erreur (-- -)${NC}"
        echo -e "${RED}Difference:${NC}"
        diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
        failed_tests=$((failed_tests + 1))
    fi
else
    echo -e "${RED}test $test_num erreur (-- -) - codes de sortie differents${NC}"
    failed_tests=$((failed_tests + 1))
fi
test_num=$((test_num + 1))
total_tests=$((total_tests + 1))
section_total=$((section_total + 1))

print_section_summary "Tests cas limites"

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
