/*
* To change this license header, choose License Headers in Project Properties.
* To change this template file, choose Tools | Templates
* and open the template in the editor.
*/

/*
* File:   main.c
* Author: frindel
*
* Created on 11 luglio 2016, 14.49
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/*allocazione dinamica di una matrice dati il numero di righe "rows" e di colonne "cols" 
passati come parametri*/

float **createMatrix(int rows, int cols) {

	float **A = (float **)calloc(rows, sizeof(float *));
	int i;
	for (i = 0; i < rows; i++)
		A[i] = (float *)calloc(cols, sizeof(float));
	
	return A;

}

/*deallocazione della matrice A, passata per riferimento come parametro, 
assieme al suo numero di righe "rows"*/

void freeMatrix(float** A, int rows) {
	int i;
	for (i = 0; i < rows; i++)
		free (A[i]);
	free(A);
}

void fillMatrix(int, int, float**);				//popola la matrice manualmente o con numeri generati casualmente
void printMatrix(float **, int, int);			//stampa a video la matrice;
float **sum(float **, float **, int, int);		//somma due matrici passate come parametro, assieme alle loro dimensioni
float **sub(float **, float **, int, int);		//sottrae una matrice dall'altra; le matrici e le loro dimensioni sono passate come parametri
float **mul(float **, float **, int, int, int); //moltiplica due matrici tra loro. L'algoritmo è quello classico, con T=O(n^3)
void trans(float **, int, int);					//traspone la matrice data, passata come parametro assieme alle sue dimensioni
int retry();									//torna al menu di scelta dell'operazione da compiere.

