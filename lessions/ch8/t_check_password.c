#define _DEFAULT_SOURCE /* Get getpass() declaration from <unistd.h> */
#define _XOPEN_SOURCE /* Get crypt() declaration from <unistd.h> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include <unistd.h>
#include <stdbool.h>

int
main(int argc, char *argv[])
{
    char *username, *password, *encrypted, *p;
    struct passwd *pwd;
    struct spwd *spwd;
    bool authOK;
    size_t len;
    long lnmax;

    lnmax = sysconf(_SC_LOGIN_NAME_MAX);
    if (lnmax == -1)
        lnmax = 256;
    
    username = (char *)malloc(lnmax);
    if (username == NULL) {
        printf("Malloc Error!\n");
        exit(-1);
    }

    printf("Username: ");
    fflush(stdout);
    if (fgets(username, lnmax, stdin) == NULL) {
       printf("Fgets username Error!\n");
       exit(-1);
    }

    // Remove trailling '\n'
    len = strlen(username);
    if (username[len - 1] == '\n')
        username[len - 1] = '\0';

    pwd = getpwnam(username);
    if (pwd == NULL) {
        printf("Couldn't get password record!\n");
        exit(-1);
    }

    spwd = getspnam(username);
    if (spwd == NULL && errno == EACCES) {
        printf("No permission to read shadow password file!\n");
        exit(-1);
    }

    // If there is the shadow file and having the permission to access
    // use the password in shadow file
    if (spwd != NULL)
        pwd->pw_passwd = spwd->sp_pwdp;


    password = getpass("Password: ");

    // Get the encrypted password and erase the plain password string
    encrypted = crypt(password, pwd->pw_passwd);
    for (p = password; *p != '\0';)
        *p++ = '\0';

    if (encrypted == NULL) {
        printf("Crypt Error!\n");
        exit(-1);
    }

    authOK = strcmp(encrypted, pwd->pw_passwd) == 0;
    if (!authOK) {
        printf("Incorrect password!\n");
        exit(-1);
    }

    printf("Successfully authenticated: UID=%ld\n", (long) pwd->pw_uid);

    exit(0);
}
