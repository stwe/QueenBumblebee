# QueenBumblebee

## TODOs

### Position history

### Remis

#### Patt
Eine Stellung, in der der Spieler am Zug nicht im Schach steht, aber keinen legalen Zug mehr hat.

Keine legalen Züge -> Patt vs Matt.

Wenn GenerateLegalMoves() leer ist:
- steht Seite am Zug im Schach? -> Matt
- Sonst -> Patt

- mit bestmove 0000 antworten

#### 50-Züge-Regel
50 Züge pro Seite ohne Bauernzug oder Schlag → Remis.

- positionState.halfMoveClock >= 100 -> Score 0

- mit bestmove 0000 antworten

#### Dreifache Stellungswiederholung
Wenn dieselbe Stellung dreimal mit denselben Zugmöglichkeiten entsteht → Remis.

- Zughistory -> Score 0

#### Unzureichendes Material
Kein Spieler kann Matt setzen:
- Nur Könige
- König + Läufer gegen König
- König + Springer gegen König
- König + zwei Springer gegen König ist ebenfalls nicht forcierbar → Remis.
- König + Läufer gegen König + Läufer mit Läufern auf derselben Feldfarbe → Remis.
