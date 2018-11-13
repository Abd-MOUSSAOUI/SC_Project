#!/bin/sh

#
# Tests basiques de vérification de syntaxe
#

. ./ftest.sh

###############################################################################
# Tests d'arguments invalides
# (on attend un message d'erreur du type "usage: ..." pour être sûr
# que le problème de syntaxe est bien détecté)

$V ./pcap               2> $TMP >&2 || tu && fail "pcap: nb arg (0) invalide"
$V ./pcap 1 2           2> $TMP >&2 || tu && fail "pcap: nb arg (2) invalide"
$V ./pcap 1 2 3         2> $TMP >&2 || tu && fail "pcap: nb arg (3) invalide"
$V ./pcap a             2> $TMP >&2 || tu && fail "pcap: arg invalide"
$V ./pcap -5            2> $TMP >&2 || tu && fail "pcap: nb quais < 0"
$V ./pcap 0             2> $TMP >&2 || tu && fail "pcap: nb quais = 0"

$V ./pnav               2> $TMP >&2 || tu && fail "pnav: nb arg (0) invalide"
$V ./pnav a             2> $TMP >&2 || tu && fail "pnav: nb arg (1) invalide"
$V ./pnav a b           2> $TMP >&2 || tu && fail "pnav: nb arg (2) invalide"
$V ./pnav a b c         2> $TMP >&2 || tu && fail "pnav: nb arg (3) invalide"
$V ./pnav a b c d e     2> $TMP >&2 || tu && fail "pnav: nb arg (5) invalide"
$V ./pnav a -1 0 0      2> $TMP >&2 || tu && fail "pnav: nb conteneurs < 0"
$V ./pnav a 0 0 0       2> $TMP >&2 || tu && fail "pnav: nb conteneurs = 0"
$V ./pnav a 1 -1 0      2> $TMP >&2 || tu && fail "pnav: ta < 0"
$V ./pnav a 0 0 -1      2> $TMP >&2 || tu && fail "pnav: tc < 0"

$V ./pstop a            2> $TMP >&2 || tu && fail "pstop: nb arg (1) invalide"
$V ./pstop a b          2> $TMP >&2 || tu && fail "pstop: nb arg (2) invalide"

$V ./pfcam              2> $TMP >&2 || tu && fail "pfcam: nb arg (0) invalide"
$V ./pfcam a            2> $TMP >&2 || tu && fail "pfcam: nb arg (1) invalide"
$V ./pfcam a b          2> $TMP >&2 || tu && fail "pfcam: nb arg (2) invalide"
$V ./pfcam a b c d      2> $TMP >&2 || tu && fail "pfcam: nb arg (4) invalide"
$V ./pfcam a -1 0       2> $TMP >&2 || tu && fail "pfcam: nb camions < 0"
$V ./pfcam a 0 0        2> $TMP >&2 || tu && fail "pfcam: nb camions = 0"
$V ./pfcam a 1 -1       2> $TMP >&2 || tu && fail "pfcam: tc < 0"

$V ./pdump a            2> $TMP >&2 || tu && fail "pdump: nb arg (1) invalide"
$V ./pdump a b          2> $TMP >&2 || tu && fail "pdump: nb arg (2) invalide"

$V ./pclean a           2> $TMP >&2 || tu && fail "pclean: nb arg (1) invalide"
$V ./pclean a b         2> $TMP >&2 || tu && fail "pclean: nb arg (2) invalide"

logs_aux
echo "ok"
exit 0
