/*
This file is part of rmw<https://theimpossibleastronaut.github.io/rmw-website/>

Copyright (C) 2012-2021  Andy Alt (arch_stanton5995@proton.me)
Other authors: https://github.com/theimpossibleastronaut/rmw/blob/master/AUTHORS.md

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "messages_rmw.h"

void
print_msg_error(void)
{
  /* TRANSLATORS: this precedes a string which informs the user of a more
   * serious error, sometimes a fatal one */
  fputs(_("  :error: "), stderr);
}

void
print_msg_warn(void)
{
  /* TRANSLATORS: this precedes a string which warns the user of some minor
   * but not fatal problem */
  fputs(_(" :warning: "), stdout);
}

/**
 * Called if fopen() returns NULL. prints an error message and some
 * extra info about the cause.
 * @param[in] filename the name of the file
 * @param[in] function_name the name of the function where the error originated
 * @return void
 */
void
open_err(const char *filename, const char *function_name)
{
  print_msg_error();
  /* TRANSLATORS:  "opening" refers to a file  */
  fprintf(stderr, _("while opening %s\n\
function: <%s>\n\
%s\n"), filename, function_name, strerror(errno));

  return;
}

/**
 * Closes a file, checks for an error. If one is returned, print a message
 * with some extra info about the error.
 *
 * @param[in] file_ptr a file pointer that already exists
 * @param[in] filename
 * @param[in] function_name the name of the calling function
 * @return an error number, 0 if no error
 */
int
close_file(FILE *file_ptr, const char *filename, const char *function_name)
{
  if (file_ptr == NULL)
    return -1;

  if (fclose(file_ptr) != EOF)
    return 0;
  else
  {
    int dup_errno = errno;
    /* TRANSLATORS:  "closing" refers to a file  */
    print_msg_error();
    fprintf(stderr, _("while closing %s\n\
function: <%s>\n\
%s\n"), filename, function_name, strerror(dup_errno));
    return dup_errno;
  }
}

void
display_dot_trashinfo_error(const char *dti)
{
  print_msg_error();
  /* TRANSLATORS:  ".trashinfo" should remain untranslated
   *
   *               "format" refers to the layout of the file
   *                contents
   */
  printf(_("format of .trashinfo `file %s` is incorrect"), dti);
  putchar('\n');
  return;
}

void
msg_warn_restore(int result)
{
  if (result == 0)
    return;

  if (result != ENOENT)
  {
    print_msg_warn();
    if (result != ENOENT)
    {
      /* TRANSLATORS: ignore "restore()"
       * "returned" refers to an error code (number) that was returned by
       * an operation */
      printf(_("restore() returned %d\n"), result);
    }
  }

  return;
}


void
real_fatal_malloc(const char *func, const int line)
{
  fprintf(stderr, "%s -- %s:L%d\n", strerror(errno), func, line);
  exit(ENOMEM);
}


void
msg_err_close_dir(const char *dir, const char *func, const int line)
{
  print_msg_error();
  fprintf(stderr, "while closing %s -- %s:L%d\n", dir, func, line);
  perror("closedir()");
  exit(errno);
}

void
msg_err_open_dir(const char *dir, const char *func, const int line)
{
  print_msg_error();
  fprintf(stderr, _("while opening %s -- %s:L%d\n%s\n"), dir, func, line,
          strerror(errno));
  return;
}

void
msg_err_rename(const char *src_file, const char *dest_file, const char *func,
               const int line)
{
  print_msg_error();
  printf(_("while trying to move (rename)\n\
  %s -> %s -- %s:L%d\n"), src_file, dest_file, func, line);
  perror("rename()");
  exit(errno);
}

/*!
 * Used for error-checking calls to fprintf.
 * @param[in] func The function in which the error ocurred
 * @return exit failure
 */
void
msg_err_fatal_fprintf(const char *func)
{
  print_msg_error();
  fprintf(stderr, "fprintf returned an error in %s.\n", func);
  exit(EXIT_FAILURE);
}

/*!
 * Used by functions that prevent buffer overflows
 * @param[in] func the function from which it was originally called
 * @param[in] line the line number of the original calling function
 * @return void
 * @see bufchk
 * @see bufchk_len
 */
void
msg_err_buffer_overrun(const char *func, const int line)
{
  print_msg_error();
  fprintf(stderr, "func = %s:L%d\n", func, line);
  /* TRANSLATORS:  "buffer" in the following instances refers to the amount
   * of memory allocated for a string  */
  fputs("Buffer length exceeded.\n", stderr);
  fputs
    ("If you think this may be a bug, please report it to the rmw developers.\n",
     stderr);
}

/*!
 * Called if lstat() returns an error.
 * @param[in] func the name of the calling function
 * @param[in] line the line number from where the function was called
 * @return void
 */
void
msg_err_lstat(const char *file, const char *func, const int line)
{
  print_msg_error();
  perror("lstat()");
  fprintf(stderr, "%s in %s:L%d\n", file, func, line);
  exit(errno);
}

void
msg_err_remove(const char *file, const char *func)
{
  int dup_errno = errno;
  print_msg_error();
  fprintf(stderr, _("while removing %s:\n%s\n(func:%s)\n"), file,
          strerror(dup_errno), func);
}


void
msg_err_mkdir(const char *dir, const char *func)
{
  perror("rmw_mkdir()");
  print_msg_error();
  fprintf(stderr, _("while creating %s (%s)\n"), dir, func);
}

void
msg_success_mkdir(const char *dir)
{
  printf(_("Created directory %s\n"), dir);
  return;
}


void
msg_warn_file_not_found(const char *file)
{
  printf("'%s': %s\n", file, strerror(ENOENT));
}
