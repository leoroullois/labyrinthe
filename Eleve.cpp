
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

struct Rectangle {
  int xMin, xMax, yMin, yMax;
  Rectangle(int _xMin, int _yMin, int _xMax, int _yMax) {
    xMin = _xMin, xMax = _xMax, yMin = _yMin, yMax = _yMax;
  }

  V2 getCoordonneeMin() { return V2(xMin, yMin); }
  V2 getCoordonneeMax() { return V2(xMax, yMax); }
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

  // ? true si le héros possède une clé
  bool hasKey = false;
  bool hasGun = false;
  int nbBalles = 0;
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

  Rectangle getRect() {
    return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
  }
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
  V2 Pos = V2(440, 450);
  Rectangle getRect() {
    return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
  }
};

struct _Chest {
  string texture = "[   WWWWWWWWWWWWWW   ]"
                   "[ WGGGGWWWWWWWWWWWWW ]"
                   "[WG   GWW    WWW   WW]"
                   "[WGRRGGW  YY  WWWRRWW]"
                   "[WWRRWWWW    WWWWRRWW]"
                   "[ WWWWWWWWWWWWWWWWWW ]";

  V2 Size;
  int IdTex;
  V2 Pos = V2(405, 50);

  bool isOpened = false;
  Rectangle getRect() {
    return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
  }
};

struct _Gun {
  string texture = "[   O                        ]"
                   "[  OGO                    OGO]"
                   "[  OGGOOOOOOOOOOOOOOOOOOOOGGO]"
                   "[   OGGMMGGGGGGMMMMMMMMMMMGO ]"
                   "[    OMMSSGGGGSSGGGGGGGGGGGOO]"
                   "[    OMSSSSSSSSOWWOWWOWWOGGOO]"
                   "[    OMSSSSSSSOWWOWWOWWOWWGOO]"
                   "[  OOMMSSWWSOOOOOOMMMMMMMMMO ]"
                   "[ OMMMSSWWWOO   O OMOMOMOMO  ]"
                   "[OMSSSSWWWWO O  O  O O O O   ]"
                   "[OMOOOSWWWWO    O            ]"
                   "[OO  OWWWWWOOOOO             ]"
                   "[    OWWWWWO                 ]"
                   "[    OWWWWWO                 ]"
                   "[    OWWWWWO                 ]"
                   "[    OWWYWWO                 ]"
                   "[    OWWWWWO                 ]"
                   "[     OOOOO                  ]";

  V2 Size;
  int IdTex;
  V2 Pos = V2(325, 200);
  Rectangle getRect() {
    return Rectangle(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
  }
};
struct GameData {

  string Map = "MMMMMMMMMMMMMMM"
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

  // indique la pr�sence d'un mur � la case (x,y)
  bool Mur(int x, int y) { return Map[(15 - y - 1) * 15 + x] == 'M'; }

  int Lpix = 40; // largeur en pixels des cases du labyrinthe

  _Heros Heros; // data du h�ros
  _Key Key;
  _Chest Chest;
  _Gun Gun;
  int difficulty = 0;
  int ecran = 0;

