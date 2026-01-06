// =============================================================================
// MODUŁ: Gra w Kości (Kasyno RP) – Prospect RP
// ChatGPT 2024 – wersja dostosowana do def_colors.inc Prospect Roleplay
// =============================================================================

#if defined _KOSTKA_INCLUDED
    #endinput
#endif
#define _KOSTKA_INCLUDED

// ============================= KONFIG ==============================
#define CASINO_X        2000.0
#define CASINO_Y        1500.0
#define CASINO_Z        10.0
#define CASINO_RADIUS   50.0
#define MAX_BET         100000000
#define TAX_PERCENT     5
#define COOLDOWN_SECONDS 10
// ==================================================================
new PlayerData[MAX_PLAYERS+1][pEnum];
// ========================== FUNKCJE ===============================
stock bool:IsInCasinoArea(playerid)
{
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    return (GetDistanceBetweenPoints(x, y, z, CASINO_X, CASINO_Y, CASINO_Z) <= CASINO_RADIUS);
}

stock DiceRoll() return random(6) + 1;

stock GetPlayerNameEx(playerid)
{
    static n[MAX_PLAYER_NAME];
    GetPlayerName(playerid, n, sizeof n);
    return n;
}

stock LogDice(const text[])
{
    new File:f = fopen("log_dice.txt", io_append);
    if(f)
    {
        new line[256];
        new d, m, y, h, min, s;
        getdate(y, m, d);
        gettime(h, min, s);
        format(line, sizeof line, "[%02d/%02d/%04d %02d:%02d:%02d] %s\r\n", d, m, y, h, min, s, text);
        fwrite(f, line);
        fclose(f);
    }
}

stock ResetDiceData(playerid, targetid)
{
    PlayerData[playerid][pDiceOffer] = INVALID_PLAYER_ID;
    PlayerData[targetid][pDiceOffer] = INVALID_PLAYER_ID;
    PlayerData[playerid][pDiceBet] = 0;
    PlayerData[targetid][pDiceBet] = 0;
    PlayerData[playerid][pDiceRolls] = 0;
    PlayerData[targetid][pDiceRolls] = 0;
}

stock EndDiceGame(winner, loser)
{
    new bet = PlayerData[winner][pDiceBet];
    new total = bet * 2;
    new tax = (total * TAX_PERCENT) / 100;
    new payout = total - tax;

    GivePlayerCash(winner, payout);

    new msg[128];
    format(msg, sizeof msg, "{9ACD32}Wygrałeś grę w kości o %s (po potrąceniu %s podatku kasyna).", FormatNumber(payout), FormatNumber(tax));
    SendClientMessage(winner, COLOR_LIGHTGREEN, msg);
    format(msg, sizeof msg, "{FF6347}Przegrałeś grę! %s zgarnął %s.", GetPlayerNameEx(winner), FormatNumber(payout));
    SendClientMessage(loser, COLOR_LIGHTRED, msg);

    new logText[256];
    format(logText, sizeof logText, "%s (ID:%d) wygrał z %s (ID:%d) grę za %s (%d rzutów)", GetPlayerNameEx(winner), winner, GetPlayerNameEx(loser), loser, FormatNumber(bet), PlayerData[winner][pDiceRolls]);
    LogDice(logText);

    ResetDiceData(winner, loser);
    PlayerData[winner][pLastBet] = gettime();
    PlayerData[loser][pLastBet] = gettime();
}
// ==================================================================

// ========================== KOMENDY ===============================
CMD:sprawdzkase(playerid, const params[])
{
    if(!IsInCasinoArea(playerid))
        return SendClientMessage(playerid, COLOR_RED, "Musisz znajdować się w kasynie!");

    new targetid;
    if(sscanf(params,"u",targetid))
        return SendClientMessage(playerid,COLOR_SYNTAX,"Użycie: /sprawdzkase [id/nick]");
    if(!IsPlayerConnected(targetid))
        return SendClientMessage(playerid,COLOR_RED,"Gracz jest offline.");

    new msg[128];
    format(msg,sizeof msg,"%s ma przy sobie %s.",GetPlayerNameEx(targetid),FormatNumber(GetPlayerMoney(targetid)));
    SendClientMessage(playerid,COLOR_AQUA,msg);
    return 1;
}

