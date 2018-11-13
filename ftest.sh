#
# Fonctions et variables auxiliaires utilisées pour les différents
# tests.
#

TEST=$(basename $0 .sh)

TMP=/tmp/$TEST-$$
LOG=$TEST.log
V=${VALGRIND}		# appeler avec la var. VALGRIND à "" ou "valgrind -q"

DEBUG_PORT=${DEBUG_PORT:-1} ; export DEBUG_PORT

# Rediriger stderr vers le log pour voir les résultats des tests
# On conserve stdout inchangé, il faudra le rediriger à la demande
exec 2> $LOG

set -u			# erreur si utilisation d'une variable non définie
set -x			# mode trace

if [ -z "$V" ]
then MARGE=0.1		# 100 ms devraient suffire pour avoir une marge
else MARGE=1.0		# marge agrandie car vagrind ralentit l'exécution
fi

###############################################################################
# Fichiers de log auxiliaires

logs_aux ()
{
    set +x
    for f in $(ls -d /tmp/* | grep "^$TMP")
    do
	echo "==> log aux $f"
	cat $f
    done >&2
    rm -f $TMP.*
}

###############################################################################
# Une fonction qu'il vaudrait mieux ne pas avoir à appeler...

fail ()
{
    echo "==> Échec du test '$TEST' sur '$1'."
    echo "==> Log : '$LOG'."
    logs_aux
    echo "==> Exit"
    exit 1
}

###############################################################################
# Teste la présence du traditionnel message : "usage: prog arg..." dans $TMP
# Renvoie vrai si trouvé, faux si pas trouvé

tu ()
{
    # rappel: "! cmd" => inverse le code de retour de cmd
    ! grep -q "usage: " $TMP
}

###############################################################################
# Teste si le processus existe toujours et juste renvoyer vrai si toujours là

ps_test ()
{
    kill -0 $1 2> /dev/null
}

###############################################################################
# Teste si le processus existe toujours, sinon signale l'erreur

ps_existe ()
{
    kill -0 $1 2> /dev/null || fail "$2"
}

###############################################################################
# Teste si le processus est bien terminé, sinon signale l'erreur

ps_termine ()
{
    kill -0 $1 2> /dev/null && fail "$2"
}

###############################################################################
# Chronomètre à utiliser de la façon suivante :
#	c=$(chrono_start)
#	...
#	duree=$(chrono_end $c)

chrono_start ()
{
    date +%s
}

chrono_end ()
{
    local start=$1 end
    end=$(date +%s)
    # renvoyer la durée
    echo $((end-start))
}
