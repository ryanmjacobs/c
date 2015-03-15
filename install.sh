#!/usr/bin/env bash

C_RST='\e[0m'
C_BLUE='\e[0;34m'
C_RED='\e[0;31m'
C_BRED='\e[1;31m'
C_BGREEN='\e[1;32m'
C_BBLUE='\e[1;34m'
C_BMAGENTA='\e[1;35m'

if [ -z "$INSTALL_PREFIX" ] ; then
	INSTALL_PREFIX="/usr/local/bin"
fi

usage() {
echo -e -n "Usage: ${C_BLUE}[ENV]${C_RST} $0 "
echo -e -n "[ ${C_BGREEN}install${C_RST} | "
echo -e -n " ${C_BRED}uninstall${C_RST} | "
echo -e    " ${C_BMAGENTA}about${C_RST} ]"
echo -e -n "\nEnvs\n ${C_BLUE}INSTALL_PREFIX${C_RST}=${C_BBLUE}$INSTALL_PREFIX ${C_RST}\n\n"
}

about() {
usage
cat << EOF 
c is a very simple, but useful utlility for quickly
compiling and executing C code without any hubbubb.

even in the case of blah.c, 'make blah', will leave
annoying artifacts which must be removed afterwards.
EOF

echo -e -n ${C_BRED}

cat <<EOF

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
DO NOT USE THIS AS BUILD SYSTEM, FAILURE AWAITS YOU
IF USED AS WHICH!! 
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
EOF

echo -e -n ${C_RST}
}


case "${1}" in
	uninst)
		echo -e "${C_RED}Uninstalling c from $INSTALL_PREFIX${C_RST}"

		if [ -d "$INSTALL_PREFIX" ]; then
			rm -vf "$INSTALL_PREFIX/c"
		else
			echo -e "${C_BMAGENTA}"
			echo "'$INSTALL_PREFIX' dir not found."
			echo "Maybe set the INSTALL_PREFIX env?"
			echo -e "${C_RST}"
			exit 1
		fi
		
		exit
		;;

	help)
		usage
		exit 1
		;;

	about)
		about
		exit 1
		;;

	inst)
		install -v -d -m 0755 "$INSTALL_PREFIX"

		if [ $? -ne 0 ]; then
			exit 1
		fi

		install -v -m 0755 "./c" "$INSTALL_PREFIX"

		if [ $? -ne 0 ]; then
			exit 1
		fi


		exit 0
		;;

	*)
		usage
		exit 1
		;;
esac 
