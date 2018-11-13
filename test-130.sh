#!/bin/sh

#
# Test basique de fonctionnement : accueillir et décharger un navire
#

. ./ftest.sh

# avant toute chose, partir d'une place nette
$V ./pclean 2> /dev/null >&2

# lancer la simulation en arrière plan
$V ./pcap 2  > $TMP.cap 2>&1 &
PIDCAP=$!

sleep $MARGE

# une flotte de camions demande le navire "aurore boréale" : pas encore là
# rappel : seul le premier caractère du nom est significatif
$V ./pfcam "aurore boréale" 2 0 >&2	&& fail "pfcam aurait dû échouer"

# le navire "aurore boréale" s'annonce
$V ./pnav "aurore boréale" 2 0 0  > $TMP.nava 2>&1 &
PIDNAV=$!

# lui laisser le temps d'accoster
sleep $MARGE
ps_existe $PIDNAV		"le navire devrait être là"

# la flotte de camions arrive : on ne teste que le premier caractère du nom
$V ./pfcam "aile mordorée" 2 0 >&2	|| fail "pfcam a échoué"

sleep $MARGE

# normalement, le navire a dû quitter le quai
ps_termine $PIDNAV		"le navire devrait être parti"

# c'est bon, arrêter la simulation
$V ./pstop >&2

sleep $MARGE
ps_termine $PIDCAP		"port non arrêté"

logs_aux
echo "ok"
exit 0
