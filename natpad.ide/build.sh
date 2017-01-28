#!/bin/bash

ANT_VERSION=1.8.1

ANT_HOME="./build/ant/apache-ant-${ANT_VERSION}/"
ANT_RUN="./build/ant/apache-ant-${ANT_VERSION}/bin/ant"


NATPAD_WORKSPACE=`pwd`
ANT_SCRIPTS=`pwd`/natpad-builder/src/ant-scripts

if [ ! -n "$NATPAD_HOME" ]
then
	NATPAD_HOME=$NATPAD_WORKSPACE
fi


setup_ant() {
   if [ ! -f ./build/ant/ant-natpad.jar ] || [ $1=="true" ] 
   then
      mkdir -p ./build/ant/
      tar -C ./build/ant/ -xzf ./natpad-builder/src/ant-src/apache-ant-${ANT_VERSION}-bin.tar.gz 
      ${ANT_RUN} -f ./natpad-builder/src/ant-scripts/create-ant-lib.xml
   fi
}

set_env_detect() {
	${ANT_RUN} -f ./natpad-builder/src/ant-scripts/prepare-build.xml
	ANT_OS_SCRIPTS=${ANT_SCRIPTS}/${ostype}
	ANT_OPTS="-DNATPAD_HOME=${NATPAD_HOME} -DGVLIB_HOME=${GVLIB_HOME}"
}



case "$1" in
setup)
	setup_ant "true"
	set_env_detect
    ;;

clean)
	if [ ! -n "$2" ]
        then
                ${ANT_RUN} ${ANT_OPTS} -f ./natpad-builder/src/ant-scripts/build.xml $1
        else
                ${ANT_RUN} ${ANT_OPTS} -f ./natpad-builder/src/ant-scripts/build.xml $1.$2
        fi
    ;;


windows|chroot32|linux)
	setup_ant "false"
	if [ ! -n "$2" ]
        then
                ${ANT_RUN} ${ANT_OPTS} -f ./natpad-builder/src/ant-scripts/${1}/build.xml
        else
                ${ANT_RUN} ${ANT_OPTS} -f ./natpad-builder/src/ant-scripts/${1}/build.xml $2
        fi
    ;;

package)
	setup_ant "false"
	if [ ! -n "$2" ]
	then
		${ANT_RUN} ${ANT_OPTS} -f ./natpad-builder/src/ant-scripts/build.xml $1
	else
		${ANT_RUN} ${ANT_OPTS} -f ./natpad-builder/src/ant-scripts/build.xml $1.$2 
	fi
    ;;

install)
	rm -Rf ./natpad-builder/build/ant
	setup_env "false" 
    ${ANT_RUN} ${ANT_OPTS} -f ${ANT_OS_SCRIPTS}/build.xml build
    cp -f ../elk/build/elk /usr/bin/natpad 
    ;;


configure | dist)
	setup_ant "false"
	set_env_detect
	if [ ! -n "$2" ]
	then
		sudo ${ANT_RUN} ${ANT_OPTS} -f ./natpad-builder/src/ant-scripts/build.xml $1
	else
                sudo ${ANT_RUN} ${ANT_OPTS} -f ./natpad-builder/src/ant-scripts/build.xml $1.$2
        fi
    ;;

build | fullbuild)
	setup_ant "false"
	set_env_detect
	if [ ! -n "$2" ]
	then
		${ANT_RUN} ${ANT_OPTS} -f ./natpad-builder/src/ant-scripts/build.xml $1
	else
        ${ANT_RUN} ${ANT_OPTS} -f ./natpad-builder/src/ant-scripts/build.xml $1.$2
    fi
    ;;
test)
	setup_ant "true"
	java -cp ./build/ant/ant-natpad.jar net.natpad.builder.Builder $1
    ;;

*)
    echo "Usage: build.sh <action> [sub-action]"
    echo "  action is one of"
    echo "    setup                 does an initial setup"
    echo "    clean                 clean all build files of the modules"
    echo "    build <platform>"
    echo "    package <pcktype>     create a source package or an installer"
    echo "    configure <platform>"
    echo "  platform is one of"
    echo "    linux"
    echo "    windows"
    echo "  pcktype is one of"
    echo "    debian"
    echo "    rpm"
    echo "    targz"
    exit 1
    ;;
esac

