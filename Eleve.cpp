#pragma warning(disable : 4996)

#include "G2D.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define ECRAN_ACCUEIL 0
#define ECRAN_OPTIONS 1
#define INIT_PARTIE 2
#define ECRAN_JEU 3
#define ECRAN_GAME_OVER 4
#define ECRAN_WIN 5

#define SCORE_MOMIE 200
#define SCORE_DIAMOND 150

#define COMPTEUR_TRAP 50
#define MIN_TRAP 70

struct _NiveauDonjon {
  string Map;
  string Map2 = "MMMMMMMMMMMMMMM"
                "M M           M"
                "M M M MMM MMM M"
                "M   M       M M"
                "MMM M M MMM M M"
                "M   M M     M M"
                "M MMM MMM MMMMM"
                "M   M  M      M"
                "M M M  M M MM M"
                "M M M  M M M  M"
                "M M M MM M MMMM"
                "M M M    M    M"
                "M M M MMMMMMM M"
                "M M      M    M"
                "MMMMMMMMMMMMMMM";
  string Map1 = "MMMMMMMMMMMMMMM"
                "M   M         M"
                "M M M MMM MMM M"
                "M M M M     M M"
                "M M M M     MMM"
                "M     M     M M"
                "M MMM MMM MMM M"
                "M   M  M      M"
                "M M M  M M MM M"
                "M M M  M M MM M"
                "M MMM MM M MMMM"
                "M   M M      MM"
                "M M M M MMM  MM"
                "M M     MMM  MMM"
                "MMMMMMMMMMMMMMM";

  int niveau;
  void setNiveau(int _niveau) { niveau = _niveau; }
  int getNiveau() { return niveau; }
  void setTexture() {
    if (niveau == 1) {
      Map = Map1;
    } else if (niveau == 2) {
      Map = Map2;
    }
  }
};
struct Rectangle {
  int xMin, xMax, yMin, yMax;
  Rectangle(int _xMin, int _yMin, int _xMax, int _yMax) {
    xMin = _xMin, xMax = _xMax, yMin = _yMin, yMax = _yMax;
  }

  V2 getCoordonneeMin() { return V2(xMin, yMin); }
  V2 getCoordonneeMax() { return V2(xMax, yMax); }
};

struct _TexturePack {
  string textureMur = "[SGGWSSWS]"
                      "[GSSSGGSW]"
                      "[GSGGGGSS]"
                      "[WSSGGWGG]"
                      "[GSGWGGGG]"
                      "[SGGSWSGS]"
                      "[GGWSGGGS]"
                      "[GGGGSSGG]";
  string textureSol = "[LLFJLLFJ]"
                      "[LLFJLFFJ]"
                      "[LFFJLFFF]"
                      "[LFJJLFFJ]"
                      "[FFJJLFJJ]"
                      "[LFJJLFJJ]"
                      "[LFFJLFFJ]"
                      "[LLFJLFFJ]";

  V2 Size;
  int IdTexMur;
  int IdTexSol;
};
bool InterRectRect(Rectangle R1, Rectangle R2) {
  if (R1.yMax < R2.yMin) {
    return false;
  }
  if (R1.yMin > R2.yMax) {
    return false;
  }
  if (R1.xMin > R2.xMax) {
    return false;
  }
  if (R1.xMax < R2.xMin) {
    return false;
  }
  return true;
};
struct _Heros {
  int xMin, xMax, yMin, yMax, width;
  string texture = "[RRR  ]"
                   "[RRWR ]"
                   "[RRR  ]"
                   "[YY   ]"
                   "[YYY  ]"
                   "[YY YG]"
                   "[GG   ]"
                   "[CC   ]"
                   "[CC   ]"
                   "[C C  ]"
                   "[C C  ]";

  string texture2 = "[RRR  ]"
                    "[RRWR ]"
                    "[RRR  ]"
                    "[YY   ]"
                    "[YYY  ]"
                    "[YY YG]"
                    "[GG   ]"
                    "[CC   ]"
                    "[CCCC ]"
                    "[C   C]"
                    "[C    ]";

  bool typeTexture = false;
  int numTexture = 10;

  bool hasKey = false;
  bool hasGun = false;

  int score = 0;
  int nbBullets = 10;

  int LastDirection = 0;
  int getLastDirection() { return LastDirection; }
  void setLastDirection(int _LastDirection) { LastDirection = _LastDirection; }

  bool getHasKey() { return hasKey; }
  void setHasKey(bool pKey) { hasKey = pKey; }
  int nbVies = 3;

  V2 Size;
  int IdTex;
  V2 Pos = V2(45, 45);

  void changeTexture() {
    if (numTexture > 0) {
      numTexture = numTexture - 1;
    } else if (typeTexture) {

      numTexture = 10;
      typeTexture = !typeTexture;
      IdTex = G2D::InitTextureFromString(Size, texture);
      Size = Size * 2; // on peut zoomer la taille du sprite

    } else {
      numTexture = 10;
      typeTexture = !typeTexture;
      IdTex = G2D::InitTextureFromString(Size, texture2);
      Size = Size * 2; // on peut zoomer la taille du sprite
    }
  }

  void reset() {
    hasKey = false;
    hasGun = false;

    Pos = V2(45, 45);
  }
  Rectangle getRect() {
    return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
  }
};
struct _Life {
  string vivantTexture = "[   KKK    KKK   ]"
                         "[  KRRRK  KRRRK  ]"
                         "[ KRMRMRKKRRRRRK ]"
                         "[KRMWRRRRRRRRRRRK]"
                         "[KRWRRRRRRRRRRRRK]"
                         "[KRMRRRRRRRRRRRRK]"
                         "[KRRRRRRRRRRRRRRK]"
                         "[ KRRRRRRRRRRRRK ]"
                         "[ KRRRRRRRRRRRRK ]"
                         "[  KRRRRRRRRRRK  ]"
                         "[   KRRRRRRRRK   ]"
                         "[    KRRRRRRK    ]"
                         "[     KRRRRK     ]"
                         "[      KRRK      ]"
                         "[       KK       ]";

