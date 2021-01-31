#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "../include/pile.h"
#include "../include/json_tree.h"
#include "../include/tp2_lex.h"
#include "../include/tp3_a.h"

/**
 * \fn int traduction(int _lex)
 * \brief fonction qui traduit l'element en sortie de l'analyse lexicale en élément pour l'analyse syntaxique 
 *
 * \param[inout] _lex un entier caca 
 * \return une valeur entre 0 et 11 qui represente chacun un symbole, un ou des caratère, un nombre ou une erreur
 */

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

/**
 * \fn int analyseurLR(TLex * _Data)
 * \brief fonction qui effectue l'analyse syntaxique
 *
 * \param[inout] _Data donnees de suivi de l'analyse syntaxique
 * \return 1 pour une erreur dans l'analyse et 0 si il n'y a eu aucune erreur
 */

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
    int fini = -1;
    while (fini==-1)
    {   
        //on Affiche la pile (pour suivre l'avancement)
        //printf("pile :");
        //printIntPile(pileInt);

        //on regarde dans la table des actions, l'action à effectuer
        int action = tableAction[sommetInt(pileInt)][lexeme];


        //on Affiche successivement le sommet de la pile, le lexeme et l'action qui en découle
        //printf("sommet : %d\n",sommetInt(pileInt));
        //printf("lexeme : %d\n",lexeme);
        //printf("action : %d\n",action);
        

        if (action == LR_A)
        {
            fini = 0;
        }
        else if (action == LR_D)
        {
            empilerInt(pileInt, tableAnalyse[sommetInt(pileInt)][lexeme]);
            lexeme = traduction(lex(_data)); /* on lit le symbole suivant */
        }
        else if (action == LR_E)
        {
            printf("Erreur de syntaxe...");
            fini = 1;
        }
        else
        { /*c'est une réduction, la table donne le numéro de la règle : n
            on depile autant d'états que d'éléments
            en partie droite de la règle */
            int i = 0;
	        //printf("taille partie droite :%d\n",taillePartieDroiteRegle[tableAnalyse[sommetInt(pileInt)][lexeme]]);

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
    deleteIntPile(&pileInt);
    
    return fini;
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

	// Parcours le fichier
	FILE *file = fopen(filename, "r");
	if (!file){
		fprintf(stderr, "Erreur d'ouverture du fichier. '%s'\n", filename);
        //fclose(file);
		return 1;
	}
	char *json = malloc(file_len+1);
	char c;
	size_t i = 0;
	while ((c = fgetc(file)) != EOF){
		json[i] = (char) c;
		i++;
	}

	json[i] = '\0';
    TLex *lexData = initLexData(json);
    
	int res = analyseurLR(lexData);
   
	free(json);
    printf("Résultat : %d\n", res);
    
    fclose(file);
    //erreur incompréhensible de segmentation fault
    deleteLexData(&lexData);
	return 0;
}
