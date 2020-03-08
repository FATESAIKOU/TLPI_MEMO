#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>

#define SG_SIZE (NGROUPS_MAX + 1)

int getGroups(const char *user, size_t gidsetsize, gid_t *grouplist);
int isUserInList(const char *user, char **gr_mem);

int
main(int argc, char *argv[])
{
    gid_t grouplist[SG_SIZE];
    size_t gidsetsize = SG_SIZE;
    
    gidsetsize = getGroups("fatesaikou", gidsetsize, grouplist);
    
    int i, res;
    printf("GIDS: ");
    for (i = 0; i < gidsetsize; i ++) {
        printf("%ld ", (long) grouplist[i]);
    }
    printf("\n");

    res = setgroups(gidsetsize, grouplist);
    if (res == -1) {
        printf("setgroups error\n");
        exit(-1);
    }

    return 0;
}

int
getGroups(const char *user, size_t gidsetsize, gid_t *grouplist)
{
    
    struct passwd *pwd;
    
    // Get main group from /etc/passwd
    pwd = getpwnam(user);
    if (pwd == NULL || gidsetsize <= 0) {
        printf("getpwname error\n");
        exit(-1);
    }
    
    grouplist[0] = pwd->pw_gid;
   
    // Get other group from /etc/group
    int i = 1;
    struct group *grp;

    while ((grp = getgrent()) != NULL) {
        if ( isUserInList(user, grp->gr_mem) ) {
            if (i >= gidsetsize) {
                printf("Too Many Groups");
                exit(-1);
            }

            grouplist[i++] = grp->gr_gid;
        }
    }

    endgrent();

    return i;
}

int
isUserInList(const char *user, char **gr_mem)
{
    char **p = gr_mem;

    while (*p != NULL) {
        if (strcmp(user, *p) == 0) {
            return 1;
        }
        p++;
    }

    return 0;
}