  string mortTexture = "[   KKK    KKK   ]"
                       "[  KKKKK  KKKKK  ]"
                       "[ KKKKKKKKKKKKKK ]"
                       "[KKKWKKKKKKKKKKKK]"
                       "[KKWKKKKKKKKKKKKK]"
                       "[KKKKKKKKKKKKKKKK]"
                       "[KKKKKKKKKKKKKKKK]"
                       "[ KKKKKKKKKKKKKK ]"
                       "[ KKKKKKKKKKKKKK ]"
                       "[  KKKKKKKKKKKK  ]"
                       "[   KKKKKKKKKK   ]"
                       "[    KKKKKKKK    ]"
                       "[     KKKKKK     ]"
                       "[      KKKK      ]"
                       "[       KK       ]";
  V2 Size;
  V2 Pos;
  int IdTex;
  bool exist;
  string texture;
  _Life(V2 _Pos) { Pos = _Pos; }
  void setTexture(string _Texture) { texture = _Texture; }
};
struct _Momie {
  string texture = "[    O    ]"
                   "[  BBOBB  ]"
                   "[ BBOOOB  ]"
                   "[BBBOOOBBB]"
                   "[OOOWWWOOO]"
                   "[BBWKWKWBB]"
                   "[OOWWWWWOO]"
                   "[BBWWRWWBB]"
                   "[BGGWKWGGB]"
                   "[BWGGKGGWB]"
                   "[ WWGKGWW ]"
                   "[ WWWGWWW ]"
                   "[ WWWWWWW ]"
                   "[ BWWWWWB ]"
                   "[ WBBBBBW ]"
                   "[  YYBYY  ]"

      ;
  V2 Size;
  int IdTex;
  V2 Pos;
  V2 Dir = V2(1, 0);
  bool estMorte;
  bool getMorte() { return estMorte; }
  void setMorte(bool _estMorte) { estMorte = _estMorte; }
  int changeCompteur = 50;

  _Momie(int x, int y) { Pos = V2(x, y); }

  bool isMomie(_Momie m) { return (m.Pos.x == Pos.x && m.Pos.y == Pos.y); }
  bool getTapeMomie(_Momie m) { return InterRectRect(m.getRect(), getRect()); }

  Rectangle getRect() {
    return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
  }
};

struct _Key {
  string texture = "[                   ]"
                   "[  W R         WWRW ]"
                   "[  W W        W    W]"
                   "[WRWWWWWRWWWWWW    W]"
                   "[             W    R]"
                   "[              WWWW ]";

  V2 Size;
  int IdTex;
  V2 Pos = V2(460, 50);

  void setPos(int x, int y) { Pos = V2(x, y); }
  Rectangle getRect() {
    return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
  }
};

struct _Chest {
  string texture = "[     KKKKKKKKKKKK     ]"
                   "[    KWWWWWWWWWWWWK    ]"
                   "[  KWGWWWWWWWWWWWWWWK  ]"
                   "[ KWGKGGWWWWWWWWWWKWWK ]"
                   "[KWGKKKGWWKKKKWWWKKKWWK]"
                   "[KWGRRGGWKKYYKKWWWRRWWK]"
                   "[KWWRWWWWWKKKKWWWWWRWWK]"
                   "[ KWWWRWWWWWWWWWWWWWWK ]"
                   "[  KWWWWWWWWWWWWWWWWK  ]"
                   "[   KKKKKKKKKKKKKKKK   ]";

  V2 Size;
  int IdTex;
  V2 Pos = V2(70, 530);

  void setPos(int x, int y) { Pos = V2(x, y); }

  bool isOpened = false;
  Rectangle getRect() {
    return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
  }
};
struct _Diamond {
  string texture = "[    KKKK    ]"
                   "[   KWWWWK   ]"
                   "[  KWCCCWCK  ]"
                   "[ KWWCCWCCCK ]"
                   "[ KWCWWWWCCK ]"
                   "[KWCWCCCCWCWK]"
                   "[KWCWCCCCWCCK]"
                   "[KWWWCCCCWCCK]"
                   "[KWCCWWWWCCCK]"
                   "[ KCCCCCCCCK ]"
                   "[ KWCCCCCCCK ]"
                   "[  KCCCCCCK  ]"
                   "[   KKKKKK   ]";

  V2 Size;
  int IdTex;
  V2 Pos;

  bool exist = true;
  _Diamond(V2 _Pos) { Pos = _Pos; }
  Rectangle getRect() {
    return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
  }
};

