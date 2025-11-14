#!/bin/bash

# Test exact du test 39
TMP_DIR=$(mktemp -d)
TEST_DIR="$TMP_DIR/test_dir"
mkdir -p "$TEST_DIR"
touch "$TEST_DIR/file_in_dir1" "$TEST_DIR/file_in_dir2"

echo "=== Test exact du test 39 ==="
echo "Dossier : $TEST_DIR"
echo ""

echo "=== Commande exacte du script ==="
echo "LC_ALL=C /bin/bash -c \"ls -ar $TEST_DIR\""
echo ""

echo "=== Sortie de ft_ls ==="
./ft_ls -ar "$TEST_DIR" > "$TMP_DIR/mine" 2>&1
cat "$TMP_DIR/mine"
echo ""

echo "=== Sortie de ls (exactement comme le script) ==="
LC_ALL=C /bin/bash -c "ls -ar $TEST_DIR" > "$TMP_DIR/real" 2>&1
cat "$TMP_DIR/real"
echo ""

echo "=== Différence ==="
diff "$TMP_DIR/mine" "$TMP_DIR/real" || echo "Il y a une différence !"
echo ""

echo "=== Test avec guillemets autour du chemin ==="
LC_ALL=C /bin/bash -c "ls -ar \"$TEST_DIR\"" > "$TMP_DIR/real2" 2>&1
cat "$TMP_DIR/real2"
echo ""
diff "$TMP_DIR/mine" "$TMP_DIR/real2" && echo "Avec guillemets : identique !" || echo "Avec guillemets : différence !"

rm -rf "$TMP_DIR"

