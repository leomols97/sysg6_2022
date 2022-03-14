/*---------------------------------------------------------------------------------------
NOM      : Exemple.c
CLASSE   : Applications - Latex - Illustration
OBJET    : Sert d'exemple pour inclure une source en latex
         : Dans ce ces, ce programme affiche Hello
HOWTO    : gcc Exemple.c -o Exemple; ./Exemple
AUTEUR   : J.C. Jaumain, le 3/11/2010
BUGS     :  /
REMARQUE : Impose lstset {escapechar=@\symbol{64}@} pour l'interprétation des balises latex
----------------------------------------------------------------------------------------*/
main() {
	int i;  // Pour récupérer le nombre de caractères écrits
	tab[10] Buffer='Hello'; // Le buffer
	i=write(1,Buffer,5); // La @$\frac{1}{2}$@ du buffer
	exit(0);
}