struct _Trap {
  string activeTexture = "[      WG                 WG       ]"
                         "[      SG                 SG       ]"
                         "[      WSG                WSG      ]"
                         "[     WWSG               WWSG      ]"
                         "[     WSSGG              WWSGG     ]"
                         "[    WWSSGG             WWWSGG     ]"
                         "[    WSSGGGG            WSSGGGG    ]"
                         "[   SWSSGGGGS          SWSSGGGGS   ]"
                         "[ SWSSSGGGGGGS       SWSSSGGGGGGS  ]"
                         "[SKSSGGGGGGGGGKS WG SKSSGGGGGGGGGKS]"
                         "[SKGGGGGGGGGGGKS SG SKGGGGGGGGGGGKS]"
                         "[ SGGGGGGGGGGGS  WSG SGGGGGGGGGGGS ]"
                         "[  SGGGGGGGGGS  WWSG  SGGGGGGGGGS  ]"
                         "[               WSSGG              ]"
                         "[              WWSSGG              ]"
                         "[              WSSGGGG             ]"
                         "[             SWSSGGGGS            ]"
                         "[           SWSSSGGGGGGS           ]"
                         "[      WG  SKSSGGGGGGGGKS WG       ]"
                         "[      SG  SKSSGGGGGGGGKS SG       ]"
                         "[      WSG  SGGGGGGGGGGS  WSG      ]"
                         "[     WWSG   SGGGGGGGGS  WWSG      ]"
                         "[     WSSGG              WWSGG     ]"
                         "[    WWSSGG             WWWSGG     ]"
                         "[    WSSGGGG            WSSGGGG    ]"
                         "[   SWSSGGGGS          SWSSGGGGS   ]"
                         "[ SWSSSGGGGGGS       SWSSSGGGGGGS  ]"
                         "[SKSSGGGGGGGGGKS    SKSSGGGGGGGGGKS]"
                         "[SKGGGGGGGGGGGKS    SKGGGGGGGGGGGKS]"
                         "[ SGGGGGGGGGGGS      SGGGGGGGGGGGS ]"
                         "[  SGGGGGGGGGS        SGGGGGGGGGS  ]";

  string willBeActiveTexture = "[                                  ]"
                               "[                                  ]"
                               "[                                  ]"
                               "[                                  ]"
                               "[                                  ]"
                               "[                                  ]"
                               "[                                  ]"
                               "[   SKKKKKKKS          SKKKKKKKS   ]"
                               "[ SKKKKKKKKKKS       SKKKKKKKKKKS  ]"
                               "[SKKKKWWGGKKKKKS    SKKKKWWGGKKKKKS]"
                               "[SKKKSSSGGGKKKKS    SKKKSSSGGGKKKKS]"
                               "[ SKKKSSGGKKKKS      SKKKSSGGKKKKS ]"
                               "[  SKKKKKKKKKS        SKKKKKKKKKS  ]"
                               "[                                  ]"
                               "[                                  ]"
                               "[                                  ]"
                               "[             SKKKKKKKS            ]"
                               "[           SKKKKKKKKKKS           ]"
                               "[          SKKKKWWGGKKKKKS         ]"
                               "[          SKKKSSSGGGKKKKS         ]"
                               "[           SKKKSSGGKKKKS          ]"
                               "[            SKKKKKKKKKS           ]"
                               "[                                  ]"
                               "[                                  ]"
                               "[                                  ]"
                               "[   SKKKKKKKS          SKKKKKKKS   ]"
                               "[ SKKKKKKKKKKS       SKKKKKKKKKKS  ]"
                               "[SKKKKWWGGKKKKKS    SKKKKWWGGKKKKKS]"
                               "[SKKKSSSGGGKKKKS    SKKKSSSGGGKKKKS]"
                               "[ SKKKSSGGKKKKS      SKKKSSGGKKKKS ]"
                               "[  SKKKKKKKKKS        SKKKKKKKKKS  ]";

  string inactiveTexture = "[                                  ]"
                           "[                                  ]"
                           "[                                  ]"
                           "[                                  ]"
                           "[                                  ]"
                           "[                                  ]"
                           "[                                  ]"
                           "[   SKKKKKKKS          SKKKKKKKS   ]"
                           "[ SKKKKKKKKKKS       SKKKKKKKKKKS  ]"
                           "[SKKKKKWKKKKKKKS    SKKKKKWKKKKKKKS]"
                           "[SKKKKSSKKKKKKKS    SKKKKSSKKKKKKKS]"
                           "[ SKKKKSKKKKKKS      SKKKKSKKKKKKS ]"
                           "[  SKKKKKKKKKS        SKKKKKKKKKS  ]"
                           "[                                  ]"
                           "[                                  ]"
                           "[                                  ]"
                           "[             SKKKKKKKS            ]"
                           "[           SKKKKKKKKKKS           ]"
                           "[          SKKKKKWKKKKKKKS         ]"
                           "[          SKKKKSSKKKKKKKS         ]"
                           "[           SKKKKSKKKKKKS          ]"
                           "[            SKKKKKKKKKS           ]"
                           "[                                  ]"
                           "[                                  ]"
                           "[                                  ]"
                           "[   SKKKKKKKS          SKKKKKKKS   ]"
                           "[ SKKKKKKKKKKS       SKKKKKKKKKKS  ]"
                           "[SKKKKKWKKKKKKKS    SKKKKKWKKKKKKKS]"
                           "[SKKKKSSKKKKKKKS    SKKKKSSKKKKKKKS]"
                           "[ SKKKKSKKKKKKS      SKKKKSKKKKKKS ]"
                           "[  SKKKKKKKKKS        SKKKKKKKKKS  ]";

  string texture = inactiveTexture;
  V2 Size;
  int IdTex;
  V2 Pos = V2(405, 50);
  int compteur = rand() % COMPTEUR_TRAP + MIN_TRAP;

  int active = 0;

  _Trap(int x, int y) { Pos = V2(x, y); }

  void setActive(bool _active) { active = _active; }
  int getActive() { return active; }

  void setTexture(string _texture) { texture = _texture; }

