#!/bin/bash

PDXINFO="$1"
NEW_VALUE="---"

VFILE=$(mktemp "${TMPDIR:-/tmp/}XXXXXXXXXX")

while read LINE; do
  KEY=`echo $LINE | awk -F '=' '{print $1}'`
  VALUE=`echo $LINE | awk -F '=' '{print $2}'`

  if [ "$KEY" = 'buildNumber' ]; then
    NEW_VALUE=`expr "$VALUE" + 1`
    printf '%03d' "$NEW_VALUE" > $VFILE
    printf 'Incrementing build number %s to %03d\n' "$VALUE" "$NEW_VALUE" > /dev/stderr
    printf '%s=%03d\n' "$KEY" "$NEW_VALUE"
  else
    printf '%s=%s\n' "$KEY" "$VALUE"
  fi
done < "$1" | sponge "$1"

NEW_VALUE=$(cat $VFILE)
rm $VFILE

git add "$1"
git commit -m "Increment build number to $NEW_VALUE"
