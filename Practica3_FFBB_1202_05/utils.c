#include "utils.h"
#include <string.h>
#include <stdlib.h>

int no_deleted_registers = NO_DELETED_REGISTERS;
#define TAM_IDX 5 /*Tamaño en bytes de una fila del fichero index*/
#define TAMREG (int)(sizeof(int)*4+sizeof(char)*4)
bool findKeyRec(FILE *f, int * nodeIDOrDataOffset, int nodeID, const char * book_id);

void replaceExtensionByIdx(const char *fileName, char * indexName) {
    int len;
    if(!fileName||!indexName) return;

    strcpy(indexName, fileName);
    len=strlen(indexName);
    indexName[len-3]='i';
    indexName[len-2]='d';
    indexName[len-1]='x';

    return;
}
bool file_exist(const char * namefile){
    FILE *file=NULL;
    if((file=fopen(namefile,"r"))){
        fclose(file);
        return true;
    }
    return false;
}


bool createTable(const char * tableName) {
    FILE *f=NULL;
    char *indexName=NULL;

    if(!tableName) return false;
    
    indexName=(char *)malloc((strlen(tableName)+1)*sizeof(char));
    if(!indexName) {
        fclose(f);
        return false;
    }
    replaceExtensionByIdx(tableName,indexName);
    
    if(file_exist(tableName)){
        if(!file_exist(indexName)){
            if(!createIndex(indexName)){
                free(indexName);
                return false;
            }
            free(indexName);
            return true; 
        }

    }
    f=fopen(tableName, "wb+");
    if(!f)
        return false;

    
    fwrite(&no_deleted_registers,1,sizeof(int),f);
    if(!createIndex(indexName)){
        free(indexName);
        fclose(f);
        return false;
    }
    free(indexName);
    fclose(f);
    return true;
    }

bool createIndex(const char *indexName) {
    FILE *f=NULL;
    if(!indexName) return false;
    
    if(file_exist(indexName)) return true;
    f=fopen(indexName, "wb+");
    if(!f) return false;

    fwrite(&no_deleted_registers,1,sizeof(int),f);
    fwrite(&no_deleted_registers,1,sizeof(int),f);
    fclose(f);
    return true;
}

void printnode(size_t _level/*prof total*/, size_t level/*Prof nodo*/, FILE * indexFileHandler, int node_id, char side) {
    char info[5];
    int offset, i, hi, hd, err;
    if(_level<level)
        return;

    err=fseek(indexFileHandler, node_id*TAM_IDX*sizeof(int)+2*sizeof(int), SEEK_SET);/*Situamos el cursor en el renglon adecuado*/
    if(err!=0){
        return;
    }
    if(4!=fread(info, sizeof(char), 4, indexFileHandler)){
        return;
    }
    info[4]='\0';
    if(1!=fread(&hi, sizeof(int), 1, indexFileHandler)){
        return;
    }

    if(1!=fread(&hd, sizeof(int), 1, indexFileHandler)){
        return;
    }

    err=fseek(indexFileHandler,sizeof(int),SEEK_CUR);/*Nos saltamos el padre*/
    if(err!=0){
        return;
    }
    if(1!=fread(&offset, sizeof(int), 1, indexFileHandler)){
        return;
    }/*Guardamos los datos del fichero*/

    for(i=0; i<(int)level; i++) printf("    ");/*Tabula una vez por cada unidad de profundidad*/
    
    printf("%c %s (%d): %d\n", side, info, node_id, offset);
    if(hi!=-1) printnode(_level, level+1, indexFileHandler, hi, 'l');
    if(hd!=-1) printnode(_level, level+1, indexFileHandler, hd, 'r');

    return;
}

void printTree(size_t level, const char * indexName)
{
    FILE *f=NULL;
    int num;
    if(!indexName) return;
    f=fopen(indexName, "rb+");
    if(!f) return;
    fread(&num, sizeof(int), 1, f);
    if(num==-1){
        return;
    }
    printnode(level, 0, f, num, ' ');
    fclose(f);

    return;
}

