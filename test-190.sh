#!/bin/sh

#
# test pour vérifier la bonne terminaison du système
#

. ./ftest.sh

# nombre de quais
NQUAIS=3
# on va travailler sur ces navires
NAVIRES="a b c d e f"
# qui transportent ce nombre de conteneurs
NCONTENEURS=4			# doit être pair

# avant toute chose, partir d'une place nette
./pclean 2> /dev/null >&2

# lancer la simulation en arrière plan,
./pcap $NQUAIS  > $TMP.cap 2>&1 &
PIDCAP=$!
sleep $MARGE

# On démare tous les navires : 3 peuvent accoster, 3 restent en attente
PIDNAV=""
for m in $NAVIRES
do
    # lancer les navires (hum... toujours façon de parler)
    ./pnav $m $NCONTENEURS 0 0 > $TMP.nav$m 2>&1 &
    PIDNAV="$PIDNAV $!"
done
sleep $MARGE

./pdump > $TMP.dmp1 >&1		|| fail "dump 1"

#
# Première étape : décharger les 3 navires à quai, mais seulement à moitié
#

# on décharge les navires à moitié (sans savoir quels navires décharger)
# Certains n'arriveront pas à décharger, le navire n'étant pas à quai
PIDCAM=""
for m in $NAVIRES
do
    ./pfcam $m $((NCONTENEURS/2)) 0 > $TMP.cam1$m 2>&1 &
    PIDCAM="$PIDCAM $!"
done
sleep $MARGE

# tous les camions devraient maintenant avoir terminé
for pid in $PIDCAM
do
    ps_termine $pid		"camion $pid non terminé"
done
# mais pas les navires
for pid in $PIDNAV
do
    ps_existe $pid		"navire $pid terminé avant pstop"
done

#
# Deuxième étape : donner l'ordre de fermture
# cela provoque la terminaison des navires en attente
#

./pstop > $TMP.stp 2>&1		|| fail "pstop"
sleep $MARGE

./pdump > $TMP.dmp2 >&1		|| fail "dump 1"

# les navires à quai devraient toujours là : comme on ne connaît pas
# leur identité, on va se fier aux pid
AQUAI=""
for pid in $PIDNAV
do
    if ps_test $pid
    then AQUAI="$AQUAI $pid"
    fi
done

# on devrait avoir card(NAV_A_QUAI) == NQUAIS
test $(echo $AQUAI | wc -w) = $NQUAIS	|| fail "mauvais nb de ps terminés"

#
# Troisième étape : finir de décharger les navires
#

PIDCAM=""
for m in $NAVIRES
do
    ./pfcam $m $((NCONTENEURS/2)) 0 > $TMP.cam2$m 2>&1 &
    PIDCAM="$PIDCAM $!"
done
sleep $MARGE

# tous les camions devraient maintenant avoir terminé
for pid in $PIDCAM
do
    ps_termine $pid		"camion $pid non terminé à la fin"
done
# mais pas les navires
for pid in $AQUAI
do
    ps_termine $pid		"navire $pid non terminé à la fin"
done

ps_termine $PIDCAP		"pcap non terminé"


logs_aux
echo ok
exit 0
