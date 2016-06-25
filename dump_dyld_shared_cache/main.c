#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int copy_file( char * from, char * to )
{
    printf( "Copy from\n%s\nto\n%s\n", from, to );
    
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];
     
    inputFd = open( from, O_RDONLY );
    fcntl( inputFd, F_GLOBAL_NOCACHE, 1 );
    
    if ( inputFd == -1 )
        printf( "opening file" );
     
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    
    outputFd = open( to, openFlags, filePerms );
    
    if ( outputFd == -1 )
        printf( "opening file" );
    
    while ( ( numRead = read(inputFd, buf, BUF_SIZE ) ) > 0 )
        if ( write( outputFd, buf, numRead ) != numRead )
            printf( "couldn't write whole buffer" );
    
    if (numRead == -1)
        printf( "read");
    
    if (close(inputFd) == -1)
        printf( "close input" );
    
    if (close(outputFd) == -1)
        printf( "close output" );
    
    return 1;
}

int main( )
{
    char * home;
    home = getenv( "HOME" );
    
    strcat( home, "/tmp/" );
    
    DIR           *d;
    struct dirent *dir;
    char path[] = "/System/Library/Caches/com.apple.dyld/";
    d = opendir( path );
    
    char file_path[256];
    char new_path[256];
    
    if (d)
    {
        while (( dir = readdir(d) ) != NULL )
        {
            if ( dir->d_type == DT_REG )
            {
                strcpy( file_path, path );
                strcat( file_path, dir->d_name );
                
                strcpy( new_path, home );
                strcat( new_path, dir->d_name );
                
                copy_file( file_path, new_path );
            }
        }
        closedir(d);
    }
}
