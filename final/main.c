
#define toStr(x) #x
 
 
int main(int argc, char* argv[]){
    int i;
    char* fname;
    char* as;
    char* ob;
    char* ext;
 
    for(i = 1; i < argc; i++){
        createFileName(argv[i], *fname);
        FILE* f = fopen(toStr(fname), "r");
        if(!f){
            printf("Error: cannot open the file %s\n", fname);
            break;
        }
        createFileName(argv[i], *as, ".as");
        FILE* fas = fopen(toStr(as), "w");
        if(!fas){
            printf("Error: cannot open the file %s\n", fas);
            break;
        }
        createFileName(argv[i], *ob, ".ob");
        FILE* fob = fopen(toStr(ob), "w");
        if(!fob){
            printf("Error: cannot open the file %s\n", fob);
            break;
        }
        createFileName(argv[i], *ext, ".ext");
        FILE* fext = fopen(toStr(ext), "w");
        if(!f){
            printf("Error: cannot open the file %s\n", fext);
            break;
        }
 
        else{
            IC = 0;
            DC = 0;
            firstRun(fas);
            secondRun(sList, fob, fext);
 
        }