bool findKey(const char * book_id, const char *indexName, int * nodeIDOrDataOffset)
 {
    FILE *f=NULL;
    int num[2],err;
    bool retorno;

    if(!book_id||!indexName||!nodeIDOrDataOffset) return false;
    f=fopen(indexName, "rb+");
    if(!f) return false;

    fread(&num, sizeof(int), 2, f);
    if(num[0]==-1){
        *nodeIDOrDataOffset=-1;
        fclose(f);
        return false;
    }
    err=fseek(f, 2*sizeof(int) + num[0]*(4*sizeof(int)+4*sizeof(char)), SEEK_SET);/*Situamos el cursor en el renglon adecuado*/
    if(err!=0){
        fclose(f);
        return false;
    }
    retorno=findKeyRec(f, nodeIDOrDataOffset, num[0], book_id);/*LLamamos a una funcion auxiliar recursiva*/
    fclose(f);

    return retorno;
 }

bool findKeyRec(FILE *f, int * nodeIDOrDataOffset, int nodeID, const char * book_id){
    char info[5];
    int cmp,err, datos[4], hijo, setO=2*sizeof(int) + nodeID*(4*sizeof(int)+4*sizeof(char));
    
    if(!f||!nodeIDOrDataOffset||nodeID<0) return false;/*Control de errores*/

    err=fseek(f, setO, SEEK_SET);/*Situamos el cursor en el renglon adecuado*/
    if(err!=0){
        return false;
    }
    fread(info, sizeof(char), 4, f);

    cmp=strncmp(info, book_id,PK_SIZE);/*Comparamos el valor que buscamos con el nodo en el que nos encontramos*/

    fread(datos, sizeof(int), 4, f);

    if(cmp==0){
        *nodeIDOrDataOffset=datos[3];
        return true;
    }

    if(cmp>0){/*Seleccionamos el id del nodo*/
        hijo=datos[0];
    }
    else{
        hijo=datos[1];
    }
    if(hijo==-1){ /*Si no tiene el hijo adecuado*/
        *nodeIDOrDataOffset=nodeID;
        return false;
    }

    return findKeyRec(f, nodeIDOrDataOffset, hijo, book_id);
 }

bool addIndexEntry(char * book_id,  int bookOffset, char const * indexName) {
    int parentID, elID=0, nuevoBorr,err;
    Node N;
    char palPadre[5];
    FILE *f;
    bool borr=true;

    if(!book_id||bookOffset<0||!indexName) return false;/*Control de errores*/

    if(true==findKey(book_id, indexName, &parentID)) return false;

    f=fopen(indexName, "rb+");
    if(!f) return false;

    if(parentID==-1){
        fwrite(&elID, sizeof(int),1, f);
        err=fseek(f, sizeof(int)*2, SEEK_SET);
        if(err!=0){
            fclose(f);

            return false;
        }
        strncpy(N.book_id,book_id, 4);
        N.left=-1;
        N.right=-1;
        N.parent=-1;
        N.offset=bookOffset;
        fwrite(&N, sizeof(Node),1, f);
    
        fclose(f);
        return true;
    }
    err=fseek(f, sizeof(int), SEEK_SET);
    if(err!=0){
        fclose(f);

        return false;;
    }
    fread(&elID, sizeof(int), 1, f);
    if(elID==-1){/*En caso de que no haya ningun espacio borrado*/
        fseek(f, 0, SEEK_END);
        elID=(ftell(f)-sizeof(int)*2)/TAMREG;
        borr=false;
    }

    /*Actualizamos el padre*/
    err=fseek(f, parentID*TAMREG+sizeof(int)*2, SEEK_SET);
    if(err!=0){
        fclose(f);

        return false;;
    }
    fread(palPadre, sizeof(char), 4, f);
    if(strncmp(palPadre, book_id,PK_SIZE)<0){/*Si es el hijo derecho hay que desplazar 4 bytes el cursor*/
        err=fseek(f, sizeof(int), SEEK_CUR);
        if(err!=0){
        fclose(f);

        return false;;
        }
    }
    fwrite(&elID, 1, sizeof(int), f);

    /*Actualizamos el primer espacio borrado*/
    if(borr==true){
        err=fseek(f, elID*TAMREG+sizeof(int)*2+sizeof(char)*4, SEEK_SET);
        if(err!=0){
            fclose(f);

            return false;;
        }
        fread(&nuevoBorr, 1, sizeof(int), f);
        err=fseek(f, sizeof(char)*4, SEEK_SET);
        if(err!=0){
            fclose(f);

            return false;;
        }
        fwrite(&nuevoBorr, 1, sizeof(int), f);
    }
    
    /*Escribimos el nuevo indice en el lugar pertinente*/
    err=fseek(f, elID*TAMREG+sizeof(int)*2, SEEK_SET);
    if(err!=0){
        fclose(f);

        return false;;
    }
    strncpy(N.book_id,book_id, 4);
    N.left=-1;
    N.right=-1;
    N.parent=parentID;
    N.offset=bookOffset;
    fwrite(&N, 1, sizeof(Node), f);
    fclose(f);

    return true;
}
/*r*/
int findReg(FILE *f, int tam, int offSet, int *fatherOffset){
    int sigB, tamDisp,err;
    if(!f||tam<0||offSet<0) return -1;
    
    err=fseek(f, offSet, SEEK_SET);
    if(err!=0){
        return 0;
    }
    fread(&sigB, 1, sizeof(int), f);
    fread(&tamDisp, 1, sizeof(int), f);
    if(tamDisp>=tam){
        return offSet;
    }
    *fatherOffset=offSet;

    return findReg(f, tam, sigB, fatherOffset);
}