  Rectangle getRect() {
    return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
  }
};
struct _Gun {
  string texture = "[   K                        ]"
                   "[  KGK                    KKK]"
                   "[  KGGKKKKKKKKKKKKKKKKKKKKGGK]"
                   "[   KGGMMGGGGGGMMMMMMMMMMMGK ]"
                   "[    KMMSSGGGGSSGGGGGGGGGGGKK]"
                   "[    KMSSSSSSSSKWWKWWKWWKGGKK]"
                   "[    KMSSSSSSSKWWKWWKWWKWWGKK]"
                   "[  KKMMSSWWSKKKKKKMMMMMMMMMK ]"
                   "[ KMMMSSWWWKK   K KMKMKMKMK  ]"
                   "[KMSSSSWWWWK K  K  K K K K   ]"
                   "[KMKKKSWWWWK    K            ]"
                   "[KK  KWWWWWKKKKK             ]"
                   "[    KWWWWWK                 ]"
                   "[    KWWWWWK                 ]"
                   "[    KWWWWWK                 ]"
                   "[    KWWYWWK                 ]"
                   "[    KWWWWWK                 ]"
                   "[     KKKKK                  ]";

  V2 Size;
  int IdTex;
  V2 Pos = V2(45, 200);
  Rectangle getRect() {
    return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
  }
};
struct _Chargeur {
  string texture = "[  KK  ]"
                   "[ KYYK ]"
                   "[ KYYK ]"
                   "[ KYYK ]"
                   "[ KYYK ]"
                   "[KYYYYK]"
                   "[KYYYYK]"
                   "[KYYYYK]"
                   "[KYYYYK]"
                   "[KYYYYK]"
                   "[KYYYYK]"
                   "[ KYYK ]"
                   "[KYYYYK]"
                   "[KYYYYK]"
                   "[KKKKKK]";
  V2 Size;
  V2 Pos;
  int IdTex;
  bool exist;
  _Chargeur(V2 _Pos) { Pos = _Pos; }
};

struct _Bullet {
  string textureNorth = "[  KK  ]"
                        "[ KYYK ]"
                        "[ KYYK ]"
                        "[ KYYK ]"
                        "[ KYYK ]"
                        "[KYYYYK]"
                        "[KYYYYK]"
                        "[KYYYYK]"
                        "[KYYYYK]"
                        "[KYYYYK]"
                        "[KYYYYK]"
                        "[ KYYK ]"
                        "[KYYYYK]"
                        "[KYYYYK]"
                        "[KKKKKK]";
  string textureSouth = "[KKKKKK]"
                        "[KYYYYK]"
                        "[KYYYYK]"
                        "[ KYYK ]"
                        "[KYYYYK]"
                        "[KYYYYK]"
                        "[KYYYYK]"
                        "[KYYYYK]"
                        "[KYYYYK]"
                        "[KYYYYK]"
                        "[ KYYK ]"
                        "[ KYYK ]"
                        "[ KYYK ]"
                        "[ KYYK ]"
                        "[  KK  ]";

  string textureEast = "[KK KKKKKKK     ]"
                       "[KYKYYYYYYYKKKK ]"
                       "[KYYYYYYYYYYYYYK]"
                       "[KYYYYYYYYYYYYYK]"
                       "[KYKYYYYYYYKKKK ]"
                       "[KK KKKKKKK     ]";
  string textureWeast = "[     KKKKKKK KK]"
                        "[ KKKKYYYYYYYKYK]"
                        "[KYYYYYYYYYYYYYK]"
                        "[KYYYYYYYYYYYYYK]"
                        "[ KKKKYYYYYYYKYK]"
                        "[     KKKKKKK KK]";
  V2 Size;
  int IdTex;
  V2 Pos;
  string texture;

  bool exist = false;
  void setExist(bool _exist) { exist = _exist; }
  bool getExist() { return exist; }

  void setTexture(string _texture) { texture = _texture; }

  V2 getDirectionBullet() {
    if (texture == textureSouth)
      return V2(0, -2);
    if (texture == textureNorth)
      return V2(0, 2);
    if (texture == textureEast)
      return V2(2, 0);
    if (texture == textureWeast)
      return V2(-2, 0);
    return V2(0, 0);
  }
  void setLastDirectionTexture(_Heros &heros) {
    if (heros.getLastDirection() == 0) {
      setTexture(textureSouth);
    } else if (heros.getLastDirection() == 1) {
      setTexture(textureNorth);
    } else if (heros.getLastDirection() == 2) {
      setTexture(textureEast);
    } else if (heros.getLastDirection() == 3) {
      setTexture(textureWeast);
    }
  }

  void killMomie(_Heros &heros, _Momie &momie) {
    momie.setMorte(true);
    heros.score += SCORE_MOMIE;
  }
  Rectangle getRect() {
    return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
  }
};
struct GameData {
  // indique la pr�sence d'un mur � la case (x,y)
  _NiveauDonjon NiveauDonjon;
  bool Mur(int x, int y) {
    return NiveauDonjon.Map[(15 - y - 1) * 15 + x] == 'M';
  }

  int Lpix = 40; // largeur en pixels des cases du labyrinthe

  _Heros Heros; // data du h�ros
  _Key Key;
  _Chest Chest;
  _Gun Gun;
  _Bullet Bullet;
  _TexturePack TexturePack;
  int difficulty = 0;
  int ecran = 0;

  vector<_Momie> momies = {};
  vector<_Diamond> diamonds = {};

  void setDiamonds() {
    diamonds.clear();
    if (NiveauDonjon.getNiveau() == 1) {
      // diamonds.push_back(_Diamond(V2(530, 367)));
      // diamonds.push_back(_Diamond(V2(48, 535)));
      // diamonds.push_back(_Diamond(V2(253, 290)));
      // diamonds.push_back(_Diamond(V2(492, 212)));
      // diamonds.push_back(_Diamond(V2(334, 52)));
    } else if (NiveauDonjon.getNiveau() == 2) {
      // diamonds.push_back(_Diamond(V2(530, 367)));
      // diamonds.push_back(_Diamond(V2(48, 535)));
      // diamonds.push_back(_Diamond(V2(253, 290)));
      // diamonds.push_back(_Diamond(V2(492, 212)));
      // diamonds.push_back(_Diamond(V2(334, 52)));
    }
    for (_Diamond &diamond : diamonds) {
      diamond.IdTex = G2D::InitTextureFromString(diamond.Size, diamond.texture);
      diamond.Size = diamond.Size * 1.5; // on peut zoomer la taille du sprite
      diamond.exist = true;
    }
  }

