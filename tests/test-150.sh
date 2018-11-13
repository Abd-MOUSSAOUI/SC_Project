#!/bin/sh

#
# Vérifier les différents temps d'attente => test lent (8 sec)
# On ne teste pas avec valgrind, car cela fausse terriblement les temps
#

. ./ftest.sh

if [ ! -z "$V" ]
then
    echo "ok (test incompatible avec valgrind)"
    exit 0
fi

# avant toute chose, partir d'une place nette
./pclean 2> /dev/null >&2

# lancer la simulation en arrière plan
./pcap 1  > $TMP.cap 2>&1 &
PIDCAP=$!
sleep 0.1

#
# Premier test pour un navire x
#

# mesurer le temps pour accoster : 1 seconde
./pnav x 4 1000 700 > $TMP.navx 2>&1 &
PIDNAV=$!

sleep 0.9
# là, c'est encore trop tôt pour trouver le navire
./pfcam x 1 0 >&2		&& fail "nav non trouvé => trop tôt"

sleep 0.2
# maintenant, à 1,1 sec du démarrage, il devrait être à quai
./pfcam x 1 0 >&2		|| fail "nav non trouvé : devrait être à quai"

# on a déchargé un conteneur, on va décharger les 3 qui restent
# le temps total de l'opération de transbordement devrait
# être d'environ : 700 + 2*max(700,400) + 400 = 2500 ms
c=$(chrono_start)
time ./pfcam x 3 400 >&2	|| fail "échec déchargement x"
dur=$(chrono_end $c)
test $dur -ge 2 -a $dur -le 3	|| fail "temps déchargement x invalide"

sleep 0.1
ps_termine $PIDNAV		"pnav $PIDCAP non terminé"


#
# Deuxième test avec un navire y
#

./pnav y 3 0 700 > $TMP.navy 2>&1 &
PIDNAV=$!
sleep 0.1

# le temps total de l'opération de transbordement devrait
# être de : 700 + 2*max(700,900) + 900 = 3400 ms
# le temps initial peut être en partie absorbé par le précédent sleep 0.1
c=$(chrono_start)
time ./pfcam y 3 900 >&2	|| fail "échec déchargement y"
dur=$(chrono_end $c)
test $dur -ge 3 -a $dur -le 4	|| fail "temps déchargement y invalide"

sleep 0.1
ps_termine $PIDNAV		"pnav $PIDCAP non terminé"

./pstop >&2

sleep 0.1
ps_termine $PIDCAP		"pcap $PIDCAP non terminé"

logs_aux
echo ok
exit 0
