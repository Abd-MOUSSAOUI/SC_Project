#!/bin/sh

#
# Test basique de fonctionnement : arrêt et redémarrage
#

. ./ftest.sh

# avant toute chose, partir d'une place nette
$V ./pclean 2> /dev/null >&2

# lancer la simulation en arrière plan
$V ./pcap 2  > $TMP.cap 2>&1 &
PIDCAP=$!

# pcap est toujours là, pas encore planté ?
sleep $MARGE
ps_existe $PIDCAP               "pcap planté"

# c'est bon, arrêter la simulation
$V ./pstop  >&2

sleep $MARGE
ps_termine $PIDCAP		"pcap non arrêté"

logs_aux
echo "ok"
exit 0
