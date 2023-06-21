/* Autor: Roberto Marabini */
/* This file implements a set of unitary and functional tests
 * (not necessarily complete) designed to test the
 * methods that you are required to implement in this assignment
 *
 * We recommend you to follow a Test-driven development (TDD)
 * that is, execute the first test, which will fail, then create the code
 * to fix the errors and rerun the tests. Once the first test is
 * satisfied and the requirements for the first method are met
 * uncomment the second test and repeat the process
 *
 * IMPORTANT: do NOT modify the code in this file
 * but you are welcomed to add new tests
 */

#include "tester.h"

#define TAMMAX 1000
#define TAMRegInd (int)(sizeof(char)*4+sizeof(int)*4)

void checkReplaceExtensionByIdx(const char * tableName,
                                const char * indexName);
void checkCreateIndex(const char * indexName);
void checkCreateTable(const char * tableName);

void checkPrint(const char * indexName);
void createTestIndexFile(const char * indexName);
void checkFindKey(const char * indexName);
void checkAddIndexEntry(const char * indexName);

int OffsetID(int ID){
    return (int)sizeof(int)*2+ID*TAMRegInd;
}
int inTable(int *array, int key, int tam){
    int i;
    if(!array||tam<0) return -1;
    for(i=0; i<tam; i++){
        if(array[i]==key) return 1;
    }
    return 0;
}
void impPal(char *palabra, int tam){
    int i;
    if(!palabra) return;

    for(i=0; i<tam; i++) printf("%c", palabra[i]);
}
void printData(const char * indexName, const char * tableName){
    FILE *fIndex=NULL, *fTable=NULL;
    int IDBorr[TAMMAX], nRegBor=0, tReg[TAMMAX], nReg, aux, i, j;
    char palabra[TAMMAX];

    if(!indexName||!tableName){
        printf("\nERROR AL IMPRIMIR LOS DATOS\n");
        return;
    }

    fIndex=fopen(indexName, "rb+");
    fTable=fopen(tableName, "rb+");
    if(!fIndex||!fTable){/*Control de errores*/
        printf("\nERROR EN LA APERTURA AL IMPRIMIR LOS DATOS\n");
        return;
    }
    
    /*Almacenamos los IDs de los registros borrados y el numero de ellos*/
    fseek(fIndex, sizeof(int), SEEK_SET);
    fread(&aux, sizeof(int), 1, fIndex);
    while(aux!=-1){
        IDBorr[nRegBor]=aux;
        fseek(fIndex, OffsetID(aux)+sizeof(char)*4, SEEK_SET);
        nRegBor++;
        fread(&aux, sizeof(int), 1, fIndex);
    }

    /*Almacenamos los IDs de los registros NO borrados*/
    fseek(fIndex, 0, SEEK_END);
    aux=ftell(fIndex);
    nReg=(aux-sizeof(int)*2)/TAMRegInd;/*Almacenamiento de número de IDs de regs, borrados y sin borrar*/

    for(i=0, j=0; i < nReg; i++){
        if(inTable(IDBorr, i, nRegBor)==0){
            tReg[j]=i;
            j++;
        }
    }
    nReg=j;

    /*Sustituimos los IDs por el Offset de los datos*/
    for(i=0; i < nReg; i++){
        fseek(fIndex, OffsetID(tReg[i]+1)-sizeof(int), SEEK_SET);
        fread(&aux, sizeof(int), 1, fIndex);
        tReg[i]=aux;
    }

    /*Imprimimos los registros libres*/
    printf("-------Información de los registros de indice libres-------");
    for(i=0; i < nRegBor; i++){
        fseek(fIndex, OffsetID(IDBorr[i]+1)-sizeof(int), SEEK_SET);
        fread(&aux, sizeof(int), 1, fIndex);
        printf("\nID(%d): %d ", IDBorr[i], aux);

        fseek(fTable, aux+sizeof(char)*4, SEEK_SET);
        fread(&aux, sizeof(int), 1, fTable);
        printf("%d", aux);
    }
    printf("\n\n");

    

    printf("\n-------Tabla de Datos-------");
    for(i=0; i < nReg; i++){
        fseek(fTable, tReg[i], SEEK_SET);
        fread(palabra, sizeof(char)*4, 1, fTable);
        fread(&aux, sizeof(int), 1, fTable);
        printf("\nLibro %d (%d): ", i+1, tReg[i]);
        impPal(palabra, 4);
        printf(" %d ", aux);
        fread(palabra, sizeof(char), aux, fTable);
        impPal(palabra, aux);
    }
    printf("\n\n");
    fclose(fIndex);
    fclose(fTable);
}


