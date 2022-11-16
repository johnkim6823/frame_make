#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fstream>
const char *path = "./yuv";
 
using namespace std;



int main()
{
   
    struct  dirent **namelist;
    int     count;
    int     idx;
    ofstream file3("cid.txt", ios::app);

    if((count = scandir(path, &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", path, strerror(errno));
        return 1;
    }

    for(idx = 0; idx < count; idx++) {
        printf("%s\n", namelist[idx]->d_name);
        file3 << namelist[idx]->d_name << endl;
    }

    // 건별 데이터 메모리 해제
    for(idx = 0; idx < count; idx++) {
        free(namelist[idx]);
    }

    // namelist에 대한 메모리 해제
    free(namelist);
    file3.close();
    return 0;

}