CMD:kostka(playerid,const params[])
{
    new arg[16];
    if(sscanf(params,"s[16]",arg))
    {
        SendClientMessage(playerid,COLOR_SYNTAX,"Użycie: /kostka [id/nick] [kwota] [rzuty] | /kostka akceptuj | /kostka odrzuc | /kostka anuluj");
        return 1;
    }

    if(!strcmp(arg,"akceptuj",true))
    {
        new opp = PlayerData[playerid][pDiceOffer];
        if(opp==INVALID_PLAYER_ID || !IsPlayerConnected(opp))
            return SendClientMessage(playerid,COLOR_RED,"Nie masz żadnej oferty gry.");

        if(GetPlayerMoney(playerid) < PlayerData[opp][pDiceBet])
            return SendClientMessage(playerid,COLOR_RED,"Brakuje Ci pieniędzy!");
        if(!IsInCasinoArea(playerid))
            return SendClientMessage(playerid,COLOR_RED,"Musisz być w kasynie.");

        SendClientMessage(playerid, COLOR_LIGHTGREEN, "Zaakceptowałeś grę!");
        SendClientMessage(opp, COLOR_LIGHTGREEN, "Twój przeciwnik zaakceptował! Gra się rozpoczyna.");

        GivePlayerCash(playerid, -PlayerData[opp][pDiceBet]);
        GivePlayerCash(opp, -PlayerData[opp][pDiceBet]);

        new r = PlayerData[opp][pDiceRolls];
        new s1, s2;
        for(new i; i < r; i++)
        {
            new d1 = DiceRoll(), d2 = DiceRoll();
            s1 += d1;
            s2 += d2;
            new txt[128];
            format(txt,sizeof txt,"{FFFF90}Rzut #%d: %s wyrzuca %d | %s wyrzuca %d", i+1, GetPlayerNameEx(opp), d1, GetPlayerNameEx(playerid), d2);
            SendClientMessage(playerid, COLOR_WHITE, txt);
            SendClientMessage(opp, COLOR_WHITE, txt);
        }

        if(s1 == s2)
        {
            SendClientMessage(playerid, COLOR_YELLOW, "Remis! Dogrywka do skutku!");
            SendClientMessage(opp, COLOR_YELLOW, "Remis! Dogrywka do skutku!");
            do {
                new d1 = DiceRoll(), d2 = DiceRoll();
                if(d1 != d2)
                {
                    if(d1 > d2) EndDiceGame(opp, playerid);
                    else EndDiceGame(playerid, opp);
                    break;
                }
            } while(true);
        }
        else if(s1 > s2) EndDiceGame(opp, playerid);
        else EndDiceGame(playerid, opp);
        return 1;
    }

    if(!strcmp(arg,"odrzuc",true) || !strcmp(arg,"anuluj",true))
    {
        new opp = PlayerData[playerid][pDiceOffer];
        if(opp == INVALID_PLAYER_ID)
            return SendClientMessage(playerid, COLOR_RED, "Brak aktywnej oferty.");

        new text[128];
        if(!strcmp(arg,"anuluj",true))
            format(text,sizeof text,"{FFD200}%s anulował ofertę gry w kości.",GetPlayerNameEx(playerid));
        else
            format(text,sizeof text,"{FF6347}%s odrzucił ofertę gry w kości.",GetPlayerNameEx(playerid));

        SendClientMessage(opp,COLOR_RED,text);
        ResetDiceData(playerid,opp);
        return 1;
    }

    new targetid, bet, rolls;
    if(sscanf(params,"uii",targetid,bet,rolls))
        return SendClientMessage(playerid, COLOR_SYNTAX, "Użycie: /kostka [id/nick] [kwota] [rzuty]");

    if(!IsPlayerConnected(targetid))
        return SendClientMessage(playerid,COLOR_RED,"Gracz jest offline.");
    if(targetid==playerid)
        return SendClientMessage(playerid,COLOR_RED,"Nie możesz grać sam ze sobą!");
    if(bet <= 0 || bet > MAX_BET)
        return SendClientMessage(playerid,COLOR_RED,"Kwota 1$–100.000.000$.");
    if(rolls < 1 || rolls > 6)
        return SendClientMessage(playerid,COLOR_RED,"Liczba rzutów 1–6.");
    if(GetPlayerMoney(playerid) < bet)
        return SendClientMessage(playerid,COLOR_RED,"Nie masz tyle pieniędzy!");
    if(!IsInCasinoArea(playerid))
        return SendClientMessage(playerid,COLOR_RED,"Musisz być w kasynie!");

    PlayerData[playerid][pDiceOffer] = targetid;
    PlayerData[targetid][pDiceOffer] = playerid;
    PlayerData[playerid][pDiceBet] = bet;
    PlayerData[playerid][pDiceRolls] = rolls;

    new msg[128];
    format(msg,sizeof msg,"{FFD200}%s oferuje Ci grę w kości o %s na %d rzutów!",GetPlayerNameEx(playerid),FormatNumber(bet),rolls);
    SendClientMessage(targetid,COLOR_YELLOW,msg);
    SendClientMessage(targetid,COLOR_GRY3,"Akceptuj: /kostka akceptuj | Odrzuć: /kostka odrzuc");
    SendClientMessage(playerid,COLOR_LIGHTGREEN,"Oferta wysłana! /kostka anuluj aby anulować.");

    new log[256];
    format(log,sizeof log,"%s (ID:%d) zaprosił %s (ID:%d) – %s (%d rzutów).",GetPlayerNameEx(playerid),playerid,GetPlayerNameEx(targetid),targetid,FormatNumber(bet),rolls);
    LogDice(log);
    return 1;
}
// ==================================================================

// ========================== KASYNO INFO ===========================
stock CheckPlayerCasinoEntry(playerid)
{
    new bool:inside = IsInCasinoArea(playerid);

    if(inside && !PlayerData[playerid][pInCasino])
    {
        PlayerData[playerid][pInCasino] = true;
        SendClientMessage(playerid, COLOR_YELLOW3, "Witamy w kasynie! Życzymy samych wygranych!");
        SendClientMessage(playerid, COLOR_WHITE, "Graj rozważnie — szczęście jest zdradliwe.");
    }
    else if(!inside && PlayerData[playerid][pInCasino])
        PlayerData[playerid][pInCasino] = false;
}