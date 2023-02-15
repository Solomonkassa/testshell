#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

extern const char **environ;

int setenv(const char *name, const char *value, bool overwrite) {
    // Check if the name or value is NULL
    if (name == NULL || value == NULL) {
        return -1;
    }

    // Check if the name contains an equals sign
    if (strchr(name, '=') != NULL) {
        return -1;
    }

    // Get the length of the name and value
    size_t name_len = strlen(name);
    size_t value_len = strlen(value);

    // Allocate memory for the new environment variable
    char *env_var = (char *) malloc(name_len + value_len + 2);
    if (env_var == NULL) {
        return -1;
    }

    // Copy the name and value into the new environment variable
    strcpy(env_var, name);
    env_var[name_len] = '=';
    strcpy(env_var + name_len + 1, value);

    // Find the existing environment variable with the same name, if any
    const char **envp = environ;
    const char **found_env_var = NULL;
    size_t found_env_var_index = 0;
    while (*envp != NULL) {
        if (strncmp(*envp, name, name_len) == 0 && (*envp)[name_len] == '=') {
            found_env_var = envp;
            break;
        }
        envp++;
        found_env_var_index++;
    }

    // If the environment variable was found and overwrite is false, return success
    if (found_env_var != NULL && !overwrite) {
        free(env_var);
        return 0;
    }

    // If the environment variable was found, replace it with the new one
    if (found_env_var != NULL) {
        *found_env_var = env_var;
    } else {
        // Otherwise, add the new environment variable to the end of the environment
        const char **new_envp = (const char **) realloc(environ, (found_env_var_index + 2) * sizeof(char *));
        if (new_envp == NULL) {
            free(env_var);
            return -1;
        }
        environ = new_envp;
        environ[found_env_var_index] = env_var;
        environ[found_env_var_index + 1] = NULL;
    }

    return 0;
}

int main() {
    // Set a new environment variable
   
