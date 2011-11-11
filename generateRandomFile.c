#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std ;

int main(){

    FILE *fp ;
    int r, count[4] ;
    fp = fopen("colors.txt", "w") ;

    string colors[4] ;
    colors[0] = "RED " ;
    colors[1] = "BLUE " ;
    colors[2] = "GREEN " ;
    colors[3] = "YELLOW " ;
    count[0] = count[1] = count[2] = count[3] = 0 ;

    for(int i = 0 ; i < 500 ; ++i){
	r = random() % 4 ;
	fwrite(colors[r].c_str(), 1, colors[r].length(), fp) ;
	++count[r] ;
    }
    fclose(fp) ;
    for(int i = 0 ; i < 4 ; ++i)
	printf("Color %s\t\t-\t%d\n", colors[i].c_str(), count[i]) ;
}
