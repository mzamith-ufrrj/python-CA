#!/usr/bin/bash
DIR=$(date +%Y%m%d-%H%M%S)
DIR="test.$DIR"
if [ ! -d "$DIR" ]; then
  mkdir $DIR
fi
x=1
VAR=""
CONFIG="MLA-4-Default.json"
INCLUSTER="cluster.adjusted.MLA-4-STANDARD-HR-1.5-0.*"
INFIXED="statistic.fixed.MLA-4-STANDARD-HR-1.5-0.*"
INPHOTO="statistic.photo.MLA-4-STANDARD-HR-1.5-0.*"
OUTCLUSTER="CA.MLA-4-STANDARD-HR-1.5-0.csv"
OUTFIXED="SF.MLA-4-STANDARD-HR-1.5-0.csv"
OUTPHOTO="SP.MLA-4-STANDARD-HR-1.5-0.csv"
while [ $x -le 95 ]
do
  VAR+="${x} "
  x=$(( $x + 1 ))
done
parallel ./TModelCA++.exec ::: $CONFIG ::: $VAR
cat $INCLUSTER > $OUTCLUSTER
cat $INFIXED > $OUTFIXED
cat $INPHOTO > $OUTPHOTO
mv  $INCLUSTER $DIR
mv  $INFIXED  $DIR
mv  $INPHOTO $DIR
echo 'End the game!'