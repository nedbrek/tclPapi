/*
  * hello.c -- A minimal Tcl C extension.
  */
  
  #include<stdlib.h>
 #include <tcl.h>
#include <papi.h>
#include<string.h>

 static int 
 Hello_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
 {	
 
 if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) 
  {
     printf("Error initializing the PAPI Library");
  exit(1);
  }

int event_set = PAPI_NULL;
  
 if (PAPI_create_eventset(&event_set) != PAPI_OK) {
    printf("Error in subroutine PAPIF_create_eventset");
	exit(1);
}


 
 char* arg=Tcl_GetString(objv[1]);


 int argc, code;

const char **argv;

code = Tcl_SplitList(interp, arg, &argc,  &argv);

char **index=argv;

int numberofevents=0;
int event_code;



char**res;
  //  allocate storage for an array of pointers 
  res = malloc(300 * sizeof(char*));

  // for each pointer, allocate storage for an array of char
  int i;
  for ( i = 0; i < 300; i++) {
    res[i] = malloc(100 * sizeof(char));
  }



while(*index!=NULL)
{

if (PAPI_event_name_to_code(*index,&event_code) != PAPI_OK)
printf("error in event name: %d\n",PAPI_event_name_to_code(*index,&event_code));
/*char strerr[100];
PAPI_perror(PAPI_event_name_to_code(*index,&event_code),strerr, 100);
printf("%s",strerr);
*/
res[numberofevents*3]=*index;
PAPI_add_event(event_set, event_code);
index+=1;
numberofevents+=1;
}
 
  // create the eventset
  
  
  


 // start counting 


  if(PAPI_start(event_set)!= PAPI_OK) {
 printf("Abort After PAPIF_start: ");
	exit(1);
	}
	
	
Tcl_EvalFile(interp, "test.tcl");
 //stop counting
 long_long values[10];


  if (PAPI_stop(event_set, values)!= PAPI_OK) {
  printf("Abort After PAPIF_stop: ");
	exit(1);
	}
    char str[21];
    int num=45;
    sprintf(str,"%lld",values[0]);


index=argv;
int cnt=0, cnt2=1;;
while(cnt<numberofevents)
{	    sprintf(res[cnt2],"%lld",values[cnt]);
	//strcat(res[cnt2],"\n");
//	printf("%s\n",res[cnt]);
	res[cnt2+1]="|";
//	printf("%s %lld\n",*index,values[cnt++]);
	index+=1;
	cnt++;
	cnt2+=3;
	}
	

char*result=Tcl_Merge(numberofevents*3-1, (const char *const *)res);

	 Tcl_SetObjResult(interp, Tcl_NewStringObj(result, -1));
     return TCL_OK;
 }

 
 int Perf_Init(Tcl_Interp *interp)
{
  // create namespace for our commands
  Tcl_Namespace *ns = Tcl_CreateNamespace(interp, "perf2", NULL, NULL);
  // tell Tcl to grab all subcommands on import
  Tcl_Export(interp, ns, "*", 0);

  // create our subcommand
  Tcl_CreateObjCommand(interp, "perf2::init", Hello_Cmd, NULL, NULL);

  // create the ensemble
  Tcl_CreateEnsemble(interp, "perf2", ns, 0);

  // success
  return TCL_OK;
}
