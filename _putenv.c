int _putenv(char *s)
{
  int i = 0;

  for (; environ[i] != NULL;)
    {
      i++;
    }

  environ[i] = s;
  environ[i + 1] = 0;

  return (0);
}
