//
// Copyright 2013 Tarim
//
// poll is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// poll is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with poll.  If not, see <http://www.gnu.org/licenses/>.
//

//
// poll waits for interrupts on /sys/class/gpio/gpioX/value style files
// and echoes the contents of the file.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

int main( int argc, char** argv ) {
    int fd;
    struct pollfd pfd;

    const int BUFFER_SIZE = 1024;
    int count;
    char buffer[BUFFER_SIZE];

    if( argc < 2 ) {
	printf("Usage: %s file\n", argv[0]);
	exit( 2 );
    }

    fd = open( argv[1], O_RDONLY );
    if( fd < 0 ) {
	perror( argv[1] );
	exit( 1 );
    }

    pfd.fd = fd;
    pfd.events = POLLPRI;

    while( 1 ) {
	while( count = read( fd, buffer, sizeof(buffer) - 1 ) ) {
	    if( count < 0 ) {
		perror( "read" );
		exit( 1 );
	    }

	    write( STDOUT_FILENO, buffer, count );
	}
 
	lseek( fd, 0, SEEK_SET );

    	if( poll( &pfd, 1, -1 ) < 0 ) {
	    perror( "poll" );
	    exit( 1 );
	}
    }
};