Node a[TEST_ARRAY_SIZE];
char sort_a[SORTED_TEST_ARRAY_SIZE][PK_SIZE+1];
Book b[TEST_ARRAY_SIZE];

int main() {
    const char *tableName = "myDataBase.dat";
    const char *indexName = "myDataBase.idx";
    Book libro;

    /* remove file if exists */
    remove(tableName);
    remove(indexName);

    /* Start testing */
    /* uncomment this test if you find the function
     * repalceExtensionByIdx() useful and you want to
     * implement it
     * checkReplaceExtensionByIdx(tableName, indexName); */
    checkCreateIndex(indexName);
    checkCreateTable(tableName);
    createTestFiles(tableName, indexName);
    checkPrint(indexName);
    checkFindKey(indexName);
    checkAddIndexEntry(indexName);

    printf("\n\n------- COMPROBAMOS LA INSERCION EN TABLA   -------\n\n");
    createTestDataFile(tableName);
    createTestIndexFile(indexName);

    printTree(10, indexName);
    printData(indexName, tableName);
    
    /*Insercion primer libro*/
    strcpy(libro.book_id,"LIB1");
    libro.title="Titulo";
    libro.title_len=6;

    if(false==addTableEntry(&libro, tableName, indexName)){
        printf("ERROR AL InSERTAR LIB1");
        return 1;
    }
    printf("\n\n-------TRAS INSERTAR LIB1-------\n");
    printTree(10, indexName);
    printData(indexName, tableName);/*Imprimimos por pantalla los libros y su offset despues de añadir los nuevos elementos*/

    /*Insercion segundo libro*/
    strcpy(libro.book_id,"LIB2");
    libro.title="TituloDe15carac";
    libro.title_len=15;

    if(false==addTableEntry(&libro, tableName, indexName)){
        printf("ERROR AL InSERTAR LIB2");
        return 1;
    }
    printf("\n\n-------TRAS INSERTAR LIB2-------\n");
    printTree(10, indexName);
    printData(indexName, tableName);/*Imprimimos por pantalla los libros y su offset despues de añadir los nuevos elementos*/

    /*Insercion tercer libro*/
    strcpy(libro.book_id,"LIB3");
    libro.title="T";
    libro.title_len=1;

    if(false==addTableEntry(&libro, tableName, indexName)){
        printf("ERROR AL InSERTAR LIB3");
        return 1;
    }
    printf("\n\n-------TRAS INSERTAR LIB3-------\n");
    printTree(10, indexName);
    printData(indexName, tableName);/*Imprimimos por pantalla los libros y su offset despues de añadir los nuevos elementos*/

    /*Insercion cuarto libro*/
    strcpy(libro.book_id,"LIB4");
    libro.title="A";
    libro.title_len=1;

    if(false==addTableEntry(&libro, tableName, indexName)){
        printf("ERROR AL InSERTAR LIB4");
        return 1;
    }
    printf("\n\n-------TRAS INSERTAR LIB4-------\n");
    printTree(10, indexName);
    printData(indexName, tableName);/*Imprimimos por pantalla los libros y su offset despues de añadir los nuevos elementos*/
  

    return 0;
#ifdef NEVERDEFINED
#endif
    return (0);
}
