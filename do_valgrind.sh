#!/bin/zsh

SUPP_PATH=/Users/$(whoami)/Codam/utilities/valgrind/

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --suppressions=${SUPP_PATH}defaults.supp \
         --suppressions=${SUPP_PATH}minishell.supp \
         --log-file=valgrind-out.txt \
         ./minishell
         