  _Life lifes[3] = {_Life(V2(255, 8)), _Life(V2(235, 8)), _Life(V2(215, 8))};
  _Chargeur chargeurs[10] = {_Chargeur(V2(580, 20)), _Chargeur(V2(580, 5)),
                             _Chargeur(V2(570, 20)), _Chargeur(V2(570, 5)),
                             _Chargeur(V2(560, 20)), _Chargeur(V2(560, 5)),
                             _Chargeur(V2(550, 20)), _Chargeur(V2(550, 5)),
                             _Chargeur(V2(540, 20)), _Chargeur(V2(540, 5))};
  vector<_Trap> traps = {};
  void setMomies() {
    momies.clear();
    if (difficulty >= 2) {
      // ? difficile
      momies.push_back(_Momie(529, 445));
      momies.push_back(_Momie(529, 205));
    }
    if (difficulty >= 1) {
      // ? moyen
      momies.push_back(_Momie(43, 525));
      momies.push_back(_Momie(280, 45));
    }
    if (difficulty >= 0) {
      // ? facile
      momies.push_back(_Momie(250, 250));
      momies.push_back(_Momie(130, 420));
      momies.push_back(_Momie(370, 470));
    }
    for (_Momie &momie : momies) {
      momie.IdTex = G2D::InitTextureFromString(momie.Size, momie.texture);
      momie.Size = momie.Size * 2; // on peut zoomer la taille du sprite
      momie.setMorte(false);
    }
  }
  void setTrap() {
    traps.clear();
    if (difficulty >= 2) {
      // ? difficile
      traps.push_back(_Trap(82, 285));
      traps.push_back(_Trap(82 + 9 * 40, 285));
    }
    if (difficulty >= 1) {
      // ? moyen
      traps.push_back(_Trap(122, 405));
      traps.push_back(_Trap(42, 405));
    }
    if (difficulty >= 0) {
      // ? facile
      // ? donjon
      traps.push_back(_Trap(362, 485));
      traps.push_back(_Trap(322, 405));
      traps.push_back(_Trap(362, 405));
      traps.push_back(_Trap(402, 405));
      traps.push_back(_Trap(362, 322));

      traps.push_back(_Trap(402, 122));
      traps.push_back(_Trap(202, 122));
    }
    for (_Trap &trap : traps) {
      trap.IdTex = G2D::InitTextureFromString(trap.Size, trap.texture);
      trap.Size = trap.Size * 2; // on peut zoomer la taille du sprite
    }
  }
  GameData() {}
};

GameData G;

void affichage_ecran_accueil() {
  G2D::DrawStringFontMono(V2(50, 400), "Bienvenue dans le jeu du labyrinthe !",
                          20, 4, Color::White);
  G2D::DrawStringFontMono(V2(80, 300), "Appuyez sur ENTER pour continuer.", 20,
                          3, Color::Green);
}

void affichage_ecran_options() {
  G2D::DrawStringFontMono(V2(100, 500), "Choisissez votre difficulte !", 23, 3,
                          Color::White);
  G2D::DrawStringFontMono(V2(50, 300),
                          "Appuyez sur A pour lancer le mode FACILE", 16, 3,
                          Color::Green);
  G2D::DrawStringFontMono(V2(50, 250),
                          "Appuyez sur B pour lancer le mode MOYEN", 16, 3,
                          Color::Yellow);
  G2D::DrawStringFontMono(V2(50, 200),
                          "Appuyez sur C pour lancer le mode DIFFICILE", 16, 3,
                          Color::Red);

  G2D::DrawStringFontMono(
      V2(10, 100), "Pour tirer avec le pistolet, appuyer sur la touche b", 16,
      3, Color::Cyan);
}

void affichage_init_partie() {
  G2D::DrawStringFontMono(V2(220, 300), "Chargement...", 20, 3, Color::White);
  G2D::DrawStringFontMono(V2(220, 100), "Appuyez sur ENTER", 16, 3,
                          Color::Cyan);
  G2D::DrawCircle(V2(150, 250), 50, Color::Green);
  G2D::DrawCircle(V2(450, 450), 30, Color::Cyan);
  G2D::DrawCircle(V2(250, 550), 60, Color::Blue);
  G2D::DrawCircle(V2(500, 50), 30, Color::Magenta);
}

