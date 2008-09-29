Protmušio programa
==================

Domas Monkus
domas.m@gmail.com

Naudojimas
==========
Klavišai:
F1 - paleisti laikmatį
F2 - pauzė
F3 - atstatyti į pradinį laiką

g - nustatymai

1,2 - pirmos komandos taškai (1 - pridėti tašką, 2 - atimti)
3,4 - antros komandos taškai
5,6 - trečios
7,8 - ketvirtos

Lygiosios: yra galimybė (nors ir nedidelė), kad komandos mygtukus paspaus vienu metu.
Tada bus paskelbtos lygiosios. Jų atveju laikas sustabdomas, tačiau pratęsus raundą 
(spausti F1),  lygiąsias patekusios komandos galės vėl spausti mygtukus.

Falšstartas: falšstartavusi komanda (arba komandos, jei buvo lygiosios nepasibaigus
signalui) pažymima raudonai. Laikas nestabdomas. Tame raunde ji varžytis nebegali.

Raundo pradžios signalas: signalas gali būti atsitiktinės trukmės arba fiksuotos. Keičiama
per nustatymus.

Plėtojimas
==========

Programos kodas laikomas Google Code sistemoje:
http://code.google.com/p/pm-game/

Pasiimti jo kopiją galima arba iš projekto puslapio, arba naudojant svn:
svn checkout http://pm-game.googlecode.com/svn/trunk/ pm-game-read-only

Jeigu bus norinčių tobulinti programą ir pakeitimus saugoti versijų kontrolės sistemoje,
rašykite domas.m@gmail.com - suteiksim rašymo teises.

Programa naudoja wxWidgets biblioteką (http://www.wxwidgets.org/), tiksliau - jos unikodinį
build'ą. Kompiliuota GCC kompiliatoriumi (MingW), nors galima naudoti ir kitus - skirsis tik
bendro duomenų segmento deklaracija. Viskam galima naudoti wxDev-C++ IDE 
(http://wxdsgn.sourceforge.net/).

Unikodinį wxWidgets buildą galima pasidaryti pagal šias instrukcijas:
http://wxforum.shadonet.com/viewtopic.php?t=1764




