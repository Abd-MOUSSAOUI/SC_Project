#!/bin/sh

#
# Test pour détecter d'éventuels problèmes de concurrence
# entre les camions cette fois-ci
#

. ./ftest.sh

if [ ! -z "$V" ]
then
    echo "ok (test incompatible avec valgrind)"
    exit 0
fi

# nombre de simulations
NSIMU=3
# on va travailler sur ces navires : un par quai
NAVIRES="a b c d e f g h"
# qui transportent ce nombre de conteneurs
NCONTENEURS=3
# les temps
TA=0				# accostage super rapide
TD=50				# déchargement du navire
TC=100				# chargement sur le camion

# nombre de quais = nombre de navires
NQUAIS=$(echo $NAVIRES | wc -w)

# avant toute chose, partir d'une place nette
./pclean 2> /dev/null >&2

# ces tests peuvent parfois échouer... si on n'a pas de chance... mais
# seulement si on a mal synchronisé les processus entre eux. Du coup,
# on les fait tourner plusieurs fois pour avoir un filtre le plus fin
# possible

i=1
while [ $i -le $NSIMU ]
do
    echo "simulation numéro $i" >&2

    # lancer la simulation en arrière plan,
    ./pcap $NQUAIS > $TMP.cap 2>&1 &
    PIDCAP=$!
    sleep 0.1

    # Tous les navires doivent accoster
    PIDNAV=""
    for m in $NAVIRES
    do
	# lancer les navires (hum... toujours façon de parler)
	./pnav $m $NCONTENEURS $TA $TD > $TMP.nav$m 2>&1 &
	PIDNAV="$PIDNAV $!"
    done
    # temps min à attendre = ceil(card(NAVIRES)/QUAIS))
    # calcul d'une approximation (ceil => +1, on ajoute 100 pour la marge)
    nnav=$(echo $NAVIRES | wc -w)
    temps=$(( TA*(nnav/NQUAIS+1) ))
    sleep $(echo "scale=3;$temps/1000+0.1" | bc)

    # on décharge les navires cette fois-ci en parallèle
    # Temps de déchargement = TD + (NCONTENEURS-1)*max(TD, TC) + TC
    PIDCAM=""
    for m in $NAVIRES
    do
	time ./pfcam $m $NCONTENEURS $TC > $TMP.cam$m 2>&1 &
	PIDCAM="$PIDCAM $!"
    done
    if [ $TD -ge $TC ]
    then temps=$TD
    else temps=$TC
    fi
    temps=$(( (NCONTENEURS-1)*temps + TD + TC ))
    sleep $(echo "scale=3;$temps/1000" | bc)
    sleep 0.1			# marge

    # tous les camions devraient maintenant avoir terminé
    for pid in $PIDCAM
    do
	ps_termine $pid		"camion $pid non terminé"
    done
    # et les navires aussi
    for pid in $PIDNAV
    do
	ps_termine $pid		"navire $pid non terminé"
    done

    ./pstop >&2
    sleep 0.1

    ps_termine $PIDCAP		"pcap non terminé"

    i=$((i+1))
done

logs_aux
echo ok
exit 0