void affichage_ecran_jeu() {
  for (int x = 0; x < 15; x++)
    for (int y = 0; y < 15; y++) {
      int xx = x * G.Lpix - 6;
      int yy = y * G.Lpix;
      if (G.Mur(x, y))
        G2D::DrawRectWithTexture(G.TexturePack.IdTexMur, V2(xx, yy),
                                 G.TexturePack.Size);
      else {
        G2D::DrawRectWithTexture(G.TexturePack.IdTexSol, V2(xx, yy),
                                 G.TexturePack.Size);
      }
    }

  // affichage de la clef
  if (!G.Heros.getHasKey()) {
    G2D::DrawRectWithTexture(G.Key.IdTex, G.Key.Pos, G.Key.Size);
  }
  // affichage gun
  if (!G.Heros.hasGun) {
    G2D::DrawRectWithTexture(G.Gun.IdTex, G.Gun.Pos, G.Gun.Size);
  }
  // affichage du Chest
  G2D::DrawRectWithTexture(G.Chest.IdTex, G.Chest.Pos, G.Chest.Size);

  // affichage des diamants
  for (_Diamond &diamond : G.diamonds) {
    if (diamond.exist) {
      G2D::DrawRectWithTexture(diamond.IdTex, diamond.Pos, diamond.Size);
    }
  }
  for (_Life &life : G.lifes) {
    G2D::DrawRectWithTexture(life.IdTex, life.Pos, life.Size);
  }
  for (_Chargeur &chargeur : G.chargeurs) {
    if (chargeur.exist) {
      G2D::DrawRectWithTexture(chargeur.IdTex, chargeur.Pos, chargeur.Size);
    }
  }
  // affichage des traps
  for (_Trap &trap : G.traps) {
    trap.IdTex = G2D::InitTextureFromString(trap.Size, trap.texture);
    trap.Size = trap.Size * 1; // on peut zoomer la taille du sprite
    G2D::DrawRectWithTexture(trap.IdTex, trap.Pos, trap.Size);
  }
  // affichage d'une Momie
  for (_Momie &momie : G.momies) {
    if (!momie.getMorte()) {
      G2D::DrawRectWithTexture(momie.IdTex, momie.Pos, momie.Size);
    }
  }

  // affichage du héros avec boite englobante et zoom x 2
  G2D::DrawRectangle(G.Heros.Pos, G.Heros.Size, Color::Red);
  G2D::DrawRectWithTexture(G.Heros.IdTex, G.Heros.Pos, G.Heros.Size);

  // affichage bullet
  if (G.Bullet.getExist()) {
    G2D::DrawRectWithTexture(G.Bullet.IdTex, G.Bullet.Pos, G.Bullet.Size);
  }

  string vies = "Nombre de vies : ";
  G2D::DrawStringFontMono(V2(10, 12), vies, 18, 3, Color::Black);

  string score = "Score actuel :" + std::to_string(G.Heros.score);
  G2D::DrawStringFontMono(V2(150, 570), score, 25, 3, Color::Black);

  string balles = "Nombre de balles :";
  G2D::DrawStringFontMono(V2(300, 12), balles, 18, 3, Color::Black);
}

void affichage_ecran_game_over() {
  G2D::DrawStringFontMono(V2(70, 500), "Game over", 80, 10, Color::Red);

  string score = "Score : " + std::to_string(G.Heros.score);
  G2D::DrawStringFontMono(V2(70, 300), score, 20, 3, Color::Yellow);

  G2D::DrawStringFontMono(V2(50, 200),
                          "Appuyez sur ENTER pour faire une autre partie.", 16,
                          3, Color::Green);
}
void affichage_ecran_win() {
  G2D::DrawStringFontMono(V2(70, 500), "You WIN !!!!", 80, 10, Color::Green);
  G2D::DrawStringFontMono(V2(50, 200),
                          "Appuyez sur ENTER pour faire une autre partie.", 16,
                          3, Color::White);
  string score = "Score : " + std::to_string(G.Heros.score);
  G2D::DrawStringFontMono(V2(70, 300), score, 20, 3, Color::Yellow);
}
void render() {
  G2D::ClearScreen(Color::Black);
  if (G.ecran == ECRAN_ACCUEIL) {
    affichage_ecran_accueil();
  }
  if (G.ecran == ECRAN_OPTIONS) {
    affichage_ecran_options();
  }
  if (G.ecran == INIT_PARTIE) {
    affichage_init_partie();
  }
  if (G.ecran == ECRAN_JEU) {
    affichage_ecran_jeu();
  }
  if (G.ecran == ECRAN_GAME_OVER) {
    affichage_ecran_game_over();
  }
  if (G.ecran == ECRAN_WIN) {
    affichage_ecran_win();
  }
  G2D::Show();
}

bool getTapeUnMur(V2 newPos, V2 Size) {
  return (G.Mur(newPos.x / 40, newPos.y / 40)) ||
         (G.Mur((newPos.x + Size.x) / 40, (newPos.y + Size.y) / 40)) ||
         (G.Mur((newPos.x) / 40, (newPos.y + Size.y) / 40)) ||
         (G.Mur((newPos.x + Size.x) / 40, (newPos.y) / 40));
}
/**
 * Collision bullet/momie-mur
 */
void collision(_Bullet &bullet) {
  Rectangle rectBullet = bullet.getRect();
  for (_Momie &momie : G.momies) {
    if (!momie.getMorte()) {
      if (InterRectRect(rectBullet, momie.getRect())) {
        G.Bullet.setExist(false);
        G.Bullet.killMomie(G.Heros, momie);
        return;
      }
    }
  }
  V2 newPos = G.Bullet.Pos + G.Bullet.getDirectionBullet();
  if (!getTapeUnMur(newPos, G.Bullet.Size)) {
    G.Bullet.Pos = newPos;
  } else {
    G.Bullet.setExist(false);
  }
}
/**
 * Collision héros/autre
 */