bool addTableEntry(Book * book, const char * dataName, const char * indexName) {
    FILE *f=NULL;
    int bookOffset, fatherOffset=-1, tamDisp, offAux,err;

    if(!book||!dataName||!indexName){
        printf("Error en al insertar, parametros de entrada incorrectos\n");
        return false;
    }
    if(true==findKey(book->book_id, indexName, &bookOffset)){
        printf("El libro ya está en la tabla\n");
      return false;/*Buscamos si ya esta almacenado y si no el nodo padre*/  
    } 

    f=fopen(dataName, "rb+");
    if(!f) return false;
    if(bookOffset==-1){ /*Si el arbol esta vacio*/
        err=fseek(f, 2*sizeof(int), SEEK_SET);
        if(err!=0){
            fclose(f);
            return false;
        }
        fwrite(&book->book_id, 4, sizeof(char), f);
        fwrite(&book->title_len, 1, sizeof(int), f);
        fwrite(book->title, book->title_len, sizeof(char), f);
        fclose(f);
        return addIndexEntry(book->book_id,  2*sizeof(int), indexName);
    }
    fread(&bookOffset, 1, sizeof(int), f);/*Guardamos el offset del primer registro borrado*/
    if(bookOffset!=-1){/*Si no hay registros borrados */
        err=fseek(f, bookOffset+sizeof(int), SEEK_SET);
        if(err!=0){
            fclose(f);
            return false;
        }
        fread(&tamDisp, 1, sizeof(int), f);
        if(tamDisp<(int)book->title_len){ 
            bookOffset=findReg(f, book->title_len, bookOffset, &fatherOffset);/*LLamamos a findReg*/
        }else fatherOffset=0;
    }
    

    if(bookOffset==-1){/*Si no hay registros borrados*/
        err=fseek(f, 0, SEEK_END);
        if(err!=0){
            fclose(f);

            return false;
        }
        bookOffset=ftell(f);
    }
    else{
        err=fseek(f, bookOffset, SEEK_SET);
        if(err!=0){
            fclose(f);

            return false;
        }
        fread(&offAux, 1, sizeof(int), f);
        fread(&tamDisp, 1, sizeof(int), f);
        if(tamDisp>(int)(sizeof(char)*4+sizeof(int)+book->title_len)){/*Si hay espacio hay hueco libre suficente se indica como hueco disponible*/
            err=fseek(f, bookOffset+sizeof(char)*4+sizeof(int)+book->title_len, SEEK_SET);
            if(err!=0){
                fclose(f);

                return false;
            }
            fwrite(&offAux, 1, sizeof(int), f);
            tamDisp=tamDisp-book->title_len-sizeof(char)*4-sizeof(int);
            fwrite(&tamDisp, 1, sizeof(int), f);

            offAux=bookOffset+sizeof(char)*4+sizeof(int)+book->title_len;
        }
        /*Después conecta el anterior con el siguiente*/
        err=fseek(f, fatherOffset, SEEK_SET);
        if(err!=0){
                fclose(f);

                return false;
            }
        fwrite(&offAux, 1, sizeof(int), f);
    }

    /*Escribimos en el lugar apropiado*/
    err=fseek(f, bookOffset, SEEK_SET);
    if(err!=0){
        fclose(f);

        return false;
    }
    fwrite(&book->book_id, 4, sizeof(char), f);
    fwrite(&book->title_len, 1, sizeof(int), f);
    fwrite(book->title, book->title_len, sizeof(char), f);
    fclose(f);
    
    return addIndexEntry(book->book_id,  bookOffset, indexName);
}
