# Minishell

The objective of this project is for you to create a simple shell. Yes, your own little bash or zsh. You will learn a lot about processes and file descriptors.

## Codam project FAQ

All Codam projects are written within the 42/codam styleguide called The Norm. The intention is that it should force you to write clean code and make everybody write in the same style so evaluations and code reviews are easier.
This gives guidelines on maximum lines per function (25), maximum characters per line (80), maximum functions per file (5), maximum number of parameters per function (4) etc.
Although these guidelines are highly debated and do not always serve their intented function of clean code, they are non negotionable in the projects, 1 small mistake and you have to redo the project.

Thus the "cleanness" of the code in these projects are within the scope of The Norm. More information about the norm can be found in [this repository](https://github.com/42School/norminette/)

## Brief description of what we did

We decided to not just "split on spaces and go from there" (which was a common approach) but to look into [formal grammar](Grammar). We build a finite state machine to act as our tokenizer (lexing) and a relatively simple recursive descent parser to transform our tokens into an AST. From there the execution of the tree branches was trivial and while this whole setup cost us more time to build, we did not run into most of the problems many people run into that don't do proper tokenizing.

---

## Allowed external functions:

readline, rl_on_new_line, rl_replace_line,
rl_redisplay, add_history, printf, malloc, free,
write, open, read, close, fork, wait, waitpid,
wait3, wait4, signal, kill, exit, getcwd, chdir,
stat, lstat, fstat, unlink, execve, dup, dup2,
pipe, opendir, readdir, closedir, strerror, errno,
isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr,
tcgetattr, tgetent, tgetflag, tgetnum, tgetstr,
tgoto, tputs

## Your shell should:

-   Not interpret unclosed quotes or unspecified special characters like `\` or;.
-   Not use more than one global variable, think about it and be ready to explain why you do it.
-   Show a prompt when waiting for a new command.
-   Have a working History.
-   Search and launch the right executable (based on the PATH variable or by using relative or absolute path)
-   It must implement the builtins:
    -   echo with option -n
    -   cd with only a relative or absolute path
    -   pwd with no options
    -   export with no options
    -   unset with no options
    -   env with no options or arguments
    -   exit with no options
-   `’` inhibit all interpretation of a sequence of characters.
-   `"` inhibit all interpretation of a sequence of characters except for `$`.
-   Redirections:
    -   `<` should redirect input.
    -   `>` should redirect output.
    -   `"<<"` read input from the current source until a line containing only the delimiter is seen. It doesn’t need to update history!
    -   `">>"` should redirect output with append mode.
-   Pipes `|` The output of each command in the pipeline is connected via a pipe to the input of the next command.
-   Environment variables (`$` followed by characters) should expand to their values.
-   `$?` should expands to the exit status of the most recently executed foreground pipeline.
-   ctrl-C ctrl-D ctrl-\ should work like in bash.
-   When interactive:
    -   ctrl-C print a new prompt on a newline.
    -   ctrl-D exit the shell.
    -   ctrl-\ do nothing.

Anything not asked is not required.
For every point, if you have any doubt take [bash](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/) as a reference.

# Bonus for when we are really bored

> take into consideration that bonus is meant as _just for fun_!!

If the Mandatory part is not perfect don’t even think about bonuses

-   `&&`, `||` with parenthesis for priorities.
-   the wilcard `*` should work for the current working directory.
