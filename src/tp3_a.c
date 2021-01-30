#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "../include/pile.h"
#include "../include/json_tree.h"
#include "../include/tp2_lex.h"
#include "../include/tp3_a.h"


int traduction(int _lex){
	int JSON_T;
	switch(_lex){
		case 1:
			JSON_T = JSON_T_TRUE;
			break;
		case 2:
			JSON_T = JSON_T_FALSE;
			break;
		case 3:
			JSON_T = JSON_T_NULL;
			break;
		case 4:
			JSON_T = JSON_T_LCB;
			break;
		case 5:
			JSON_T = JSON_T_RCB;
			break;
		case 6:
			JSON_T = JSON_T_LB;
			break;
		case 7:
			JSON_T = JSON_T_RB;
			break;
		case 8:
			JSON_T = JSON_T_COMMA;
			break;
		case 9:
			JSON_T = JSON_T_COLON;
			break;
		case 10:
			JSON_T = JSON_T_STRING;
			break;
		case 11:
            JSON_T = JSON_T_NUMBER;
			break;
		case 12:
			JSON_T = JSON_T_NUMBER;
			break;
		default:
            JSON_T = JSON_T_ERROR;
			break;
	}
	return JSON_T;
}



int analyseurLR(TLex *_data)
{
    /* initialisation */
    TIntPile *pileInt;
    pileInt = initIntPile();
    int X;
    int n;
    empilerInt(pileInt, 0);

    /* on recupère un symbole en appelant l'analyseur lexicale puis en le traduisant pour l'analyseur syntaxique */
    int lexeme = traduction(lex(_data));

    /* boucle générale */
    int fini = 0;
    while (!fini)
    {   
        //on Affiche la pile (pour suivre l'avancement)
        printf("pile :");
        printIntPile(pileInt);

        //on regarde dans la table des actions, l'action à effectuer
        int action = tableAction[sommetInt(pileInt)][lexeme];


        //on Affiche successivement le sommet de la pile, le lexeme et l'action qui en découle
        printf("sommet : %d\n",sommetInt(pileInt));
        printf("lexeme : %d\n",lexeme);
        printf("action : %d\n",action);
        

        if (action == LR_A)
        {
            fini = 1;
            return 0;
        }
        else if (action == LR_D)
        {
            empilerInt(pileInt, tableAnalyse[sommetInt(pileInt)][lexeme]);
            lexeme = traduction(lex(_data)); /* on lit le symbole suivant */
        }
        else if (action == LR_E)
        {
            printf("pouloulou une erreur attention");
            return 1;
        }
        else
        { /*c'est une réduction, la table donne le numéro de la règle : n
            on depile autant d'états que d'éléments
            en partie droite de la règle */
            int i = 0;
	        printf("taille partie droite :%d\n",taillePartieDroiteRegle[tableAnalyse[sommetInt(pileInt)][lexeme]]);

            //on donne une valeur à X, qui dépend de la règle utilisé 
            n = tableAnalyse[sommetInt(pileInt)][lexeme];
            X = reglePartieGauche[n];
            
            int saveSommet = sommetInt(pileInt);
            while (i < taillePartieDroiteRegle[tableAnalyse[saveSommet][lexeme]])
            {
                depilerInt(pileInt);
                ++i;
            }

            //puis on empile le nouvel état 
            empilerInt(pileInt, tableAnalyse[sommetInt(pileInt)][X]); 
        }
    }











    
    /*else if (n == 4)
    { /* M -> P , M */
        /*O = dépiler de la PileJSon
            P = dépiler de la PileJSon
            Insérer la paire P dans l'objet O
            Empiler O sur PileJSon*/
    /*}
    else if (n == 5)
    { /* règle P -> s : V */
        /*Créer une paire P
            C = dépiler le conteneur de Valeur de la PileJSon
            s = dernier symbole lu par l'analyseur lexical (chaîne dans la
            table des symboles)
            insérer s et C dans P
            empiler P sur PileJSon
            effacer s de la table des symboles*/
    /*}
    else if (n == 6)
    {
    }
    else if (n == 7)
    {
    }
    else if (n == 8)
    {
    }
    else if (n == 9)
    {
    }
    else if (n == 10)
    {
    }
    else if (n == 11)
    {
    }
    else if (n == 12)
    {
    }
    else if (n == 13)
    {
    }
    else if (n == 14)
    {
    }
    else if (n == 15)
    {
    }
    else if (n == 16)
    {
    }*/
    /*

    /*
* continuer ainsi pour toutes les règles 6...16
*/
}

int main(int argc, char *argv[]){
    
	// Donne la taille du fichier
	char *filename = argv[1];

    if (!filename){
		filename = "json_test_files/test.json";
	}

	size_t file_len = 0;
	struct stat st;
	if(stat(filename, &st) == 0){
		file_len = st.st_size;
		printf("Taille du fichier (en octets) : %d\n", (int)file_len);
	}
	else{
		fprintf(stderr, "Erreur d'ouverture du fichier. '%s'\n", filename);
		return 1;
	}

	// Parcours le fichier pour en récupérer la chaîne
	FILE *file = fopen(filename, "r");
	if (!file){
		fprintf(stderr, "Erreur d'ouverture du fichier. '%s'\n", filename);
		return 1;
	}
	char *json = malloc(file_len); // Chaîne du contenu du fichier JSON
	char c;
	size_t i = 0;
	while ((c = fgetc(file)) != EOF){
		json[i] = (char) c;
		i++;
	}

	json[i] = '\0'; // Le fameux
    TLex *lexData = initLexData(json);
	int res = analyseurLR(lexData); // Traitement principal

	free(json);
	printf("Résultat : %d\n", res);

	return 0;
}
