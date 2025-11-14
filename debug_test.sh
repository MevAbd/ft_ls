#!/bin/bash

# Generate all combinations of options
options=("a" "r" "R" "t" "l")
TMP_DIR=$(mktemp -d)
test_num=1

# Function to generate all combinations of a given size
generate_combinations() {
    local size=$1
    local result=()
    
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

echo "=== EXPLICATION DES TESTS ==="
echo ""
echo "Structure des tests :"
echo "1. Test 1 : sans options, sans arguments"
echo "2. Tests 2-32 : toutes les combinaisons d'options (31 combinaisons), sans arguments"
echo "3. Test 33 : sans options, avec dossier"
echo "4. Tests 34-64 : toutes les combinaisons d'options (31 combinaisons), avec dossier"
echo ""

# Create test directory and files
TEST_DIR="$TMP_DIR/test_dir"
mkdir -p "$TEST_DIR"
touch "$TEST_DIR/file_in_dir1" "$TEST_DIR/file_in_dir2"

echo "=== DETAILS DU DOSSIER DE TEST ==="
echo "Chemin du dossier : $TEST_DIR"
echo "Contenu du dossier :"
ls -la "$TEST_DIR"
echo ""

echo "=== TESTS SANS ARGUMENTS ==="
echo "Test 1: ./ft_ls (sans options, sans arguments)"
test_num=2

for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            echo "Test $test_num: ./ft_ls -$combo (sans arguments)"
            test_num=$((test_num + 1))
        fi
    done < <(generate_combinations $size)
done

echo ""
echo "=== TESTS AVEC UN DOSSIER ==="
echo "Test $test_num: ./ft_ls $TEST_DIR (sans options, avec dossier)"
test_num=$((test_num + 1))

for size in 1 2 3 4 5; do
    while IFS= read -r combo; do
        if [ -n "$combo" ]; then
            echo "Test $test_num: ./ft_ls -$combo $TEST_DIR"
            if [ $test_num -eq 39 ]; then
                echo "  >>> C'EST LE TEST 39 ! <<<"
                echo "  Commande exacte : ./ft_ls -$combo $TEST_DIR"
                echo "  Equivalent ls   : ls -$combo $TEST_DIR"
                echo "  Contenu du dossier :"
                ls -la "$TEST_DIR"
            fi
            test_num=$((test_num + 1))
        fi
    done < <(generate_combinations $size)
done

echo ""
echo "Pour tester manuellement le test 39, execute :"
echo "./ft_ls -[options] $TEST_DIR"
echo "et compare avec :"
echo "ls -[options] $TEST_DIR"

# Cleanup
rm -rf "$TMP_DIR"

