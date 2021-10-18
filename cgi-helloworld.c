#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAXLEN 80
#define MAXLENFILE 200
#define EXTRA 5
/* 4 for field name "data", 1 for "=" */
#define MAXINPUT MAXLEN + EXTRA + 2
/* 1 for added line break, 1 for trailing NUL */

void removeNewline(char data[])
{
  // om nieuw lines te verwijderen
  char str[40];
  unsigned int len = sprintf(str, data);
  str[len - 1] = 0;
  sprintf(data, str);
}

static void
print_http_header(const char *content_type)
{
  printf("Content-Type: %s\n\n", content_type);
}

void unencode(char *src, char *last, char *dest)
{
  for (; src != last; src++, dest++)
    if (*src == '+')
      *dest = ' ';
    else if (*src == '%')
    {
      int code;
      if (sscanf(src + 1, "%2x", &code) != 1)
        code = '?';
      *dest = code;
      src += 2;
    }
    else
      *dest = *src;
  *dest = '\n';
  *++dest = '\0';
}

int main(void)
{
  char *url = "/var/www/html/info.json";
  time_t current_time;
  FILE *f;
  struct tm *local_time;
  char *content_post;
  char *cookies;

  char input[MAXINPUT], data[MAXINPUT];
  long len;
  print_http_header("text/html");
 content_post = getenv("CONTENT_LENGTH");
  cookies = getenv("HTTP_COOKIE");
 
  if (cookies != NULL)
  {
    char buff[30];
    sprintf(url, "/var/www/html/%s.json", &cookies[9]);
    f = fopen(url, "r+");

    if (f == NULL)
    {
      f = fopen(url, "w");
      fputs("{\"posts\": [{}]", f);
    }

    fclose(f);
    printf("%s", buff);
  };
  if (content_post == NULL || sscanf(content_post, "%ld", &len) != 1 || len > MAXLEN)
    printf("<P>Error in invocation - wrong FORM probably.");
  else
  {

    fgets(input, len + 1, stdin);
    unencode(input + EXTRA, input + len, data);
    f = fopen(url, "r+");
    if (f == NULL)
      printf("<P>Sorry, cannot store your data.");
    else
    {
      fseek(f, -2, SEEK_END);
      current_time = time(NULL);
      local_time = localtime(&current_time);
      char datum[40];
      sprintf(datum, "%s", asctime(local_time));
      removeNewline(data);
      removeNewline(datum);

      char str[30];
      sprintf(str, ",{\"tekst\": \"%s\" , \"datum\": \"%s\"}]}", data, datum);
      fputs(str, f);
      fclose(f);
      printf("<script>window.location.href = \"http://rpi-manud.local/\";</script>");
    }
  }
  return 0;
}