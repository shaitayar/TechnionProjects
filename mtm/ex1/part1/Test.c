#include <stdio.h>
#include "amount_set_str.h"
#include "string.h"
int main() {
    printf("***************Test_1***************\n");
    AmountSet Set1 = asCreate();
    bool check=true;
    if(asRegister(Set1, "Madrid") != AS_SUCCESS){
        printf("Test 1 Failed at check_point 1\n");
        check=false;
    }
    if(strcmp(asGetFirst(Set1),"Madrid")!=0){
        printf("Test 1 Failed at check_point 2\n");
        check=false;
    }
    if(asRegister(Set1, "Madrid") != AS_ITEM_ALREADY_EXISTS){
        printf("Test 1 Failed at check_point 3\n");
        check=false;
    }
    if(asRegister(Set1, "yones") != AS_SUCCESS){
        printf("Test 1 Failed at check_point 4\n");
        check=false;
    }

    if(asRegister(Set1, "omran") != AS_SUCCESS){
        printf("Test 1 Failed at check_point 6\n");
        check=false;
    }
    if(asDelete(Set1, "omran") != AS_SUCCESS){
        printf("Test 1 Failed at check_point 7 \n");
    }
    if(asRegister(Set1, "omran") !=AS_SUCCESS){
        printf("Test 1 Failed at check_point 8\n");
        check=false;
    }
    if(asChangeAmount(Set1,"omran",18.57)!=AS_SUCCESS)
    {
        printf("Test 1 Failed at check_point 9\n");
        check=false;
    }
    double x=0;
    if(asGetAmount(Set1,"omran",&x)!=AS_SUCCESS)
    {
        printf("Test 1 Failed at check_point 10\n");
        check=false;
    }
    if(x!=18.57)
    {
        printf("Test 1 Failed at check_point 11\n");
        check=false;
    }
    if(asRegister(Set1, "N") !=AS_SUCCESS){
        printf("Test 1 Failed at check_point 12\n");
        check=false;
    }

    if(asGetSize(Set1)!=4)
    {
        printf("Test 1 Failed at check_point 13\n");
        check=false;
    }


    AmountSet CopySet= asCopy(Set1);
    if(CopySet==NULL)
    {
        asDestroy(Set1);
        printf("Test 1 Failed at check_point 14\n");
        check=false;
    }


    if(asRegister(CopySet, "yones") !=AS_ITEM_ALREADY_EXISTS){
        printf("Test 1 Failed at check_point 15\n");
        check=false;
    }
    if(asDelete(CopySet, "yones") !=AS_SUCCESS){
        printf("Test 1 Failed at check_point 16\n");
        check=false;
    }
    if(asDelete(CopySet, "yones") !=AS_ITEM_DOES_NOT_EXIST){
        printf("Test 1 Failed at check_point 17\n");
        check=false;
    }
    if(asRegister(CopySet, "Yuval") !=AS_SUCCESS){
        printf("Test 1 Failed at check_point 18\n");
        check=false;
    }
    if(asRegister(CopySet, "Kane") !=AS_SUCCESS){
        printf("Test 1 Failed at check_point 19\n");
        check=false;
    }

 if(asGetSize(CopySet)!=5)
 {
     printf("Test 1 Failed at check_point 20\n");
     check=false;
 }


      asDestroy(Set1);
    asDestroy(CopySet);

    if(check==false)
        printf("\n**************Test 1 failed************\n");
    if(check==true){
        printf("He haaaa");
        printf("\n**************Test 1 passed************\n");
    }

    return 0;
}