  vector<_Momie> momies = {};

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
}

void affichage_init_partie() {
  G2D::DrawStringFontMono(V2(220, 300), "Chargement...", 20, 3, Color::White);
  G2D::DrawCircle(V2(150, 250), 50, Color::Green);
  G2D::DrawCircle(V2(450, 450), 30, Color::Cyan);
  G2D::DrawCircle(V2(250, 550), 60, Color::Blue);
  G2D::DrawCircle(V2(500, 50), 30, Color::Magenta);
}

void affichage_ecran_jeu() {
  for (int x = 0; x < 15; x++)
    for (int y = 0; y < 15; y++) {
      int xx = x * G.Lpix;
      int yy = y * G.Lpix;
      if (G.Mur(x, y))
        G2D::DrawRectangle(V2(xx, yy), V2(G.Lpix, G.Lpix), Color::Blue, true);
    }

  // affichage du héros avec boite englobante et zoom x 2
  G2D::DrawRectangle(G.Heros.Pos, G.Heros.Size, Color::Red);
  G2D::DrawRectWithTexture(G.Heros.IdTex, G.Heros.Pos, G.Heros.Size);

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

  // affichage d'une Momie
  for (_Momie &momie : G.momies) {
    G2D::DrawRectWithTexture(momie.IdTex, momie.Pos, momie.Size);
  }
  G2D::DrawStringFontMono(V2(100, 580), "Partie en cours", 20, 3, Color::Green);

  string vies = "Nombre de vies : " + std::to_string(G.Heros.nbVies);
  G2D::DrawStringFontMono(V2(100, 20), vies, 20, 3, Color::Green);
}

void affichage_ecran_game_over() {
  G2D::DrawStringFontMono(V2(70, 500), "Game over", 80, 10, Color::Red);
  G2D::DrawStringFontMono(V2(50, 300),
                          "Appuyez sur ENTER pour faire une autre partie.", 16,
                          3, Color::Green);
}
void affichage_ecran_win() {
  G2D::DrawStringFontMono(V2(70, 500), "You WIN !!!!", 80, 10, Color::Green);
  G2D::DrawStringFontMono(V2(50, 300),
                          "Appuyez sur ENTER pour faire une autre partie.", 16,
                          3, Color::White);
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
 * Collision héros/autre
 */
void collision(_Heros &heros) {
  Rectangle rectHero = G.Heros.getRect();
  Rectangle rectChest = G.Chest.getRect();
  Rectangle rectKey = G.Key.getRect();

  bool collisionGun = InterRectRect(G.Heros.getRect(), G.Gun.getRect());
  if (collisionGun) {
    if (!heros.hasGun) {
      cout << "You got the gun" << endl;
      G.Heros.hasGun = true;
      G.Gun.Pos = V2(-100, -100);
    }
  }
  // ? héros/clé
  bool collisionKey = InterRectRect(rectHero, rectKey);
  if (collisionKey) {
    cout << "You got the key" << endl;
    G.Heros.setHasKey(true);
    G.Key.Pos = V2(-100, -100);
  }

  // ? héros/coffre
  bool collisionChest = InterRectRect(rectHero, rectChest);
  if (collisionChest) {
    if (G.Heros.hasKey) {
      cout << "You win !" << endl;
      G.Chest.isOpened = true;
    }
  }

  // ? héros/momie
  for (_Momie &momie : G.momies) {
    bool collisionMomie = InterRectRect(rectHero, momie.getRect());
    if (collisionMomie) {
      cout << "You lose !" << endl;
      G.Heros.nbVies--;
      G.Heros.Pos = V2(45, 45);
    }
  }
  // ? héros/mur

  if (getTapeUnMur(G.Heros.Pos, G.Heros.Size)) {
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
    if (!momie.isMomie(m)) {
      if (momie.getTapeMomie(m)) {
        conditionMomie = true;
      }
    }
  }
  return conditionMomie;
}
void collision(_Momie &momie) {
  V2 Dir[4] = {V2(0, 1), V2(1, 0), V2(0, -1), V2(-1, 0)};
  V2 newPos = momie.Pos + momie.Dir;

  if (InterMomieMomie(momie)) {
    std::cout << "collision momie" << std::endl;
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
  G.Heros.hasKey = false;
  G.Heros.hasGun = false;
  G.Heros.hasGun = false;
  G.Heros.nbBalles = 0;
  G.Heros.nbVies = 3;
  G.Heros.Pos = V2(45, 45);

  G.Key.Pos = V2(440, 450);
  G.Chest.isOpened = false;

  // G.momies.clear();
  if (G2D::IsKeyPressed(Key::ENTER)) {
    G.momies.clear();
    if (G.difficulty >= 2) {
      // ? difficile
      G.momies.push_back(_Momie(529, 380));
      G.momies.push_back(_Momie(485, 205));
    }
    if (G.difficulty >= 1) {
      // ? moyen
      G.momies.push_back(_Momie(43, 525));
      G.momies.push_back(_Momie(316, 45));
    }
    if (G.difficulty >= 0) {
      // ? facile
      G.momies.push_back(_Momie(250, 250));
      G.momies.push_back(_Momie(130, 420));
      G.momies.push_back(_Momie(370, 470));
    }
    for (_Momie &momie : G.momies) {
      momie.IdTex = G2D::InitTextureFromString(momie.Size, momie.texture);
      momie.Size = momie.Size * 2; // on peut zoomer la taille du sprite
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
  }

  if (G2D::IsKeyPressed(Key::RIGHT)) {
    G.Heros.Pos.x++;
    G.Heros.changeTexture();
  }
  if (G2D::IsKeyPressed(Key::UP)) {
    G.Heros.Pos.y++;
    G.Heros.changeTexture();
  }
  if (G2D::IsKeyPressed(Key::DOWN)) {
    G.Heros.Pos.y--;
    G.Heros.changeTexture();
  }

  // ? Collisions
  collision(G.Heros);
  for (_Momie &momie : G.momies) {
    collision(momie);
  }

  if (G.Chest.isOpened) {
    return 5;
  }
  if (G.Heros.nbVies == 0) {
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
  G.Heros.IdTex = G2D::InitTextureFromString(G.Heros.Size, G.Heros.texture);
  G.Heros.Size = G.Heros.Size * 2; // on peut zoomer la taille du sprite

  G.Key.IdTex = G2D::InitTextureFromString(G.Key.Size, G.Key.texture);
  G.Key.Size = G.Key.Size * 1.5; // on peut zoomer la taille du sprite

  G.Chest.IdTex = G2D::InitTextureFromString(G.Chest.Size, G.Chest.texture);
  G.Chest.Size = G.Chest.Size * 2.5; // on peut zoomer la taille du sprite

  G.Gun.IdTex = G2D::InitTextureFromString(G.Gun.Size, G.Gun.texture);
  G.Gun.Size = G.Gun.Size * 0.8; // on peut zoomer la taille du sprite
}

int main(int argc, char *argv[]) {
  G2D::InitWindow(argc, argv, V2(G.Lpix * 15, G.Lpix * 15), V2(200, 200),
                  string("Labyrinthe"));

  AssetsInit();

  G2D::Run(Logic, render);
}
