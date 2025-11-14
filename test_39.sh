#!/bin/bash

# Test simple du test 39 : ./ft_ls -ar [dossier]

echo "=== Test 39 : ./ft_ls -ar [dossier] ==="
echo ""

# Créer un dossier de test
TMP_DIR=$(mktemp -d)
TEST_DIR="$TMP_DIR/test_dir"
mkdir -p "$TEST_DIR"
touch "$TEST_DIR/file_in_dir1" "$TEST_DIR/file_in_dir2"

echo "Dossier de test : $TEST_DIR"
echo "Contenu du dossier :"
ls -la "$TEST_DIR"
echo ""

echo "=== Commande exacte du script ==="
echo "./ft_ls -ar $TEST_DIR"
echo "LC_ALL=C /bin/bash -c \"ls -ar $TEST_DIR\""
echo ""

echo "=== Sortie de ./ft_ls -ar ==="
./ft_ls -ar "$TEST_DIR" > "$TMP_DIR/mine" 2>&1
cat "$TMP_DIR/mine"
echo ""

echo "=== Sortie de ls -ar (bash + LC_ALL=C) ==="
LC_ALL=C /bin/bash -c "ls -ar $TEST_DIR" > "$TMP_DIR/real" 2>&1
cat "$TMP_DIR/real"
echo ""

echo "=== Comparaison ==="
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo -e "\033[0;32m✓ IDENTIQUE - Test reussi !\033[0m"
else
    echo -e "\033[0;31m✗ DIFFERENT - Test echoue !\033[0m"
    echo ""
    echo "=== Diff detaille ==="
    diff "$TMP_DIR/mine" "$TMP_DIR/real" || true
    echo ""
    echo "=== Comparaison hexadécimale (caractères invisibles) ==="
    echo "ft_ls :"
    hexdump -C "$TMP_DIR/mine" | head -10
    echo ""
    echo "ls :"
    hexdump -C "$TMP_DIR/real" | head -10
fi

rm -rf "$TMP_DIR"

