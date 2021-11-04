#include <stdlib.h>
#include <stdio.h>

typedef struct{
    int x;
    int y;
}cord;

int calculaArea(cord a, cord b, cord c){
    int area = ((a.x*b.y)+(a.y*c.x)+(b.x*c.y)-(b.y*c.x)-(a.x*c.y)-(b.x*a.y))/2;
        if (area<0)
            area=-area;
    return area;
}

int somaArea(cord p1, cord p2, cord p3, cord aux){
	return (calculaArea(aux,p2, p3)+calculaArea(p1, aux, p3)+calculaArea(p1, p2, aux));
}

int procuraPonto(cord p1, cord p2, cord p3, cord aux){
    int area=calculaArea(p1, p2, p3);

    int newArea=somaArea(p1, p2, p3, aux);

    if(area>=newArea){
        return 1;
    } else return 0;
}

int main(){
    int y, x;
    cord p1, p2, p3, aux;

    p1.x=1;
    p1.y=4  ;

    p2.x=10;
    p2.y=95;

    p3.x=84;
    p3.y=26;


    for(y=100; y>=0; y--){
        for(x=0; x<100; x++){
            aux.x=y;
            aux.y=x;

            if(procuraPonto(p1, p2, p3, aux)==1)
                printf("*");
            else printf(" ");
        }
        printf("\n");
    }

    return 0;
}
