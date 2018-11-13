#!/bin/sh

#
# Tests basiques pour vérifier les conditions d'attente
#

. ./ftest.sh

# avant toute chose, partir d'une place nette
$V ./pclean 2> /dev/null >&2

# lancer la simulation en arrière plan, 1 seul quai disponible
$V ./pcap 1  > $TMP.cap 2>&1 &
PIDCAP=$!

sleep $MARGE

# lancer le premier navire : il doit accoster sans problème
$V ./pnav a 2 0 0 > $TMP.nava 2>&1 &
PIDNAVA=$!
sleep $MARGE
ps_existe $PIDNAVA		"pnav a devrait être là"

# lancer le second navire : il doit attendre
$V ./pnav b 5 0 0 > $TMP.navb 2>&1 &
PIDNAVB=$!
sleep $MARGE
ps_existe $PIDNAVB		"pnav b devrait être là"

# d'ailleurs, vérifier que les camions ne trouvent pas b à quai
$V ./pfcam b 2 0 >&2		&& fail "pfcam ne devrait pas trouver b"

# maintenant, on décharge le premier navire
$V ./pfcam a 2 0 > $TMP.cama 2>&1 &
PIDCAMA=$!

# charger sur 2 camions, même avec tc = 0, ça peut prendre un minimum de temps
sleep $(echo "3*$MARGE" | bc)
ps_termine $PIDCAMA		"pfcam a devrait avoir fini"

# le premier navire devrait être reparti
ps_termine $PIDNAVA		"pnav a devrait avoir fini"

# comme le second navire devrait avoir accosté, on peut demander à
# fermer le port. Il ne sera fermé que lorsque le navire b aura été
# complètement déchargé
$V ./pstop >&2			|| fail "pstop"

# vérifier en demandant l'accostage d'un troisième navire : il doit
# repartir illico
$V ./pnav c 2 0 0 > $TMP.navc 2>&1 &
PIDNAVC=$!
sleep $MARGE
ps_termine $PIDNAVC		"pnav c devrait être reparti"

# maintenant, on décharge les 3 premiers conteneurs du deuxième navire
$V ./pfcam b 3 0 > $TMP.camb1 2>&1 &
PIDCAMB=$!
sleep $(echo "3*$MARGE" | bc)
ps_termine $PIDCAMB		"pfcam b 1/2 devrait avoir fini"

# ensuite, on décharge les 2 derniers conteneurs du deuxième navire
$V ./pfcam b 2 0 > $TMP.camb2 2>&1 &
PIDCAMB=$!
sleep $(echo "3*$MARGE" | bc)
ps_termine $PIDCAMB		"pfcam b 2/2 devrait avoir fini"

# à la fin, il n'y a normalement plus aucun processus
ps_termine $PIDNAVB		"pnav b devrait avoir fini"
ps_termine $PIDCAP		"pcap devrait avoir fini"

logs_aux
echo "ok"
exit 0
