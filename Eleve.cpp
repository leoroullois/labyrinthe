
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

  // ? true si le héros possède une clé
  bool hasKey = false;
  bool hasPistolet = false;
  int nbBalles = 0;

  bool getHasKey() { return hasKey; }
  void setHasKey(bool pKey) { hasKey = pKey; }
  int nbVies = 3;

  V2 Size;
  int IdTex;
  int IdTex2;
  V2 Pos = V2(45, 45);

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
  _Momie(int x, int y) { Pos = V2(x, y); }

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
  int difficulty = 0;
  int ecran = 0;
  _Momie momiesFacile[3] = {_Momie(250, 260), _Momie(130, 420),
                            _Momie(370, 470)};
  _Momie momiesMoyen[3] = {_Momie(250, 260), _Momie(130, 420),
                           _Momie(370, 470)};
  _Momie momiesDifficile[3] = {_Momie(250, 260), _Momie(130, 420),
                               _Momie(370, 470)};

  _Momie momies[3] = {_Momie(250, 260), _Momie(130, 420), _Momie(370, 470)};

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

  // affichage du Chest
  G2D::DrawRectWithTexture(G.Chest.IdTex, G.Chest.Pos, G.Chest.Size);

  // affichage d'une Momie
  for (int i = 0; i < 3; i++) {
    G2D::DrawRectWithTexture(G.momies[i].IdTex, G.momies[i].Pos,
                             G.momies[i].Size);
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
  for (int i = 0; i < 3; i++) {
    bool collisionMomie = InterRectRect(rectHero, G.momies[i].getRect());
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
bool positionValide(_Momie momie, V2 newPos) {
  return !getTapeUnMur(newPos, momie.Size);
}
void collision(_Momie &momie) {
  V2 newPos = momie.Pos + momie.Dir;
  if (positionValide(momie, newPos)) {

    momie.Pos = newPos;
  } else {
    int rd = rand() % 4;

    V2 Dir[4] = {V2(0, 1), V2(1, 0), V2(0, -1), V2(-1, 0)};

    momie.Dir = Dir[rd];
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
  G.Heros.nbVies = 3;
  G.Heros.hasPistolet = false;
  G.Heros.nbBalles = 0;
  G.Heros.Pos = V2(45, 45);

  G.Key.Pos = V2(440, 450);
  G.Chest.isOpened = false;

  if (G2D::IsKeyPressed(Key::ENTER)) {
    return 3;
  }
  return 2;
}
int gestion_ecran_jeu() {
  // ? Déplacement héros
  if (G2D::IsKeyPressed(Key::LEFT))
    G.Heros.Pos.x--;
  if (G2D::IsKeyPressed(Key::RIGHT))
    G.Heros.Pos.x++;
  if (G2D::IsKeyPressed(Key::UP))
    G.Heros.Pos.y++;
  if (G2D::IsKeyPressed(Key::DOWN))
    G.Heros.Pos.y--;

  // ? Collisions
  collision(G.Heros);
  for (int i = 0; i < 3; i++) {
    collision(G.momies[i]);
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
  // Size pass� en ref et texture en param
  G.Heros.IdTex = G2D::InitTextureFromString(G.Heros.Size, G.Heros.texture);
  G.Heros.Size = G.Heros.Size * 2; // on peut zoomer la taille du sprite

  G.Key.IdTex = G2D::InitTextureFromString(G.Key.Size, G.Key.texture);
  G.Key.Size = G.Key.Size * 1.5; // on peut zoomer la taille du sprite

  G.Chest.IdTex = G2D::InitTextureFromString(G.Chest.Size, G.Chest.texture);
  G.Chest.Size = G.Chest.Size * 2.5; // on peut zoomer la taille du sprite

  for (int i = 0; i < 3; i++) {
    G.momies[i].IdTex =
        G2D::InitTextureFromString(G.momies[i].Size, G.momies[i].texture);
    G.momies[i].Size =
        G.momies[i].Size * 2; // on peut zoomer la taille du sprite
  }
}

int main(int argc, char *argv[]) {
  Rectangle vRect1 = Rectangle(1, 5, 8, 10);
  Rectangle vRect2 = Rectangle(2, 6, 7, 9);
  Rectangle vRect3 = Rectangle(11, 50, 80, 100);

  G2D::InitWindow(argc, argv, V2(G.Lpix * 15, G.Lpix * 15), V2(200, 200),
                  string("Labyrinthe"));

  AssetsInit();

  G2D::Run(Logic, render);
}
