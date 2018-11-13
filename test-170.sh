#!/bin/sh

#
# Test (long) pour détecter d'éventuels problèmes de concurrence
#

. ./ftest.sh

if [ ! -z "$V" ]
then
    echo "ok (test incompatible avec valgrind)"
    exit 0
fi

# nombre de simulations
NSIMU=3
# nombre de quais
NQUAIS=3
# on va travailler sur ces navires
NAVIRES="a b c d e f g h"
# qui transportent ce nombre de conteneurs
NCONTENEURS=3
# les temps
TA=50				# accostage
TD=50				# déchargement du navire
TC=100				# chargement sur le camion

# avant toute chose, partir d'une place nette
$V ./pclean 2> /dev/null >&2

# ces tests peuvent parfois échouer... si on n'a pas de chance... mais
# seulement si on a mal synchronisé les processus entre eux. Du coup,
# on les fait tourner plusieurs fois pour avoir un filtre le plus fin
# possible

i=1
while [ $i -le $NSIMU ]
do
    echo "simulation numéro $i" >&2

    # lancer la simulation en arrière plan
    $V ./pcap $NQUAIS > $TMP.cap 2>&1 &
    PIDCAP=$!
    sleep 0.1

    # tous ces navires en compétition pour si peu de quais...
    PIDNAV=""
    for m in $NAVIRES
    do
	# lancer les navires (façon de parler), mais ils sont lents...
	$V ./pnav $m $NCONTENEURS $TA $TD > $TMP.nav$m 2>&1 &
	PIDNAV="$PIDNAV $!"
    done
    # temps min à attendre = ceil(card(NAVIRES)/QUAIS))
    # calcul d'une approximation (ceil => +1, on ajoute 100 pour la marge)
    nnav=$(echo $NAVIRES | wc -w)
    temps=$(( TA*(nnav/NQUAIS+1) ))
    sleep $(echo "scale=3;$temps/1000" | bc)
    sleep 0.1			# marge

    # liste des navires déjà déchargés
    nav_decharges=""

    # on décharge les navires en séquence, i.e. on ne teste pas
    # tous les problèmes potentiels de concurrence entre camions
    ntour=1
    while [ $(echo $nav_decharges | wc -w) != $(echo $NAVIRES | wc -w) ]
    do
	echo "nav_decharges = $nav_decharges au tour $ntour" >&2
	ndecharges=0
	for m in $NAVIRES
	do
	    # cette fois-ci, on attend la fin
	    $V ./pfcam $m $NCONTENEURS $TC >&2
	    if [ $? = 0 ]
	    then
		nav_decharges="$nav_decharges $m"
		ndecharges=$((ndecharges+1))
	    fi
	done
	test $ndecharges = 0	&& fail "aucun navire déchargé au tour $ntour"
	ntour=$((ntour+1))
    done
    sleep 0.1

    # à la fin, tous les navires doivent avoir terminé
    for pid in $PIDNAV
    do
	ps_termine $pid		"pnav pid=$pid non termine"
    done

    $V ./pstop >&2
    sleep 0.1

    ps_termine $PIDCAP		"pcap non terminé"

    i=$((i+1))
done

logs_aux
echo ok
exit 0