int main() {
	/*Ho preferito far scegliere il tipo di operazione prima di inserire le matrici,
	in modo da evitare incongruenze con le dimensioni inserite*/

	puts("OPERAZIONI SU MATRICI");
	int ans = -1, correct = 1;	//ans e correct sono due variabili di controllo del flusso

	do {
		while (ans == -1) {							//ans definisce il tipo di operazione da effettuare, 
													//inizializzata a -1 può assumere valori da 0 a 3
			
			puts("Scegliere il tipo di operazione: "); 
			puts("0: Somma");
			puts("1: Sottrazione");
			puts("2: Moltiplicazione");
			puts("3: Trasposizione");
			scanf("%d", &ans);
			fflush(stdin);
		}

		switch (ans) {				

			/*ans == 0: SOMMA DI MATRICI*/
			case(0): {
				/*rows e cols vengono definiti dall'utente per generare 
				  le matrici addende, di dimensione rows x cols*/

				int rows = 0; 
				int cols = 0;
			 
				puts("\nSomma di A e B");
				while ((rows<1) || (cols<1)) {
					puts("\nInserire il numero di righe di A: ");
					scanf("%d", &rows);
					fflush(stdin);
					puts("\nInserire il numero di colonne di A: ");
					scanf("%d", &cols);
					fflush(stdin);
				}
				/*Vengono create le matrici addende A e B, di numeri reali, 
				tramite chiamata alla funzione createMatrix (rows, cols), 
				che restituisce un puntatore a float**. Ciascuna matrice viene poi popolata
				tramite la funzione fillMatrix (rows, cols, <matrice>). 
				Viene allocata anche la matrice C[rows][cols] che ospita il risultato dell'operazione, 
				avente anch'essa le stesse dimensioni delle matrici adddende. 
				In seguito il risultato viene stampato con una chiamata a printMatrix (C, rows, cols)*/

				float **A = createMatrix(rows, cols);
				fillMatrix(rows, cols, A);
				float **B = createMatrix(rows, cols);
				fillMatrix(rows, cols, B);

				float **C = sum(A, B, rows, cols);
				puts("\nRISULTATO A + B: ");
				printMatrix(C, rows, cols);

				/*addendi e risultati, allocati dinamicamente e settati a 0 tramite calloc(), 
				  vengono eliminati dalla memoria al termine dell'operazione. 
				  Per deallocare una matrice occorre effettuare un'iterazione per ciascuna riga, 
				  deallocandone i vettori colonna tramite chiamata a free() su ciascuno di essi. 
				  Per questo motivo si passa a parametro il numero di righe della matrice da deallocare */
				
				freeMatrix(A, rows);
				freeMatrix(B, rows);
				freeMatrix (C, rows);

				//vengono settate le variabili del controllo di flusso
				//correct: è il flag che, settato a 0, permette al do-while
				//di riprendere da capo per effettuare un'altra operazioone
				//ans viene reimpostato a -1 per permettere la stampa a video del menu
				
				correct = retry();
				ans = -1;
				break;
			}
			//ans==1: DIFFERENZA DI MATRICI
			case(1): {
				int rows = 0; 
				int cols = 0;

				/*Stesse peculiarità della somma, a parte la chiamata alla funzione sub(),
				che prende in ingresso gli stessi parametri di sum()*/

				puts("\nDifferenza di A e B");
				while ((rows<1) || (cols<1)) {
					puts("\nInserire il numero di righe delle due matrici: ");
					scanf("%d", &rows);
					fflush(stdin);
					puts("\nInserire il numero di colonne delle due matrici: ");
					scanf("%d", &cols);
					fflush(stdin);
				}
				
				float **A = createMatrix(rows, cols);
				fillMatrix(rows, cols, A);
				float **B = createMatrix(rows, cols);
				fillMatrix(rows, cols, B);
				float **C = sub(A, B, rows, cols);

				//stampa a video del risultato
				puts("\nRISULTATO A - B: ");
				printMatrix(C, rows, cols);
				
				//deallocazione della memoria
				freeMatrix(A, rows);
				freeMatrix(B, rows);
				freeMatrix(C, rows);

				correct = retry();
				ans = -1;
				break;
			}
//ans == 2: PRODOTTO DI MATRICI
			case(2): {
				int rowsA = 0; 
				int colsA = 0; 
				int colsB = 0;

				/*qui la particolarità è che viene chiamata la funzione mul(A,B,rowsA,colsA,colsB)
				  che prende in ingresso le dimensioni di A e il numero di colonne di B, essendo la
				  matrice-prodotto di dimensioni (rowsA,colsB), sussistendo il vincolo colsA=rowsB */
				
				puts("\nProdotto di A e B");
				while ((rowsA<1) || (colsA<1)) {
					puts("\nInserire il numero di righe di A: ");
					scanf("%d", &rowsA);
					fflush(stdin);
					puts("\nInserire il numero di colonne di A: ");
					scanf("%d", &colsA);
					fflush(stdin);
				}

				while (colsB<1) {
					puts("\nInserire il numero di colonne di B: ");
					scanf("%d", &colsB);
					fflush(stdin);
				}

				float **A = createMatrix(rowsA, colsA);
				fillMatrix(rowsA, colsA, A);
				float **B = createMatrix(colsA, colsB);
				fillMatrix(colsA, colsB, B);
				float **C = mul(A, B, rowsA, colsA, colsB);
				puts("\nRISULTATO A x B:");
				printMatrix(C, rowsA, colsB); //C = AxB ha dimensione (rowsA, colsB)

				/*deallocazione della memoria. Notare che su B viene passato colsA come parametro,
				  su C viene passato rowsA */

				freeMatrix(A, rowsA);
				freeMatrix(B, colsA);
				freeMatrix(C, rowsA);
				correct = retry();
				ans = -1;
				break;
			}
//ans=3: TRASPOSIZIONE DELLA MATRICE
			case(3): {
				int rows = 0; 
				int cols = 0;
				puts("Trasposizione di A");

				/*La trasposizione chiama la funzione trans(), che si limita a stampare a video
				le righe di A* come colonne di A e viceversa. Notare che la matrice generata B
				ha dimensioni invertite rispetto all'originale, ma questo avviene dentro la funzione*/

				while ((rows<1) || (cols<1)) {
					puts("Inserire il numero di righe di A: ");
					scanf("%d", &rows);
					fflush(stdin);
					puts("Inserire il numero di colonne di A: ");
					scanf("%d", &cols);
					fflush(stdin);
				}

				float **A = createMatrix(rows, cols);
				fillMatrix(rows, cols, A);
				puts("\nRISULTATO A*: ");
				trans(A, rows, cols);
				freeMatrix(A, rows);
				
				correct = retry();
				ans = -1;
				break;
			}
//ans assume valori non consentiti: si ristampa il menu
			default: {
				puts("\nvalore errato. Riprovare: ");
				correct = 0;			//questo flag fa sì che il do-while riprenda da capo
				ans = -1;				//questo flag permette di ristampare a video il menu di scelta
				break;
			}

		}

	} while (correct == 0);

	fflush(stdin);

	getchar();
	return (EXIT_SUCCESS);
}

/*Tramite questa funzione, chiamata dal main dopo l'allocazione di una matrice operanda, 
  viene popolata la matrice, passata per riferimento assieme alle sue dimensioni passate per valore.
  Il popolamento può avvenire a mano, tramite inserimento di ciascun elemento 
  da parte dell'utente, oppure automaticamente, richiedendo all'utente solo 
  l'inserimento da tastiera di un valore massimo da generare tramite chiamata a rand()
  Le matrici generate sono, ovviamente, a elementi REALI */

