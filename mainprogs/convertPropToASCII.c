/* smear_APE4D.c
 *
 * uses qcd-lib to parallely APE-smear configurations
 * in 4d
 *
 * Christos Kallidonis, Dec 2015
 ********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include <qcd.h>
#include <string.h>

int main(int argc,char* argv[])
{
   qcd_uint_4 i;
   int myid,numprocs, namelen;    
   char processor_name[MPI_MAX_PROCESSOR_NAME];
   int params_len;                               // needed to read inputfiles
   char *params = NULL;                          // needed to read inputfiles				 
   char param_name[qcd_MAX_STRING_LENGTH];       // name of parameter file 
   qcd_real_8 theta[4] = {M_PI,0.0,0.0,0.0};     // antiperiodic b.c. in time
   qcd_uint_2 L[4];
   qcd_uint_2 P[4];          
   qcd_geometry geo;
   qcd_propagator prop;
   char prop_name[qcd_MAX_STRING_LENGTH];
   char ASCIIprop_name[qcd_MAX_STRING_LENGTH];
     
   //set up MPI
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numprocs);         // num. of processes taking part in the calculation
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);             // each process gets its ID
   MPI_Get_processor_name(processor_name,&namelen); //   
   char *message;
          
   //////////////////// READ INPUT FILE /////////////////////////////////////////////
            
   if(argc!=2)
   {
      if(myid==0) fprintf(stderr,"No input file specified\n");
      MPI_Finalize();
      exit(EXIT_FAILURE);
   }

   strcpy(param_name,argv[1]);
   if(myid==0)
   {
      i=0;
      printf("opening input file %s\n",param_name);
      params=qcd_getParams(param_name,&params_len);
      if(params==NULL)
      {
         i=1;
      }
   }
   MPI_Bcast(&i,1,MPI_INT, 0, MPI_COMM_WORLD);
   if(i==1)
   {
      MPI_Finalize();
      exit(EXIT_FAILURE);
   }
   MPI_Bcast(&params_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
   if(myid!=0) params = (char*) malloc(params_len*sizeof(char));
   MPI_Bcast(params, params_len, MPI_CHAR, 0, MPI_COMM_WORLD);
   
   sscanf(qcd_getParam("<processors_txyz>",params,params_len),"%hd %hd %hd %hd",&P[0], &P[1], &P[2], &P[3]);
   sscanf(qcd_getParam("<lattice_txyz>",params,params_len),"%hd %hd %hd %hd",&L[0], &L[1], &L[2], &L[3]);
   if(qcd_initGeometry(&geo,L,P, theta, myid, numprocs))
   {
      MPI_Finalize();
      exit(EXIT_FAILURE);
   }   
   
   if(myid==0) printf(" Local lattice: %i x %i x %i x %i\n",geo.lL[0],geo.lL[1],geo.lL[2],geo.lL[3]);
                     	    
   strcpy(prop_name,qcd_getParam("<lime_propagator>",params,params_len));
   if(myid==0) printf("Got propagator file name: %s\n",prop_name);

   strcpy(ASCIIprop_name,qcd_getParam("<ASCII_propagator>",params,params_len));
   if(myid==0) printf("Got propagator file name: %s\n",ASCIIprop_name);

   free(params);
      
   ///////////////////////////////////////////////////////////////////////////
   int j=0,k=0;
   j += qcd_initPropagator(&prop, &geo);
   MPI_Allreduce(&j, &k, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
   if(k>0){
     if(myid==0) fprintf(stderr,"Error, not enough memory to load propagator\n");
     exit(EXIT_FAILURE);
   }

   //-Load the propagator
   if(qcd_getPropagator(prop_name,qcd_PROP_LIME, &prop)) exit(EXIT_FAILURE);
   if(myid==0) printf("propagator loaded\n");

   if(qcd_writePropagatorASCII(ASCIIprop_name,&prop)) exit(EXIT_FAILURE);
   if(myid==0) printf("propagator written in ASCII format\n");

   qcd_destroyPropagator(&prop);
   qcd_destroyGeometry(&geo);
   MPI_Finalize();
}
