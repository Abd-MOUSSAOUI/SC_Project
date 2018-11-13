#!/bin/sh

#
# Tests de création et suppression des objets IPC System V
#

. ./ftest.sh

###############################################################################
# Test de création et suppression des IPC
# - nettoyer les IPC System V
# - prendre une photo 1 du résultat de ipcs -m / ipcs -s
# - créer le port
# - prendre une photo 2 du résultat de ipcs et comparer photo 2 / photo 1
# - arrêter le port
# - prendre une photo 3 du résultat de ipcs et comparer photo 3 / photo 1


$V ./pclean >&2              || fail "pclean"

# conserver l'état initial des IPC
ipcs -m > $TMP.m1
ipcs -s > $TMP.s1
(echo "État initial des IPC" ; cat $TMP.m1 $TMP.s1) >&2

# créer les IPC
$V ./pcap 3  > $TMP.cap 2>&1 &

sleep $MARGE

# normalement, on ne devrait pas retrouver la même chose
ipcs -m > $TMP.m2
ipcs -s > $TMP.s2
(echo "État des IPC après création" ; cat $TMP.m2 $TMP.s2) >&2
cmp -s $TMP.m1 $TMP.m2       && fail "création : ipcs -m identique"
cmp -s $TMP.s1 $TMP.s2       && fail "création : ipcs -s identique"

# supprimer : on devrait retrouver les IPC du début"
$V ./pstop >&2               || fail "pstop"

sleep $MARGE

ipcs -m > $TMP.m2
ipcs -s > $TMP.s2
(echo "État des IPC à la fin" ; cat $TMP.m2 $TMP.s2) >&2
cmp -s $TMP.m1 $TMP.m2       || fail "suppression : ipcs -m pas identique"
cmp -s $TMP.s1 $TMP.s2       || fail "suppression : ipcs -s pas identique"

###############################################################################
# Test pendant qu'on est là
# - tenter de démarrer les programmes qui supposent le port déjà créé

$V ./pdump               >&2 && fail "pdump avec port non créé"
$V ./pstop               >&2 && fail "pstop avec port non créé"
$V ./pfcam x 2 0         >&2 && fail "pfcam avec port non créé"
$V ./pnav x 2 0 0        >&2 && fail "pnav avec port non créé"

logs_aux
echo "ok"
exit 0
