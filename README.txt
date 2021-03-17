Mit help können Sie sich alle Befehle anzeigen lassen. Die Syntax ist die gleiche wia man sie aus der normalen Shell kennt außer bei cat und wc.
Beispiele für cat:
cat Datei1         (gibt Datei1 aus)
cat Datei1 Datei2    (gibt Datei und Datei2 aus)
cat Datei1 Datei2 > Datei 3     (speichert Inhalt von Datei1 und Datei2 in Datei3)

Nur diese 3 Eingaben funkionieren bei wc:
wc -c Datei	(gibt Anzahl Characters aus)	
wc -w Datei	(gibt Anzahl wörter aus)
wc -l Datei	(gibt Anzahl Zeilen aus)

Die Shell hat eine Autovervollständigung mit Tab (Nur auf Windows, da ich nicht weiß, wie man in Linux direkt auf Input zugreifen kann). Das kann zu zwei Problemen führen:
1. Problem: Wenn Sie etwas schreiben und es wieder löschen wollen, weil Sie sich zum Beispiel vertippt haben, kann es vorkommen, dass die Buchstaben nicht richtig gelöscht werden.
		Um dieses Problem zu beheben müssen Sie einfach in Zeile 57 der shell.c (nicht commands.h) beim printf("      "); einige Leerzeichen mehr machen.
2. Problem: Wenn bei jedem Buchstaben den Sie tippen eine oder mehrere neue Zeilen gemacht werden, dann sollten Sie zuerst versuchen die Shell auf Vollbild zu machen.
		Wenn das nichts bringt, dann einfach wieder in der Zeile 57 einige Leerzeichen löschen.

Entschuldigung für diese Probleme, habe lange versucht es zu reparieren, aber habe es nicht hinbekommen.

Ein weiteres Problem gibt es beim Befehl nano. Ich habe versucht einen nano Editor nachzuprogrammieren, der den Text der bereits in der Datei steht und den Text den man selbst schreibt im RAM speichert.
Wenn man aber zu viel schreibt, wird das anlegen im RAM schnell zu einem Problem und die Shell stürzt ab. Wenn man nur sehr wenig an der Datei verändert, dann funktioniert es.
Auch diesen Fehler konnte ich nicht beheben, da wir auch mit Prof Larcher noch nicht sehr oft mit einem C Programm auf den Speicher zugegriffen haben.

Der Rest sollte auf Windows/Linux funktionieren.
Chmod logischerweise nur auf Linux
Bei ls -l werden auch nur auf Linux die Rechte angezeigt
