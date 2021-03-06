

//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
#include <stdio.h>
#define PATH_MAX 2500
#if defined(__linux__) //linux
#define MYOS 1
#elif defined(_WIN32)
#define MYOS 2
#elif defined(_WIN64)
#define MYOS 3
#elif defined(__unix__) 
#define MYOS 4  // freebsd
#define PATH_MAX 2500
#else
#define MYOS 0
#endif
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <time.h>


char *fbasenoext(char* mystr)
{
    char *retstr;
    char *lastdot;
    if (mystr == NULL)
         return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;
    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';
    return retstr;
}




///////////////////////////////////////////
void readfile( char *filesource )
{
   FILE *source; 
   int ch ; 
   source = fopen( filesource , "r");
   while( ( ch = fgetc(source) ) != EOF )
   {
         printf( "%c", ch );
   }
   fclose(source);
}




///////////////////////////////////////////
void readfileline( char *fileoutput , char *filesource )
{
   FILE *source; 
   FILE *target; 
   int ch, c, co;
   char lline[PATH_MAX];
   char charo[PATH_MAX];
   int pcc = 0;
   int linecount = 0;
   int posx    = 100;
   int posy    = 730;
   int posxmax = 588;
   int posymax = 835;

   source = fopen( filesource , "r");
   target = fopen( fileoutput , "wb+");
   //fputs( "%!PS\n" , target );

   int export_skip = 0;
   int area_archive = 0;
   int fileeof = 0;
   while( fileeof == 0 )
   {
       export_skip = 0;
       c = fgetc(source); 
       if ( c == EOF ) fileeof = 1;

       else
       {

           if ( c != '\n' )
               lline[pcc++]=c;

           if ( ( lline[ 0 ] == '!' )  && ( lline[ 1 ] == 'E' ) && ( lline[ 2 ] == 'O' )  && ( lline[ 3 ] == 'F' )  )
              fileeof = 1;

           if ( ( lline[ 0 ] == '/' )  && ( lline[ 1 ] == '*' ) ) 
               area_archive = 1;

           if ( ( lline[ 0 ] == '*' )  && ( lline[ 1 ] == '/' ) ) 
           {    area_archive = 0;  export_skip = 1;       }

           if ( fileeof == 0 )
           if ( c == '\n' ) 
           {
             lline[pcc++]='\0';
             printf( "%s", lline );
             printf( "\n" );

             if ( area_archive == 0 )
             if ( export_skip == 0 )
             if (  ( lline[ 0 ] == '/' ) && ( lline[ 1 ] == '/' )  )
             {
                   printf( "Rem : do nothing.\n" );
             }
             else if ( ( lline[0] != '%' ) &&  ( strstr( lline, "note = {not" ) != 0 ) )
             {
                   printf( "Rem : do nothing.\n" );
             }
             else
             {
               fputs( lline , target);
               fputs( "\n" , target);
             }

             // clear
             lline[0]='\0';
             pcc = 0;

           }

           co = c; 
       }
   }
   fclose(source);
   fclose( target );
}









int main( int argc, char *argv[])
{
    char fichier[PATH_MAX];
    char targetfile[PATH_MAX];
    char string[PATH_MAX];

    if ( argc == 2)
    if ( strcmp( argv[1] , "time" ) ==  0 ) 
    {
       printf("%d\n", (int)time(NULL));
       return 0;
    }

    struct winsize w; // need ioctl and unistd 
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
    printf("Env HOME:  %s\n", getenv( "HOME" ));
    printf("Env PATH:  %s\n", getcwd( string, PATH_MAX ) );
    printf("Env TERM ROW:  %d\n", w.ws_row );
    printf("Env TERM COL:  %d\n", w.ws_col );

    ///////////////
    if ( argc == 1)
    {
       printf("Usage: please enter a file to use this software." );
       printf("\n" );
       return 0;
    }

    ///////////////
    if ( argc == 2)
      strncpy( fichier, argv[ 1 ] , PATH_MAX );

    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    if ( argc == 2)
    {
          strncpy( targetfile, fbasenoext( argv[ 1 ] ) , PATH_MAX );
          strncat( targetfile , ".bib" , PATH_MAX - strlen( targetfile ) -1 );
          printf( "> Target: %s\n" , targetfile );
          printf( "> Source: %s => Target: %s \n", argv[1] , targetfile  );
          printf( "\n" );
          printf( "\n" );
          readfileline( targetfile, argv[ 1 ] );
          printf( "\n" );
          printf( "\n" );
    }
    return 0;
}


