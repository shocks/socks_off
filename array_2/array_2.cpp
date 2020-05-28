#include"array_2.h"
bool array_2::find(int* matrix, int rows, int columns, int number){
    bool found=false;
    if(matrix!=nullptr && rows > 0 && columns > 0){
        int row=0;
        int column=columns-1;
        while(row<rows && column>=0){
            if(number==matrix[row*columns+column]){
                found=true;
                break;
            }
            else{
                if(number<matrix[row*columns+column])
                    column--;         
                else 
                    row++;
            }
        }
    }
    return found;
}