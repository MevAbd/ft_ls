ft_ls

ft_ls est une ré-implémentation personnalisée de la commande Unix ls.
Le projet permet d’afficher le contenu d’un répertoire en reproduisant le comportement de la commande originale, ainsi que plusieurs flags importants.

🛠️ Compilation

Pour compiler le projet :
make
Cela génère l’exécutable ft_ls.

▶️ Utilisation

Pour lancer le programme :
./ft_ls
Cette commande liste le contenu du répertoire courant, comme le ferait ls.

Vous pouvez également lui passer des options et/ou un chemin, exactement comme pour la commande ls :
./ft_ls [options] [fichier|répertoire]

📌 Options disponibles

Le programme gère plusieurs flags classiques :

Flag	Description
-a	Affiche tous les fichiers, y compris ceux commençant par . (fichiers cachés).
-l	Affiche le contenu sous forme de liste détaillée (droits, propriétaire, taille, date…).
-t	Trie les fichiers par date de modification (du plus récent au plus ancien).
-r	Inverse l’ordre du tri (équivalent de ls -r).
-R	Affiche le contenu des sous-répertoires de manière récursive.

Les flags peuvent être combinés librement, comme avec ls :
./ft_ls -lart

🌍 Important : environnement LC_ALL=C

Pour obtenir un comportement identique à la commande ls, et afin d’assurer un tri cohérent (notamment pour les caractères spéciaux), ft_ls doit être testé avec la variable d’environnement suivante :
LC_ALL=C ls

🧪 Testeur automatique

Le projet inclut un script de test nommé testeurLs.sh.
Il compare le comportement de ft_ls avec celui de la vraie commande ls sur un grand nombre de combinaisons de flags.

Pour le lancer :
./testeurLs.sh
Il exécutera automatiquement toutes les combinaisons possibles d’options afin de vérifier que ft_ls se comporte correctement.

✔️ Objectif pédagogique

Ce projet permet de :

Comprendre la manipulation des fichiers et répertoires (API système : opendir, readdir, stat…).

Gérer le tri, les métadonnées et l’affichage formaté.

Manipuler les structures de données pour organiser les fichiers.

Reproduire un comportement Unix réel.
