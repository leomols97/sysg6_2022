Présentation de fork(), vfork() & clone() par Léopold Mols
==========================================================

Rendez-vous dans le dossier nommé "MOLS_FORK_VFORK_CLONE" via un terminal



Pour regénérer un fichier pdf depuis le fichier LaTex
====================================================

1) Rendez-vous dans le dossier nommé "Documents", puis dans le dossier nommé "fork(), vfork() & clone()" via une fenêtre de terminal
2) Si 'pip3' n'est pas installé sur votre ordinateur, installez-le au préalable.
3) Installez le package 'Pygments' via la commande 'pip3 install pygments'.
4) Entrez et exécutez la commande "pdflatex -shell-escape fork_vfork_clone.tex" pour générer un fichier pdf nommé de la même manière qu'est nommé le fichier .tex

5) RÉEXÉCUTEZ CETTE COMMANDE !!!

6) Si le fichier pdf ne contient pas la table des matières ou que la table des matières ne contient pas d'élément cliquables, exécutez encore une fois la commande




Pour compiler les codes
=======================

1) Rendez-vous dans un des dossiers de code contenu dans le dossier nommé "codes" ("1-fork", "2-vfork" ou "3-clone")
2) Entrez et exécutez la commande "make"
3) Si la commande ne s'exécute pas correctement, entrez la commande "gcc -pthread -o <nomDuFichier> <nomDuFichier>.c". Ajoutez en fin de ligne l'option "-lm" si vous voulez compiler les fichiers nommés "forkMoreProcesses" ou "vforkMoreProcesses".
4) Exécutez le fichier objet que vous souhaitez exécuter en entrant et exécutant la commande "./<nomDuFichierAExecuter>".
5) Lisez bien les instructions affichées à l'écran.