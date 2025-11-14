#!/bin/bash

# Créer un dossier de test identique à celui du script
TMP_DIR=$(mktemp -d)
TEST_DIR="$TMP_DIR/test_dir"
mkdir -p "$TEST_DIR"
touch "$TEST_DIR/file_in_dir1" "$TEST_DIR/file_in_dir2"

echo "=== Test 39 : ./ft_ls -ar [dossier] ==="
echo ""
echo "Dossier testé : $TEST_DIR"
echo "Contenu du dossier :"
ls -la "$TEST_DIR"
echo ""

echo "=== Sortie de ./ft_ls -ar ==="
./ft_ls -ar "$TEST_DIR" > "$TMP_DIR/mine" 2>&1
cat "$TMP_DIR/mine"
echo ""

echo "=== Sortie de ls -ar (avec LC_ALL=C et bash) ==="
LC_ALL=C /bin/bash -c "ls -ar $TEST_DIR" > "$TMP_DIR/real" 2>&1
cat "$TMP_DIR/real"
echo ""

echo "=== Différence ==="
if diff "$TMP_DIR/mine" "$TMP_DIR/real" > /dev/null 2>&1; then
    echo "Aucune différence !"
else
    diff "$TMP_DIR/mine" "$TMP_DIR/real"
    echo ""
    echo "=== Comparaison hexadécimale (pour voir les caractères invisibles) ==="
    echo "ft_ls :"
    hexdump -C "$TMP_DIR/mine" | head -20
    echo ""
    echo "ls :"
    hexdump -C "$TMP_DIR/real" | head -20
fi

echo ""
echo "=== Test avec ls normal (sans LC_ALL=C et bash) ==="
ls -ar "$TEST_DIR" > "$TMP_DIR/normal" 2>&1
echo "Sortie :"
cat "$TMP_DIR/normal"
echo ""
if diff "$TMP_DIR/mine" "$TMP_DIR/normal" > /dev/null 2>&1; then
    echo "Aucune différence avec ls normal !"
else
    echo "Différence avec ls normal :"
    diff "$TMP_DIR/mine" "$TMP_DIR/normal"
fi

# Cleanup
rm -rf "$TMP_DIR"

