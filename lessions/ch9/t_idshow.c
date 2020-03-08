#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fsuid.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>

#define SG_SIZE (NGROUPS_MAX + 1)

char *getUserNameFromId(uid_t uid);
char *getGroupNameFromId(gid_t gid);

int
main(int argc, char *argv[])
{
    uid_t ruid, euid, suid, fsuid;
    gid_t rgid, egid, sgid, fsgid;
    gid_t suppGroups[SG_SIZE];
    int numGroups, j;
    char *p;

    if (getresuid(&ruid, &euid, &suid) == -1) {
        printf("getresuid error");
        exit(-1);
    }

    if (getresgid(&rgid, &egid, &sgid) == -1) {
        printf("getresgid error");
        exit(-1);
    }

    /* unprivileged process -> just return the current fsid(no change)
     * privileged process -> equal to 0
     */
    fsuid = setfsuid(0);
    fsgid = setfsgid(0);

    printf("UID: ");
    p = getUserNameFromId(ruid);
    printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long) ruid);
    p = getUserNameFromId(euid);
    printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long) euid);
    p = getUserNameFromId(suid);
    printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long) suid);
    p = getUserNameFromId(fsuid);
    printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long) fsuid);
    printf("\n");

    printf("GID: ");
    p = getGroupNameFromId(rgid);
    printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long) rgid);
    p = getGroupNameFromId(egid);
    printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long) egid);
    p = getGroupNameFromId(sgid);
    printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long) sgid);
    p = getGroupNameFromId(fsgid);
    printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long) fsgid);
    printf("\n");

    numGroups = getgroups(SG_SIZE, suppGroups);
    if (numGroups == - 1) {
        printf("getgroups error");
        exit(-1);
    }
    
    printf("Supplementary Groups (%d): ", numGroups);
    for (j = 0; j < numGroups; j ++) {
        p = getGroupNameFromId(suppGroups[j]);
        printf("%s (%ld) ", (p == NULL) ? "???" : p, (long) suppGroups[j]);
    }
    printf("\n");

    return 0;
}

char *
getUserNameFromId(uid_t uid) // return value using static space!!
{
    struct passwd *pwd;
    
    pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name;
}

char *
getGroupNameFromId(gid_t gid) // return value using static space!!
{
    struct group *grp;
    
    grp = getgrgid(gid);
    return (grp == NULL) ? NULL : grp->gr_name;
}
