#!/bin/bash

git add -add
echo "Enter the commit message: "
read -r -e quick_msg

git commit -m "$quick_msg"
echo -e "\033[1;33mcommit done\033[0m"
git pull -r
echo -e "\033[1;33mpull done\033[0m"
git push 
