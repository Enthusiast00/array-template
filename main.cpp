#include "array.h"

Array<char> string("moon");
Array<char> string1('-');
Array<char> string2("pie");

Array<uint32_t> array;

int main(){

    for(uint32_t i=0;i<20;i++){ array.PushBack( rand()%89 ); }

    printf("pre sort\n");
    for(uint32_t i=0;i<10;i++){ printf("array[%u]: %u \n",i,array[i]); }
    array.Sort(0,array.Size());
    printf("post sort\n");
    for(uint32_t i=0;i<10;i++){ printf("array[%u]: %u \n",i,array[i]); }
    array.Clear();

    printf("str  %s\n",string.Data());
    printf("str1 %s\n",string1.Data());
    printf("str2 %s\n",string2.Data());

    Array<char> string3;

    string3 =':';
    string3 +=' ';

    string3 += string+string1;
    printf("str  %s\n",string3.Data());
    string3 += string2;
    printf("str  %s\n",string3.Data());

    return 0;
}
