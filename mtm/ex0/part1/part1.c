#include <stdio.h>
#include <stdlib.h>

int powFunc(int base, int power);

int main() {

    int size;
    printf("Enter size of input:\n");
    scanf("%d",&size);
    if(size<=0){
        printf("Invalid size\n");
        return 0;
    }

    int* numbers=malloc(sizeof(int)*size);
    if (numbers == NULL) return 0;

    printf("Enter numbers:\n");
    for(int i=0;i<size;i++) {
        if(scanf("%d",numbers+i)==0) {
            printf("Invalid number\n");
            free(numbers);
            return 0;
        }
    }

    int sum=0;

    for(int i=0;i<size;i++) {
        int power=0;
        while(powFunc(2,power)<=numbers[i]) {
            if(powFunc(2,power)==numbers[i]) {
                printf("The number %d is a power of 2: %d = 2^%d\n",numbers[i],numbers[i],power);
                sum+=power;
                break;
            }
            ++power;
        }
    }

    printf("Total exponent sum is %d\n",sum);

    free(numbers);

    return 0;
}

int powFunc(int base, int power) 
{
    int result = 1;
    for (int i = 0; i < power; i++) {
        result *= base;
    }
    return result;
}