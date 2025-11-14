#!/bin/bash

# Script pour tester un seul test (le test 39 par exemple)
# Usage: ./test_single.sh 39

TEST_NUM=$1

if [ -z "$TEST_NUM" ]; then
    echo "Usage: ./test_single.sh <numero_test>"
    echo "Exemple: ./test_single.sh 39"
    exit 1
fi

# Créer un dossier de test identique
TMP_DIR=$(mktemp -d)
TEST_DIR="$TMP_DIR/test_dir"
mkdir -p "$TEST_DIR"
touch "$TEST_DIR/file_in_dir1" "$TEST_DIR/file_in_dir2"

echo "=== Test $TEST_NUM ==="
echo "Dossier de test : $TEST_DIR"
echo ""

# Calculer quelle commande correspond au test 39
# Test 1 : sans options
# Tests 2-32 : combinaisons sans arguments (31 tests)
# Test 33 : sans options avec dossier
# Tests 34-64 : combinaisons avec dossier (31 tests)

if [ $TEST_NUM -eq 1 ]; then
    echo "Commande : ./ft_ls (sans options, sans arguments)"
    ./ft_ls > "$TMP_DIR/mine" 2>&1
    LC_ALL=C /bin/bash -c "ls" > "$TMP_DIR/real" 2>&1
elif [ $TEST_NUM -ge 2 ] && [ $TEST_NUM -le 32 ]; then
    # Tests 2-32 : combinaisons sans arguments
    # Calculer quelle combinaison
    options=("a" "r" "R" "t" "l")
    combo_index=$((TEST_NUM - 2))
    combo=""
    # Générer la combinaison (simplifié)
    if [ $combo_index -lt 5 ]; then
        combo="${options[$combo_index]}"
    elif [ $combo_index -lt 10 ]; then
        # Combinaisons de 2
        idx=$((combo_index - 5))
        if [ $idx -eq 0 ]; then combo="ar"; fi
        if [ $idx -eq 1 ]; then combo="aR"; fi
        if [ $idx -eq 2 ]; then combo="at"; fi
        if [ $idx -eq 3 ]; then combo="al"; fi
        if [ $idx -eq 4 ]; then combo="rR"; fi
    fi
    echo "Commande : ./ft_ls -$combo (sans arguments)"
    ./ft_ls -$combo > "$TMP_DIR/mine" 2>&1
    LC_ALL=C /bin/bash -c "ls -$combo" > "$TMP_DIR/real" 2>&1
elif [ $TEST_NUM -eq 33 ]; then
    echo "Commande : ./ft_ls $TEST_DIR (sans options, avec dossier)"
    ./ft_ls "$TEST_DIR" > "$TMP_DIR/mine" 2>&1
    LC_ALL=C /bin/bash -c "ls $TEST_DIR" > "$TMP_DIR/real" 2>&1
elif [ $TEST_NUM -ge 34 ] && [ $TEST_NUM -le 64 ]; then
    # Tests 34-64 : combinaisons avec dossier
    # Test 34 = -a, 35 = -r, 36 = -R, 37 = -t, 38 = -l, 39 = -ar, etc.
    options=("a" "r" "R" "t" "l")
    combo_index=$((TEST_NUM - 34))
    
    # Générer toutes les combinaisons dans le même ordre que le script
    all_combos=()
    for size in 1 2 3 4 5; do
        total=$((1 << 5))
        i=0
        while [ $i -lt $total ]; do
            combo=""
            count=0
            j=0
            while [ $j -lt 5 ]; do
                if [ $((i & (1 << j))) -ne 0 ]; then
                    combo="${combo}${options[j]}"
                    count=$((count + 1))
                fi
                j=$((j + 1))
            done
            if [ $count -eq $size ] && [ -n "$combo" ]; then
                all_combos+=("$combo")
            fi
            i=$((i + 1))
        done
    done
    
    combo="${all_combos[$combo_index]}"
    echo "Commande : ./ft_ls -$combo $TEST_DIR"
    ./ft_ls -$combo "$TEST_DIR" > "$TMP_DIR/mine" 2>&1
    LC_ALL=C /bin/bash -c "ls -$combo $TEST_DIR" > "$TMP_DIR/real" 2>&1
else
    echo "Test numero $TEST_NUM non gere"
    rm -rf "$TMP_DIR"
    exit 1
fi

echo ""
echo "=== Sortie de ft_ls ==="
cat "$TMP_DIR/mine"
echo ""

echo "=== Sortie de ls (bash + LC_ALL=C) ==="
cat "$TMP_DIR/real"
echo ""

echo "=== Comparaison ==="
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "\033[0;32mIDENTIQUE - Test reussi !\033[0m"
else
    echo -e "\033[0;31mDIFFERENT - Test echoue !\033[0m"
    echo ""
    echo "=== Diff detaille ==="
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
fi

rm -rf "$TMP_DIR"

