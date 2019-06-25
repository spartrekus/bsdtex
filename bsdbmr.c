

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

#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

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

int fexist(char *a_option)
{
  char dir1[PATH_MAX]; 
  char *dir2;
  DIR *dip;
  strncpy( dir1 , "",  PATH_MAX  );
  strncpy( dir1 , a_option,  PATH_MAX  );

  struct stat st_buf; 
  int status; 
  int fileordir = 0 ; 

  status = stat ( dir1 , &st_buf);
  if (status != 0) {
    fileordir = 0;
  }

  // this is compatible to check if a file exists
  FILE *fp2check = fopen( dir1  ,"r");
  if( fp2check ) {
  // exists
  fileordir = 1; 
  fclose(fp2check);
  } 

  if (S_ISDIR (st_buf.st_mode)) {
    fileordir = 2; 
  }
return fileordir;
/////////////////////////////
}





////////////////////////////////////////////////////////////////////
char *strcut( char *str , int myposstart, int myposend )
{     // copyleft, C function made by Spartrekus 
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( ( str[i] != '\0' ) && ( str[i] != '\0') )
         if ( ( i >=  myposstart-1 ) && (  i <= myposend-1 ) )
           ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
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
   fputs( "\n" , target );
   fputs( "\n" , target );
   fputs( "%%Converted using the C programming language.\n" , target );
   fputs( "\n" , target );
   fputs( "\n" , target );

   FILE *fp5;

   char ncpfile[PATH_MAX];
   char fooline[PATH_MAX];
   int export_skip = 0;
   int area_archive = 0;
   int fileeof = 0;
   while( fileeof == 0 )
   {
       export_skip = 0;
       c = fgetc(source); 
       if ( c == EOF ) fileeof = 1;
       // EOF

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
             {
             if ( export_skip == 0 )
             {
             if (  ( lline[ 0 ] == '/' ) && ( lline[ 1 ] == '/' )  )
             {
                   printf( ">Rem : do nothing.\n" );
             }


             else if ( ( lline[0] == '!' ) && ( lline[1] == 'r' )   && ( lline[2] == 'e' ) 
                    && ( lline[3] == 'm' ) && ( lline[4] == ' ' ) )
             {   // !rem skip 
                 printf( "=> !rem, just skip.\n" );
             }


             else if   ( ( lline[0] == '!' ) && ( lline[1] == 's' )  
                      && ( lline[0] == '!' ) && ( strstr(lline, "!section " ) != 0 ) )
             {
                 strncpy( fooline , strcut( lline, 8+2 , strlen( lline ) )  ,  PATH_MAX );
                 fputs( "\\section{" , target);
                 fputs( fooline , target);
                 fputs( "}" , target);
                 fputs( "\n" , target);
                 printf( "=> section\n" );
             }


             else if   ( ( lline[0] == '#' ) && ( lline[1] == ' ' ) ) 
             {
                 strncpy( fooline , strcut( lline, 1+2 , strlen( lline ) )  ,  PATH_MAX );
                 fputs( "\\section{" , target);
                 fputs( fooline , target);
                 fputs( "}" , target);
                 fputs( "\n" , target);
                 printf( "=> section\n" );
             }

             else if ( ( lline[0] == '#' ) && ( lline[1] == '#' ) && ( lline[2] == ' ' ) )
             {
                 strncpy( fooline , strcut( lline, 2+2 , strlen( lline ) )  ,  PATH_MAX );
                 fputs( "\\subsection{" , target);
                 fputs( fooline , target);
                 fputs( "}" , target);
                 fputs( "\n" , target);
                 printf( "=> subsection\n");
             }

             else if ( ( lline[0] == '!' ) && ( strstr(lline, "!clr" ) != 0 ) )
             {
                 printf( "=> clearpage or new frame.\n" );
                 printf( "=> \\begin{itemize}.\n" );
             }

             else if ( ( lline[0] == '!' ) && ( strstr(lline, "!enddoc" ) != 0 ) )
             {
                 printf( "=> enddoc\n" );
                 fputs( "\\end{document}" , target);
                 fputs( "\n" , target);
                 fputs( "\n" , target);
             }

             else if ( ( lline[0] == '!' ) && ( strstr(lline, "!bigskip" ) != 0 ) )
             {
                 fputs( "\\bigskip" , target);
                 fputs( "\n" , target);
                 printf( "=> bigskip\n" );
             }

             else if ( ( lline[0] == '#' ) && ( strstr(lline, "#include{" ) != 0 ) )
             {
                 strncpy( ncpfile , strcut( lline , 8+2 , strlen( lline )-1 )  ,  PATH_MAX );
                 printf( "=> Found an include with string  |%s|%d|\n" , ncpfile , fexist( ncpfile ) );
                 strncpy( ncpfile , getenv( "HOME" ), PATH_MAX );
                 strncat( ncpfile , "/include/mrk/" , PATH_MAX - strlen( ncpfile ) -1 );
                 strncat( ncpfile , strcut( lline , 8+2 , strlen( lline )-1 )  , PATH_MAX - strlen( ncpfile ) -1 );
                 printf( "=> Found an include #include: |%s|%d|\n" , ncpfile , fexist( ncpfile ) );
                 int ch ; 
                 if ( fexist( ncpfile ) == 1 ) 
                 {
                   fp5 = fopen( ncpfile  , "r");
                   while( ( ch = fgetc(fp5) ) != EOF )
                   {
                      printf( "%c", ch );
                      fputc( ch, target);
                   }
                   fclose(fp5);
                 }
             }


             else if ( ( lline[0] == '/' ) && ( lline[1] == '/' ) ) 
             {
                   printf( ">Rem : do nothing.\n" );
             }
             else
             {
               fputs( lline , target);
               fputs( "\n" , target);
             }
             }
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

    ////////////////////////////////////////////////////////
    if ( argc == 3)
    if ( strcmp( argv[1] , "-f" ) ==  0 ) 
    {
       printf("%syellow\n", KYEL);
       readfile( argv[ 2 ] );
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
          strncat( targetfile , ".tex" , PATH_MAX - strlen( targetfile ) -1 );
          printf( "> Target: %s\n" , targetfile );
          printf( "> Source: %s => Target: %s \n", argv[1] , targetfile  );
          printf( "\n" );
          printf( "\n" );
          readfileline( targetfile, argv[ 1 ] );
          printf( "\n" );
          printf( "> Target: %s\n" , targetfile );
          printf( "> Source: %s => Target: %s \n", argv[1] , targetfile  );
          printf( "> Target: %s\n" , targetfile );
          printf( "> COMPLETED.\n" );
          printf( "\n" );
    }
    return 0;
}

/*
> this is me 
- item 1
- item 2
- item 3
!fig{fig.png}

gives

\begin{itemize}
\item this is me
\begin{itemize}
\item item 1
\item item 2
\item item 3
\begin{center}
\includegraphics[width=1.0\textwidth]{fig.png}
\end{center}
\end{itemize}
\end{itemize}
*/