void fillMatrix(int m, int n, float **A) {

	int i, j;						//contatori di ciclo
	int ans = -1;					//ans qui imposta l'inserimento manuale o automatico degli elementi

	while ((ans < 0) || (ans > 1)) {
		puts("\nInserimento manuale dei valori: premere 0");
		puts("Inserimento casuale dei valori: premere 1\n");
		scanf("%d", &ans);
	}
	
	//INSERIMENTO MANUALE
	if (ans == 0) {
		for (i = 0; i<m; i++) {
			for (j = 0; j<n; j++) {
				printf("\nInserire Elemento (%d, %d)", i + 1, j + 1);
				scanf("%f", &A[i][j]);
			}
		}
	}
	//INSERIMENTO CASUALE
	else {
		puts("\nInserire un valore massimo generabile: "); 
		int max;
		srand(time(NULL));			//inizializziamo il generatore di numeri pseudocasuali
		scanf("%d", &max);			//assegnandogli un valore da tastiera
		fflush(stdin);				//ripulire lo stdin dopo ogni scanf mette al riparo da brutte sorprese

		for (i = 0; i<m; i++) {
			for (j = 0; j<n; j++) {
				
				A[i][j] = ((float)rand() / (float)RAND_MAX) * (float)max; //rand()/RAND_MAX * max permette la generazione 
																		  //di numeri reali pseudocasuali da 0 a max
			}
			/*TODO: modificarla in modo da permettere l'aggiunta di numeri negativi*/
		}
	}

	printMatrix(A, m, n);
}

/*Questa funzione stampa a video una matrice, passata per riferimento assieme alle sue dimensioni,
 che vengono invece passate per valore*/

void printMatrix(float **A, int rows, int cols) {

	int i, j;
	for (i = 0; i<rows; i++) {
		for (j = 0; j<cols; j++) {
			
			printf("%f\t", A[i][j]);
		}
		printf("\n");
	}
}

/*questa funzione reimposta il flag correct, in modo da permettere l'esecuzione di un'altra
  operazione (0) oppure l'uscita dal programma (1)*/

int retry() {
	int replay = -1;

	do {
		puts("\nVuoi fare un'altra operazione (s=0|n=1)");
		scanf("%d", &replay);
		fflush(stdin);


		switch (replay) {
			case 0: {
				return 0;
				break;
			}
			case 1: {
				return 1;
				break;
			}
			default: {
				printf("valore errato");
				replay = -1;
			}
		}
	} while (!replay);
}

//------------------------SOMMA DI MATRICI------------------------
/*La funzione prende le due matrici addende A e B per riferimento,
  assieme alle loro dimensioni (devono essere uguali per entrambe
  perché A e B possano esseere sommate) e restituisce la matrice
  risultante C, i cui valori sono C[i][j]=A[i][j]+B[i][j]*/

float **sum(float **A, float **B, int rows, int cols) {
	int i, j;
	float **C = createMatrix(rows, cols);


	for (i = 0; i<rows; i++)
		for (j = 0; j<cols; j++) {
			C[i][j] = A[i][j] + B[i][j];
		}
	return C;
}

//------------------DIFFERENZA DI MATRICI------------------------//
/*La funzione prende le due matrici A e B per riferimento,
assieme alle loro dimensioni (devono essere uguali per entrambe
perché A e B possano esseere sommate) e restituisce la matrice
risultante C, i cui valori sono C[i][j] = A[i][j] - B[i][j]*/

float **sub(float **A, float **B, int rows, int cols) {
	int i, j;
	float **C = createMatrix(rows, cols);


	for (i = 0; i<rows; i++)
		for (j = 0; j<cols; j++) {
			
			C[i][j] = A[i][j] - B[i][j];
		}
	return C;
}

//---------------------PRODOTTO DI MATRICI------------------------//
/*La funzione prende le due matrici addende A e B per riferimento,
assieme alle dimensioni di A rowsA, colsA e al numero di colonne di B
(deve essere colsA = rowsB perché A e B possano esseere moltiplicate) 
e restituisce la matrice risultante C, i cui valori di ciascun elemento
sono dati dalla sommatoria C[i][j] += A[i][k]+B[k][j].
L'algoritmo è quello più intuitivo, avente complessità O(n^3) in quanto richiede
l'esecuzione di tre cicli for annidati*/

/*TODO: implementare l'algoritmo meno costoso, quello di complessità O(n^lg(7))*/

float **mul(float **A, float **B, int rowsA, int colsA, int colsB) {
	
	int i, j, k;					//contatori di ciclo
	float **C = createMatrix(colsA, colsB);


	for (i = 0; i<rowsA; i++)
		for (j = 0; j<colsB; j++){
			C[i][j] = 0;
			for (k = 0; k < colsA; k++) 
				C[i][j] += A[i][k]*B[k][j];
			}
	
	return C;
}

//------------------TRASPOSIZIONE DI MATRICE----------------------//
/*La funzione si limita a stampare a video gli elementi della
  matrice A passata per riferimento, con le sue dimensioni 
  rows, cols passate per valore. La stampa avviene tramite
  due cicli for annidati, il primo sulle colonne e il secondo sulle
  righe di A (di solito avviene il contrario), che stampano a 
  video l'elemento A[j][i] ad ogni passo della for interna, e un 
  newline a ogni passo del ciclo esterno */

void trans(float **A, int rows, int cols) {
	int i, j;
	
	for (i = 0; i<cols; i++){
		for (j = 0; j<rows; j++) 
			printf("%f\t", A[j][i]);
		printf("\n");
	}
}