#define	WELCOME_DIALOG	7003

stock displayWelcomeMessage(playerid) {
    new msgstring[2048]; //2048
    new name[] = "name";
    
    format(msgstring, sizeof(msgstring), "{FFFFFF}Witamy na {00aa00}Prospect Roleplay{FFFFFF}.\nProsimy, abyś poświęcił chwilę na zapoznanie się z zasadami, aby zapewnić sobie przyjemną rozgrywkę.", name);
    
    format(msgstring, sizeof(msgstring), "%s\n\n{FFFF91}Informacje o serwerze:{FFFFFF}\nTen serwer jest serwerem roleplay, co oznacza, że musisz pozostawać w roli postaci przez cały czas.\nTwój sukces w dołączaniu do frakcji lub gangów może zależeć od Twoich umiejętności roleplay. Jeśli jesteś nowy w tym\ntemacie, odwiedź naszą stronę internetową po poradniki RP, tutoriale i inne przydatne informacje.", msgstring);
    
    format(msgstring, sizeof(msgstring), "%s\n\n{FFFF91}Zasady:{FFFFFF}", msgstring);
    format(msgstring, sizeof(msgstring), "%s\nStaramy się promować zdrowe środowisko roleplay, prosimy o przestrzeganie poniższych zasad:", msgstring);
    
    format(msgstring, sizeof(msgstring), "%s\n   - Zabronione jest deathmatching ani strzelanie do innych graczy bez uzasadnienia w roli postaci.", msgstring);
    format(msgstring, sizeof(msgstring), "%s\n   - Zabroniona jest reklama serwera.", msgstring);
    format(msgstring, sizeof(msgstring), "%s\n   - Zabronione jest zabijanie lub potrącanie graczy pojazdami (w tym helikopterami).", msgstring);
    
    format(msgstring, sizeof(msgstring), "%s\n   - Serwer stosuje zerową tolerancję wobec hakowania. Łamiąc te zasady ryzykujesz permanentnym banem.", msgstring);
    
    format(msgstring, sizeof(msgstring), "%s\n\n{FFFF91}Dołączanie do społeczności:{FFFFFF}", msgstring);
    
    format(msgstring, sizeof(msgstring), "%s\nZapraszamy do naszej społeczności na {8D8DFF}forum.prospectrp.net{FFFFFF}. Rejestracja na forum pozwoli Ci", msgstring);
    format(msgstring, sizeof(msgstring), "%s\nskładać wnioski o dołączenie do frakcji, gangów i wiele innych, a także być na bieżąco z aktualizacjami i wydarzeniami.", msgstring);
    format(msgstring, sizeof(msgstring), "%s\nJeśli grasz u nas po raz pierwszy, zaleca się przejście tutorialu, który\nzapozna Cię z kluczowymi funkcjami serwera i pozwoli szybko rozpocząć rozgrywkę.", msgstring);
    
    ShowPlayerDialog(playerid, WELCOME_DIALOG, DIALOG_STYLE_MSGBOX, "Informacje", msgstring, "Graj", "");
}