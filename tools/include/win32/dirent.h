#ifndef _DIRENT_H
#define _DIRENT_H

/*
 * $Log$
 * Revision 1.1  2005/05/22 14:41:01  haraldkipp
 * Moved from tools/nutconf and tools/crurom.
 *
 * Revision 1.1  2005/04/22 15:11:03  haraldkipp
 * Added to support non-GUI Configurator.
 *
 */

#include <sys/types.h>

struct dirent {
    ino_t   d_ino;      /* inode number of entry */
    off_t   d_off;      /* offset of disk directory entry */
    wchar_t d_reclen;   /* length of this record */
    char    d_name[256];
};

typedef struct _dircontents {
    char *_d_entry;
    struct _dircontents	*_d_next;
} DIRCONT;

typedef struct _dirdesc {
    int    dd_id;           /* uniquely identify each open directory */
    long   dd_loc;          /* where we are in directory entry is this */
    struct _dircontents *dd_contents;   /* pointer to contents of dir */
    struct _dircontents *dd_cp;         /* pointer to current position */
} DIR;

extern DIR *opendir(const char *);
extern struct dirent *readdir(DIR *);
extern int closedir(DIR *);

#endif
