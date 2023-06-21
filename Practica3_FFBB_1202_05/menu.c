#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#define BufferLength 512

void Menu();
int ShowMenu();
char * Use();
void Insert(char *c);
void Print();
bool CheckExtensionDAT(char *TableName);

int main(){
    Menu();
    return 0;
}
void Menu() {   
    int nChoice=0;
    char *tableName=NULL;
    
    do {
        nChoice = ShowMenu();
        switch (nChoice) {
            case 1: {
                if(tableName!=NULL){
                 free(tableName);

                }
               tableName=Use();
               if(tableName==NULL){
                 free(tableName);
                    return;
                }
            }
                break;

            case 2: {
                Insert(tableName);
            }
                break;

            case 3: {
                Print(tableName);
            }
                break;
            case 4: {
                printf("Hasta luego\n\n");
                free(tableName);
                return;
            }
        }
    } while (1==1);

    
}

int ShowMenu() { /*Muestra las distintas opciones del Menu y devuelve el valor introducido por el usuario + control de errores*/
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Use\n"
               " (2) Insert\n"
               " (3) Print\n"
               " (4) Exit\n\n"
               "Introduce el número que corresponde a tu elección: > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("Has introducido una opción incorrecta. Por favor intentelo de nuevo\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}
/*Pide un nombre de fichero de datos y lo almacenamos*/
char * Use(){
    char *TableName=NULL;   
    TableName=(char*)malloc(BufferLength*sizeof(char));
    if(!TableName){
        printf("Error en la reserva dinámica de memoria");
        return NULL;
    }
    do{
        printf("Introduce el nombre del fichero de datos con la extensión .dat: ");
        if(1!=scanf("%s",TableName)){
            printf("Error al leer el nombre de la tabla\n");
            free(TableName);
            return NULL;
        }
        if(!CheckExtensionDAT(TableName)){
            printf("Fichero sin la extension .dat.");
        }
    }while(!CheckExtensionDAT(TableName));
    
    (void)getchar();
    if(!createTable(TableName)){
        printf("Error al crear la tabla\n");
        free(TableName);

        return NULL;
    }
    return TableName;
}
/*Funcion que pide ID y titulo de libro y lo inserta*/
void Insert(char *TableName){
    Book *Libro;
    char *IndexName=NULL;
    if(!TableName){
        printf("Necesitas seleccionar que table quieres usar con el comando use\n");
        return;
    }
    IndexName=(char*)malloc(BufferLength*sizeof(char));
    if(!IndexName){
        printf("Error en la reserva dinámica de memoria\n");
        return ;
    }

   
    Libro =(Book*)malloc(1*sizeof(Book));
    if(!Libro){
        free(IndexName);
        printf("Error en la reserva dinámica de memoria\n");

        return;
    }
    printf("Introduce el id del libro: ");
    if(1!=scanf("%s",(Libro->book_id))){
        printf("Error al leer la clave del libro\n");
        free(Libro);

        return;
    }
    printf("Introduce el titulo del libro: ");
    Libro->title =(char*)malloc(BufferLength*sizeof(char));
    if(!Libro->title){
        free(IndexName);
        free(Libro);
        printf("Error en la reserva dinámica de memoria\n");

        return;
    }
    getchar();
    if(!fgets(Libro->title, BufferLength, stdin)){
        printf("Error al leer el titulo del libro\n");
        free(Libro);
        free(IndexName);
        return;
    }
    Libro->title_len=strlen(Libro->title);
    replaceExtensionByIdx(TableName,IndexName);
    
    addTableEntry(Libro,TableName,IndexName);
        
    
    free(Libro->title);
    free(Libro);
    free(IndexName);
    return;
}
/*Función que imprime el arbol de índices*/
void Print(char* TableName){
    char *IndexName;
    int level;
    if(!TableName){
        printf("Necesitas seleccionar que table quieres usar con el comando use\n");
        return;
    }
    IndexName=(char*)malloc(BufferLength*sizeof(char));
    if(!IndexName){
        printf("Error en la reserva dinámica de memoria\n");
        return ;
    }
    printf("Introduce la profundidad hasta donde quieres imprimir:");
    if(1!=scanf("%d",&level)){
        printf("Error al leer la profundidad hasta la que queremos imprimir\n");
        free(IndexName);
        return;
    }
    getchar();
    replaceExtensionByIdx(TableName,IndexName);
    printTree(level,IndexName);
    free(IndexName);
}
bool CheckExtensionDAT(char *TableName){
    int len;
    if(!TableName){
        return false;
    }
    len=strlen(TableName);  
    if(len<4){
        return false;
    }  
    return (TableName[len-1]=='t' &&TableName[len-2]=='a'&&TableName[len-3]=='d'&&TableName[len-4]=='.');
}
