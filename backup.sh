cd ~

DIRECTORY="backup$(date +%Y%m%d%s)"
ARCHIVE=${DIRECTORY}
sleep 1
BUFFER="backup$(date +%Y%m%d%s)"

touch ${BUFFER}
find . -type f -name "*.cpp" -print >> ${BUFFER}

while getopts e:d:a: option
do
  case "${option}"
  in
    e) EXTANTION=${OPTARG}
      if [ "$EXTANTION" != "cpp" ]; then
        find . -type f -name "*.$EXTANTION" -print >> ${BUFFER}
      fi;;
    d) if [ -d "$OPTARG" ]; then
        DIRECTORY=${OPTARG}
      fi;;
    a) if [ ! -f "$OPTARG.tar" ]; then
        ARCHIVE=${OPTARG}
      fi;;
  esac
done

mkdir ${DIRECTORY}

COUNTER=0
cat ${BUFFER} | while read str; 
do 
  if [ -n "$str" ]; then
    NAME="$COUNTER\_$(basename $(echo $str | tr \' '_' | tr ' ' '_'))"
    cp "$str" ${DIRECTORY}/${NAME} 
    let "COUNTER+=1"
  fi
done

tar -cf "$ARCHIVE.tar" ${DIRECTORY}
rm -rf ${DIRECTORY}
rm ${BUFFER}

echo done.
