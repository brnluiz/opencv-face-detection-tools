# Absolute path to this script. /home/user/bin/foo.sh
SCRIPT=$(readlink -f $0)
# Absolute path this script is in. /home/user/bin
SCRIPTPATH=`dirname $SCRIPT`

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SCRIPTPATH/builds/release/linux/x86/Lib
export LD_LIBRARY_PATH
