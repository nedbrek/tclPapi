#include<Perft_common.h>

/*
 * Prototypes for procedures referenced only in this file:
 */
int 
RunScriptCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

 /*
 *______________________________________________
 *
 * RunScriptCmd --
 *
 * This function is to investigate the run time 
 * behaviour of a tcl script passes as command line argument
 * Before calling this function, InitCommand() must be called. 
 * Results:
 * The recorings of hardware counters is returned.
 *________________________________________________
 */
int 
RunScriptCmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    if(objc!=2) {
        fprintf(stderr,"Usage: perft::run_script script");
        exit(1);
    }        
    char* arg=Tcl_GetString(objv[1]);
    int counter=0,retval;
    char*result;
    
    StartCounting();

    retval=Tcl_EvalObjEx(interp, objv[1], TCL_EVAL_GLOBAL);
    if(retval) {
        printf("Error running the script\n");
        exit(1);
    }

    
    StopCounting();
    
    while(counter<papiState.numberOfEvents) {
        sprintf(papiState.eventsData[3*counter+1],"%lld",papiState.counterValues[counter]);
        papiState.eventsData[3*counter+2]=(char *)"|";
        counter++;
	}

    result=Tcl_Merge(papiState.numberOfEvents*3-1, (const char *const *)papiState.eventsData);
 
    Tcl_SetResult(interp,result,TCL_DYNAMIC);
    return TCL_OK;        
}    
