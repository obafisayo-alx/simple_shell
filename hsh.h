#ifndef _HSH_H_
#define _HSH_H_

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>

#include "typedef.h"
#include "dict.h"
#include "envt.h"
#include "dets.h"

extern char **environ;

void _sigint(int signal);

#endif /* _HSH_H_*/