void collision(_Heros &heros) {
  Rectangle rectHero = heros.getRect();
  Rectangle rectChest = G.Chest.getRect();
  Rectangle rectKey = G.Key.getRect();

  bool collisionGun = InterRectRect(heros.getRect(), G.Gun.getRect());
  if (collisionGun) {
    if (!heros.hasGun) {
      heros.hasGun = true;
    }
  }
  // ? héros/clé
  bool collisionKey = InterRectRect(rectHero, rectKey);
  if (collisionKey) {
    heros.setHasKey(true);
  }

  // ? héros/coffre
  bool collisionChest = InterRectRect(rectHero, rectChest);
  if (collisionChest) {
    if (heros.hasKey) {
      G.Chest.isOpened = true;
    }
  }
  // ? héros / trap
  for (_Trap &trap : G.traps) {
    if (trap.active == 2) {
      bool collisionTrap = InterRectRect(rectHero, trap.getRect());
      if (collisionTrap) {
        heros.nbVies--;
        heros.Pos = V2(45, 45);
        G.setMomies();
        heros.nbBullets = 10;
        for (_Life &life : G.lifes) {
          if (life.exist) {
            life.exist = false;
            life.setTexture(life.mortTexture);
            life.IdTex = G2D::InitTextureFromString(life.Size, life.texture);
            life.Size = life.Size * 1.5; // on peut zoomer la taille du sprite
            break;
          }
        }
        for (_Chargeur &chargeur : G.chargeurs) {
          chargeur.exist = true;
        }
      }
    }
  }
  // ? héros/diamond
  for (_Diamond &diamond : G.diamonds) {
    bool collisionDiamond = InterRectRect(rectHero, diamond.getRect());
    if (collisionDiamond) {
      if (diamond.exist) {
        heros.score += SCORE_DIAMOND;
        diamond.exist = false;
      }
    }
  }

  // ? héros/momie
  for (_Momie &momie : G.momies) {
    if (!momie.getMorte()) {
      bool collisionMomie = InterRectRect(rectHero, momie.getRect());
      if (collisionMomie) {
        G.setMomies();
        heros.nbVies--;
        heros.Pos = V2(45, 45);
        heros.nbBullets = 10;
        for (_Life &life : G.lifes) {
          if (life.exist) {
            life.exist = false;
            life.setTexture(life.mortTexture);
            life.IdTex = G2D::InitTextureFromString(life.Size, life.texture);
            life.Size = life.Size * 1.5; // on peut zoomer la taille du sprite
            break;
          }
        }
        for (_Chargeur &chargeur : G.chargeurs) {
          chargeur.exist = true;
        }
      }
    }
  }
  // ? héros/mur
  if (getTapeUnMur(heros.Pos, heros.Size)) {
    if (G2D::IsKeyPressed(Key::LEFT))
      heros.Pos.x++;
    if (G2D::IsKeyPressed(Key::RIGHT))
      heros.Pos.x--;
    if (G2D::IsKeyPressed(Key::UP))
      heros.Pos.y--;
    if (G2D::IsKeyPressed(Key::DOWN))
      heros.Pos.y++;
  }
};

/**
 * Collision momie/autre
 */
bool InterMomieMur(_Momie momie, V2 newPos) {
  return getTapeUnMur(newPos, momie.Size);
}
bool InterMomieMomie(_Momie &momie) {
  bool conditionMomie = false;
  for (_Momie m : G.momies) {
    if (!m.getMorte()) {
      if (!momie.isMomie(m)) {
        if (momie.getTapeMomie(m)) {
          conditionMomie = true;
        }
      }
    }
  }
  return conditionMomie;
}
/**
 * Collision Momie/mur,momie
 */
void collision(_Momie &momie) {
  V2 Dir[4] = {V2(0, 1), V2(1, 0), V2(0, -1), V2(-1, 0)};
  V2 newPos = momie.Pos + momie.Dir;

  if (InterMomieMomie(momie)) {
    momie.Dir = -momie.Dir;
    momie.Pos = momie.Pos + momie.Dir;
    momie.changeCompteur = 50;
  } else if (!InterMomieMur(momie, newPos)) {
    // pour avoir un meilleur déplacement des momies, on change leur direction
    // dès que leur compteur atteint 0
    if (momie.changeCompteur == 0) {
      int rd = rand() % 4;
      while (-Dir[rd] == momie.Dir) {
        rd = rand() % 4;
      }
      momie.Dir = Dir[rd];
      momie.changeCompteur = 50;
    }
    momie.changeCompteur--;
    momie.Pos = newPos;
  } else {
    int rd = rand() % 4;
    while (-Dir[rd] == momie.Dir) {
      rd = rand() % 4;
    }
    momie.Dir = Dir[rd];
    momie.changeCompteur = 50;
  }
}

int gestion_ecran_accueil() {
  if (G2D::IsKeyPressed(Key::ENTER)) {
    return 1;
  }
  return 0;
}
int gestion_ecran_options() {
  // * facile
  if (G2D::IsKeyPressed(Key::A)) {
    G.difficulty = 0;
    return 2;
  }
  // * moyen
  if (G2D::IsKeyPressed(Key::B)) {
    G.difficulty = 1;
    return 2;
  }
  // * difficile
  if (G2D::IsKeyPressed(Key::C)) {
    G.difficulty = 2;
    return 2;
  }
  return 1;
}

