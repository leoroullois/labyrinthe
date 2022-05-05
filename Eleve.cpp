
#pragma warning(disable : 4996)

#include "G2D.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
  bool nbBalles = 0;

  V2 Size;
  int IdTex;
  int IdTex2;
  V2 Pos = V2(45, 45);
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
  //  V2 Pos = V2(230, 250);
  V2 Pos;
  V2 Dir = V2(1, 0);
  _Momie(int x, int y) { Pos = V2(x, y); }
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

  _Momie momies[3] = {_Momie(250, 260), _Momie(130, 420), _Momie(370, 470)};
  GameData() {}
};

GameData G;

void render() {
  G2D::ClearScreen(Color::Black);

  for (int x = 0; x < 15; x++)
    for (int y = 0; y < 15; y++) {
      int xx = x * G.Lpix;
      int yy = y * G.Lpix;
      if (G.Mur(x, y))
        G2D::DrawRectangle(V2(xx, yy), V2(G.Lpix, G.Lpix), Color::Blue, true);
    }

  // affichage du h�ro avec boite englobante et zoom x 2
  G2D::DrawRectangle(G.Heros.Pos, G.Heros.Size, Color::Red);
  G2D::DrawRectWithTexture(G.Heros.IdTex, G.Heros.Pos, G.Heros.Size);

  // affichage de la clef
  G2D::DrawRectWithTexture(G.Key.IdTex, G.Key.Pos, G.Key.Size);

  // affichage du Chest
  G2D::DrawRectWithTexture(G.Chest.IdTex, G.Chest.Pos, G.Chest.Size);

  // affichage d'une Momie
  for (int i = 0; i < 3; i++) {
    G2D::DrawRectWithTexture(G.momies[i].IdTex, G.momies[i].Pos,
                             G.momies[i].Size);
  }

  G2D::Show();
}

/**
 * Collision héros/clé-coffre
 */
void collision(_Heros &heros, Rectangle rect) {
  Rectangle rectHero =
      Rectangle(heros.Pos.x, heros.Pos.y, heros.Pos.x + heros.Size.x,
                heros.Pos.y + heros.Size.y);

  if (InterRectRect(rectHero, rect)) {
    if (G2D::IsKeyPressed(Key::LEFT))
      heros.Pos.x++;
    if (G2D::IsKeyPressed(Key::RIGHT))
      heros.Pos.x--;
    if (G2D::IsKeyPressed(Key::UP))
      heros.Pos.y--;
    if (G2D::IsKeyPressed(Key::DOWN))
      heros.Pos.y++;
  }
}

/**
 * Collision héros/autre
 */
void collision(_Heros &heros) {
  // ? héros/mur
  bool tapeUnCoin =
      (G.Mur(heros.Pos.x / 40, heros.Pos.y / 40)) ||
      (G.Mur((heros.Pos.x + heros.Size.x) / 40,
             (heros.Pos.y + heros.Size.y) / 40)) ||
      (G.Mur((heros.Pos.x) / 40, (heros.Pos.y + heros.Size.y) / 40)) ||
      (G.Mur((heros.Pos.x + heros.Size.x) / 40, (heros.Pos.y) / 40));

  if (tapeUnCoin) {
    if (G2D::IsKeyPressed(Key::LEFT))
      heros.Pos.x++;
    if (G2D::IsKeyPressed(Key::RIGHT))
      heros.Pos.x--;
    if (G2D::IsKeyPressed(Key::UP))
      heros.Pos.y--;
    if (G2D::IsKeyPressed(Key::DOWN))
      heros.Pos.y++;
  }

  // ? héros/clé
  if (G.Heros.hasKey == false) {
    Rectangle rectKey =
        Rectangle(G.Key.Pos.x, G.Key.Pos.y, G.Key.Pos.x + G.Key.Size.x,
                  G.Key.Pos.y + G.Key.Size.y);
    collision(G.Heros, rectKey);
  }

  // ? héros/coffre
  Rectangle rectChest =
      Rectangle(G.Chest.Pos.x, G.Chest.Pos.y, G.Chest.Pos.x + G.Chest.Size.x,
                G.Chest.Pos.y + G.Chest.Size.y);
  collision(G.Heros, rectChest);

  // ? héros/momies
  for (int i = 0; i < 3; i++) {
    Rectangle rectMomie = Rectangle(G.momies[i].Pos.x, G.momies[i].Pos.y,
                                    G.momies[i].Pos.x + G.momies[i].Size.x,
                                    G.momies[i].Pos.y + G.momies[i].Size.y);
    collision(G.Heros, rectMomie);
  }
};

/**
 * Collision momie/autre
 */
bool positionValide(_Momie momie, V2 newPos) {
  bool tapeUnCoin =
      (G.Mur(newPos.x / 40, newPos.y / 40)) ||
      (G.Mur((newPos.x + momie.Size.x) / 40, (newPos.y + momie.Size.y) / 40)) ||
      (G.Mur((newPos.x) / 40, (newPos.y + momie.Size.y) / 40)) ||
      (G.Mur((newPos.x + momie.Size.x) / 40, (newPos.y) / 40));
  if (tapeUnCoin) {
    return false;
  }
  bool tapeUnMur = G.Mur(newPos.x / 40, newPos.y / 40);
  if (tapeUnMur) {
    return false;
  }
  return true;
}

void collision(_Momie &momie) {
  // calcul de la nouvelle position
  V2 newPos = momie.Pos + momie.Dir;

  // test pour savoir si on a une collision avec un mur
  if (positionValide(momie, newPos)) {

    // pas de collision => on déplace la momie
    momie.Pos = newPos;
  } else {
    // collision détectée, on ne bouge pas mais on change de direction
    int rd = rand() % 4; // tirage d'une valeur aléatoire entre 0 et 3
    // les 4 directions possibles : en haut, à droite, en bas, à gauche
    V2 Dir[4] = {V2(0, 1), V2(1, 0), V2(0, -1), V2(-1, 0)};

    momie.Dir = Dir[rd];
  }
}
void Logic() {
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
