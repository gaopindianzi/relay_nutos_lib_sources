#ifndef _GETOPT_H_
#define _GETOPT_H_

/*
 * $Id: getopt.h 878 2005-05-22 14:41:01Z haraldkipp $
 */

/*
 * $Log$
 * Revision 1.1  2005/05/22 14:41:01  haraldkipp
 * Moved from tools/nutconf and tools/crurom.
 *
 * Revision 1.1  2005/04/22 15:12:04  haraldkipp
 * Added to support non-GUI Configurator.
 *
 */

#ifdef __cplusplus
    extern "C" {
#endif

extern int getopt(int argc, char **argv, char *opts);

extern int optind;
extern char *optarg;

#ifdef __cplusplus
    }
#endif

#endif