int InitPartie() {
  G.Heros.reset();
  G.Chest.isOpened = false;
  if (G.NiveauDonjon.getNiveau() == 1) {
    G.Heros.nbBullets = 10;
    G.Heros.score = 0;
    G.Heros.nbVies = 3;
  }
  if (G2D::IsKeyPressed(Key::ENTER)) {
    G.setMomies();
    G.setDiamonds();
    for (_Life &life : G.lifes) {
      if (G.NiveauDonjon.getNiveau() == 1) {
        life.IdTex = G2D::InitTextureFromString(life.Size, life.vivantTexture);
        life.Size = life.Size * 1.5; // on peut zoomer la taille du sprite
        life.exist = true;
      }
    }
    for (_Chargeur &chargeur : G.chargeurs) {
      if (G.NiveauDonjon.getNiveau() == 1) {
        chargeur.IdTex =
            G2D::InitTextureFromString(chargeur.Size, chargeur.texture);
        chargeur.Size =
            chargeur.Size * 0.8; // on peut zoomer la taille du sprite
        chargeur.exist = true;
      }
    }
    G.setTrap();
    for (_Trap &trap : G.traps) {
      trap.IdTex = G2D::InitTextureFromString(trap.Size, trap.texture);
      trap.Size = trap.Size * 2; // on peut zoomer la taille du sprite
    }
    return 3;
  }
  return 2;
}
int gestion_ecran_jeu() {
  // ? Déplacement héros
  if (G2D::IsKeyPressed(Key::LEFT)) {
    G.Heros.Pos.x--;
    G.Heros.changeTexture();
    G.Heros.setLastDirection(3);
  }

  if (G2D::IsKeyPressed(Key::RIGHT)) {
    G.Heros.Pos.x++;
    G.Heros.changeTexture();
    G.Heros.setLastDirection(2);
  }
  if (G2D::IsKeyPressed(Key::UP)) {
    G.Heros.Pos.y++;
    G.Heros.changeTexture();
    G.Heros.setLastDirection(1);
  }
  if (G2D::IsKeyPressed(Key::DOWN)) {
    G.Heros.Pos.y--;
    G.Heros.changeTexture();
    G.Heros.setLastDirection(0);
  }
  // ? tirer une balle
  if (G2D::IsKeyPressed(Key::B)) {
    if (G.Heros.hasGun && G.Heros.nbBullets > 0 && !G.Bullet.exist) {
      G.Bullet.setExist(true);
      G.Bullet.Pos = G.Heros.Pos;
      G.Bullet.setLastDirectionTexture(G.Heros);
      G.Bullet.IdTex =
          G2D::InitTextureFromString(G.Bullet.Size, G.Bullet.texture);
      G.Bullet.Size = G.Bullet.Size * 0.8; // on peut zoomer la taille du sprite
      G.Heros.nbBullets--;
      for (_Chargeur &chargeur : G.chargeurs) {
        if (chargeur.exist) {
          chargeur.exist = false;
          break;
        }
      }
    }
  }
  if (G.Bullet.getExist()) {
    collision(G.Bullet);
  }
  // ? traps
  for (_Trap &trap : G.traps) {
    // ? change la texture et le isActive si compteur == 0;
    if (trap.compteur == 0) {
      trap.compteur = rand() % COMPTEUR_TRAP + MIN_TRAP;
      if (trap.active == 0) {
        trap.compteur = MIN_TRAP;
        trap.setTexture(trap.willBeActiveTexture);
        trap.active = 1;
      } else if (trap.active == 1) {
        trap.setTexture(trap.activeTexture);
        trap.active = 2;
      } else {
        trap.setTexture(trap.inactiveTexture);
        trap.active = 0;
      }
    } else {
      trap.compteur--;
    }
  }
  // ? Collisions
  collision(G.Heros);
  for (_Momie &momie : G.momies) {
    collision(momie);
  }

  if (G.Chest.isOpened) {
    if (G.NiveauDonjon.getNiveau() == 2) {
      return 5;
    } else if (G.NiveauDonjon.getNiveau() == 1) {
      G.NiveauDonjon.setNiveau(2);
      G.NiveauDonjon.setTexture();
      return 2;
    }
  }
  if (G.Heros.nbVies <= 0) {
    return 4;
  }
  return 3;
}
int gestion_ecran_game_over() {
  if (G2D::IsKeyPressed(Key::ENTER)) {
    return 1;
  }
  return 4;
}
int gestion_ecran_win() {
  if (G2D::IsKeyPressed(Key::ENTER)) {
    return 1;
  }
  return 5;
}
void Logic() {
  if (G.ecran == ECRAN_ACCUEIL) {
    G.ecran = gestion_ecran_accueil();
  }

  if (G.ecran == ECRAN_OPTIONS) {
    G.ecran = gestion_ecran_options();
  }
  if (G.ecran == INIT_PARTIE) {
    G.ecran = InitPartie();
  }
  if (G.ecran == ECRAN_JEU) {
    G.ecran = gestion_ecran_jeu();
  }

  if (G.ecran == ECRAN_GAME_OVER) {
    G.ecran = gestion_ecran_game_over();
  }
  if (G.ecran == ECRAN_WIN) {
    G.ecran = gestion_ecran_win();
  }
}

void AssetsInit() {
  // Size passé en ref et texture en param
  G.NiveauDonjon.setNiveau(1);
  G.NiveauDonjon.setTexture();

  G.TexturePack.IdTexMur =
      G2D::InitTextureFromString(G.TexturePack.Size, G.TexturePack.textureMur);
  G.TexturePack.IdTexSol =
      G2D::InitTextureFromString(G.TexturePack.Size, G.TexturePack.textureSol);
  G.TexturePack.Size =
      G.TexturePack.Size * 5; // on peut zoomer la taille du sprite

  G.Heros.IdTex = G2D::InitTextureFromString(G.Heros.Size, G.Heros.texture);
  G.Heros.Size = G.Heros.Size * 2; // on peut zoomer la taille du sprite

  G.Key.IdTex = G2D::InitTextureFromString(G.Key.Size, G.Key.texture);
  G.Key.Size = G.Key.Size * 1.5; // on peut zoomer la taille du sprite

  G.Chest.IdTex = G2D::InitTextureFromString(G.Chest.Size, G.Chest.texture);
  G.Chest.Size = G.Chest.Size * 2.5; // on peut zoomer la taille du sprite

  G.Gun.IdTex = G2D::InitTextureFromString(G.Gun.Size, G.Gun.texture);
  G.Gun.Size = G.Gun.Size * 1; // on peut zoomer la taille du sprite
}
int main(int argc, char *argv[]) {
  G2D::InitWindow(argc, argv, V2(G.Lpix * 15, G.Lpix * 15), V2(200, 200),
                  string("Labyrinthe"));

  AssetsInit();

  G2D::Run(Logic, render);
}