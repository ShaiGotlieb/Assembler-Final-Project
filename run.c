void firstRun(FILE* f){
    while(feof(f)){
        fgets(line, 80, f);
        sList = makeSplitList();
        readline(sLine, line);
        insertLineToTail(sList, sLine);
    }
    symbList = makeSymbolList();
    p = sList.head;
    sp = symbList.head;
     
    while(p){
        if(validLabel(p->label)){
            symbolflag = 1;
            SymbolTable tmp;
            index = validOpCode(p->opCode);
            if(index == DATA || index == STRING || index == MAT){
                tmp.label = (char*)malloc(sizeof(p->label));
                if(mallocValid(tmp.label)) return;
                strcat(tmp.label, p->label);
                tmp.addr = DC;
                L = memorySize(p);
                insertToDataT(p->vars, index);
                DC += L;
                tmp.ope = false;
                tmp.ext = false;
                tmp.next = NULL;
                insertSymbolToTail(symbList, tmp);
                p = p->next;
            }
            else if(index == EXTERN){
                tmp.label = (char*)malloc(sizeof(p->label));
                if(mallocValid(tmp.label)) return;
                strcat(tmp.label, p->label);
                tmp.addr = 0;
                tmp.ext = true;
                tmp.next = NULL;
                insertSymbolToTail(symbList, tmp);
                p = p->next;
            }
            else if(index <= OPERATIONS_NUM){
                tmp.label = (char*)malloc(sizeof(p->label));
                if(mallocValid(tmp.label)) return;
                strcat(tmp.label, p->label);
                tmp.addr = IC;
                L = memorySize(p);
                IC += L;
                tmp.ext = false;
                tmp.ope = true;
                tmp.next = NULL;
                insertSymbolToTail(symbList, tmp);
                p = p->next;
            }
            else{
                p = p->next;
                break;
            }
        }
        else{
            p = p->next;
            break;
        }
    }
    free(tmp.label);
 
    while(sp){
        if(sp.ext == false && sp.ope == false){
            sp.addr += IC;
            sp = sp.next;
        }
        else{
            sp = sp.next;
        }
    }
}
 
void secondRun(const SplitList* sList, FILE* fob, FILE* fext){
    p = sList.head;
    int index = validOpCode(p->opCode);
    char address[4];
    char addBase4[4];
 
 
    while(p){
        char firstWord[ONE_BYTE];
        if(index == DATA || index == STRING || index == MAT){
            p = p->next;
            break;
        }
        else if(index == ENTRY){
            sp = searchSymbol(symbList.head, p->label);
            addBase4 = convert_base4(sp->addr);
            address = convert_wierd4(addBase4);
            fprintf(fob, "\t%s\t%s\n", sp->label, address);
            p = p->next;
            break;
        }
        else if(index <= OPERATIONS_NUM){
             
        }
    }
}
