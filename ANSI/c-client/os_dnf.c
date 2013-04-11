/*
 * Program:	Operating-system dependent routines -- MS-DOS (PC-NFS) version
 *
 * Author:	Mark Crispin
 *		Networks and Distributed Computing
 *		Computing & Communications
 *		University of Washington
 *		Administration Building, AG-44
 *		Seattle, WA  98195
 *		Internet: MRC@CAC.Washington.EDU
 *
 * Date:	11 April 1989
 * Last Edited:	11 October 1994
 *
 * Copyright 1994 by the University of Washington
 *
 *  Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appears in all copies and that both the
 * above copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the University of Washington not be
 * used in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  This software is made
 * available "as is", and
 * THE UNIVERSITY OF WASHINGTON DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED,
 * WITH REGARD TO THIS SOFTWARE, INCLUDING WITHOUT LIMITATION ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, AND IN
 * NO EVENT SHALL THE UNIVERSITY OF WASHINGTON BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE) OR STRICT LIABILITY, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

/* Private function prototypes */

#include "tcp_dos.h"		/* must be before osdep includes tcp.h */
#include "mail.h"
#include "osdep.h"
#include <time.h>
#include <sys\timeb.h>
#include <sys\nfs_time.h>
#include <sys\tk_types.h>
#include <sys\socket.h>
#include <netinet\in.h>
#include <tk_errno.h>
#include <sys\uio.h>
#include <netdb.h>
#include "misc.h"


#include "fs_dos.c"
#include "ftl_dos.c"
#include "nl_dos.c"
#include "env_dos.c"
#include "tcp_dos.c"


/* Return my local host name
 * Returns: my local host name
 */

char *mylocalhost (void)
{
  if (!myLocalHost) {		/* known yet? */
    char *s,tmp[MAILTMPLEN];
    unsigned long myip;
				/* see if known host name */
    if (!gethostname (tmp,MAILTMPLEN-1)) s = tmp;
				/* no, try host address */
    else if (get_myipaddr ((char *) &myip))
      sprintf (s = tmp,"[%s]",inet_ntoa (myip));
    else s = "random-pc";	/* say what? */
    myLocalHost = cpystr (s);	/* record for subsequent use */
  }
  return myLocalHost;
}


/* Look up host address
 * Accepts: pointer to pointer to host name
 *	    socket address block
 * Returns: non-zero with host address in socket, official host name in host;
 *	    else NIL
 */

long lookuphost (char **host,struct sockaddr_in *sin)
{
  long ret = -1;
  char tmp[MAILTMPLEN];
  struct hostent *hn = gethostbyname (lcase (strcpy (tmp,*host)));
  if (!hn) return NIL;		/* got a host name? */
  *host = cpystr (hn->h_name);	/* set official name */
				/* copy host addresses */
  memcpy (&sin->sin_addr,hn->h_addr,hn->h_length);
  return T;
}
