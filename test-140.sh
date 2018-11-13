#!/bin/sh

#
# Tests de verbosité
#
# On essaye les niveaux 0, 1 et 9999. Normalement, à chaque fois,
# il devrait y avoir plus d'infos dans les différents fichiers
# générés.
#

. ./ftest.sh

if [ ! -z "$V" ]
then
    echo "ok (test incompatible avec valgrind)"
    exit 0
fi

M=roi-des-mers		# nom du navire
Q=2			# nombre de quais
C=5			# conteneurs

LISTESUFF="cap nav fcam stp"	# suffixes utilisés dans lancer_test_complet

#
# Une petite fonction pour lancer un test complet
# avec le niveau de debug $1 (0, 1, etc.)
#

lancer_test_complet ()
{
    local D=$1

    DEBUG_PORT=$D
    export DEBUG_PORT

    $V ./pclean 2>/dev/null >&2

    $V ./pcap $Q > $TMP.cap$D 2>&1 &
    PIDCAP=$!

    # vérifier qu'il a bien démarré
    sleep $MARGE		# délai pour laisser pcap démarrer
    ps_existe $PIDCAP		"pcap pas démarré ($D)"

    $V ./pnav $M $C 0 0 > $TMP.nav$D 2>&1 &
    PIDNAV=$!

    # vérifier qu'il a bien démarré
    sleep $MARGE		# délai pour laisser le navire démarrer
    ps_existe $PIDNAV		"pnav pas démarré ($D)"

    # demander l'arrêt
    $V ./pstop > $TMP.stp$D 2>&1

    sleep $MARGE		# vérifier que le navire est toujours là
    ps_existe $PIDNAV		"pnav arrêté prématurément ($D)"

    # attendre les camions
    $V ./pfcam $M $C 0       > $TMP.fcam$D 2>&1 || fail "camions ($D)"

    # attendre que tout se termine
    sleep $MARGE

    ps_termine $PIDNAV		"navire pas arrêté ($D)"
    ps_termine $PIDCAP		"capitainerie pas arrêtée ($D)"
}

# $1 : niveau 1
# $2 : niveau 2
comparer ()
{
    local s f1 f2

    for s in $LISTESUFF
    do
	f1=$TMP.$s$1
	f2=$TMP.$s$2
	cmp -s $f1 $f2 && fail "Fichiers $f1 et $f2 identiques"
    done
}


##############################################################################
# Premier test
# - normalement, comme il n'y a pas d'erreur, tous les fichiers sont vides

lancer_test_complet 0

for s in $LISTESUFF
do
    f1=$TMP.${s}0
    [ $(wc -l < $f1) -ne 0 ] && fail "Fichier $f1 non vide"
done

##############################################################################
# Deuxième test
# - au niveau 1, les fichiers ne devraient pas être vides

lancer_test_complet 1

comparer 0 1

##############################################################################
# Troisième test
# - niveau maximum, les fichiers devraient être dfférents

lancer_test_complet 9999

comparer 0 9999

logs_aux
echo "ok"
exit 0
