char* _getenv(const char* name)
{
    extern char **environ;
    size_t len = strlen(name);

    for (char** env = environ; *env; env++)
  
    {
if (_strncmp(name, *env,len) == 0 && (*env)[len] == '=') 
        {
            return &(*env)[len + 1];
        }
    }

    return NULL;
}
