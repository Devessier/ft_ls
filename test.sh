#!/bin/sh

echo -n "" > my > yours

ls "$1" > yours && ./ft_ls "$1" > my

if [ -z $(diff my yours) ]; then
	echo "fine !"
else
	echo "error :-("
fi
