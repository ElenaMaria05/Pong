#include <raylib.h>
#include<iostream>
using namespace std;

const int latime_fereastra = 1000;
const int inaltime_fereastra = 600;
int player1_score=0;
int player2_score = 0;
bool continuare_joc = true;

class Minge {
public:
    float x_minge, y_minge;
    int x_speed, y_speed;
    int raza;

    void Draw() {
        DrawCircle(x_minge, y_minge, raza, RED);
    }
    
    void Update() {
        x_minge += x_speed;
        y_minge += y_speed;
        
        if (y_minge + raza >= inaltime_fereastra || y_minge-raza <= 0) { //cand loveste marginile se inverseaza viteza
            y_speed *= -1;
        }
        if (CheckCollisionPointLine(Vector2{ x_minge + raza,y_minge + raza }, Vector2{ latime_fereastra+1 ,0 }, Vector2{ latime_fereastra+1 ,inaltime_fereastra }, 10)) {
            x_speed *= -1;
            player1_score++;
        }
        if (CheckCollisionPointLine(Vector2{ x_minge - raza,y_minge - raza }, Vector2{ -1 ,-1 }, Vector2{ -1 ,inaltime_fereastra-1 }, 10)) {
            x_speed *= -1;
            player2_score++;
        }
        if (player1_score == 7 || player2_score == 7) {
            continuare_joc = false;
        }
    }
};

class Paleta1 {
protected:
    void LimiteExterioare() {
        if (y_paleta <= 0) {
            y_paleta = 0;
        }
        if (y_paleta + inaltime_paleta >= inaltime_fereastra) {
            y_paleta = inaltime_fereastra - inaltime_paleta;
        }
    }
public:
    float x_paleta, y_paleta;
    float latime_paleta, inaltime_paleta;
    int y_speed; //se poate misca doar pe axa y;

    void Draw() {
        DrawRectangle(x_paleta, y_paleta, latime_paleta, inaltime_paleta, GREEN);//desenarea dreptunghiului incepe din coltul stanga sus, deci alinierea se face dupa el
    }
    void Update() {
        if (IsKeyDown(KEY_W)) {
            y_paleta -= y_speed;
        }
        if (IsKeyDown(KEY_S)) {
            y_paleta += y_speed;
        }
        LimiteExterioare();
    }
};

class Paleta2:public Paleta1 {
public:
    void Update() {
        if (IsKeyDown(KEY_UP)) {
            y_paleta -= y_speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y_paleta += y_speed;
        }
        LimiteExterioare();
    }
};

int main() {

    cout << "Jocul incepe"<<endl;
    const float raza_cerc = 15;
    int const latime_dreptunghi = 25;
    int const inaltime_dreptunghi = 100;

    InitWindow(latime_fereastra, inaltime_fereastra, "It's playing...");
    SetTargetFPS(60);

    Minge minge;
    minge.raza = raza_cerc;
    minge.x_minge = latime_fereastra / 2;
    minge.y_minge = inaltime_fereastra / 2;
    minge.x_speed = 7;
    minge.y_speed = 7;

    Paleta1 player1;
    player1.x_paleta = -6;
    player1.y_paleta = inaltime_fereastra / 2 - inaltime_dreptunghi / 2;
    player1.latime_paleta = latime_dreptunghi;
    player1.inaltime_paleta = inaltime_dreptunghi;
    player1.y_speed = 7;

    Paleta2 player2;
    player2.latime_paleta = latime_dreptunghi;
    player2.inaltime_paleta = inaltime_dreptunghi;
    player2.x_paleta = latime_fereastra - player2.latime_paleta +6;
    player2.y_paleta = inaltime_fereastra / 2 - player2.inaltime_paleta / 2;
    player2.y_speed = 7;

    while (WindowShouldClose() == false && continuare_joc) {
        BeginDrawing();
        DrawLine(latime_fereastra / 2, 0, latime_fereastra / 2, inaltime_fereastra, WHITE);
        DrawLine(latime_fereastra+1, 0 , latime_fereastra+1, inaltime_fereastra, RED);
        DrawLine(0, 0, 0, inaltime_fereastra, RED);

        ClearBackground(BLACK);

        minge.Update();
        minge.Draw();

        if (CheckCollisionCircleRec(Vector2{ minge.x_minge,minge.y_minge }, minge.raza, Rectangle{ player1.x_paleta,player1.y_paleta,player1.latime_paleta,player1.inaltime_paleta })) { //daca functia asta returneaza true, inseamna ca avem o coliziune cu paleta
            minge.x_speed *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ minge.x_minge,minge.y_minge }, minge.raza, Rectangle{ player2.x_paleta,player2.y_paleta,player2.latime_paleta,player2.inaltime_paleta })) { 
            minge.x_speed *= -1;
        }
       
        player1.Update();
        player1.Draw();
        
        player2.Update();
        player2.Draw();
        
        DrawText(TextFormat("%i", player1_score), latime_fereastra / 4, 20, 80, WHITE);
        DrawText(TextFormat("%i", player2_score), 3 * latime_fereastra / 4, 20, 80, WHITE);
        DrawText("player 1", latime_fereastra / 4, inaltime_fereastra - 35, 15, WHITE);
        DrawText("player 2", latime_fereastra-250, inaltime_fereastra - 35, 15, WHITE);
        
        EndDrawing();
    }
    float start_time = GetTime();
    while (!WindowShouldClose() && (GetTime() - start_time) < 100)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        if (player2_score == 7) {
            DrawText("PLAYER_2 A CASTIGAT!", latime_fereastra / 7, inaltime_fereastra / 2.5, 60, RED);
        }
        else {
            DrawText("PLAYER_1 A CASTIGAT!", latime_fereastra/7 , inaltime_fereastra / 2.5, 60, RED);
        }
        EndDrawing();
    }
    cout <<"Player_2: "<<player2_score << endl;
    cout << "Player_1: " << player1_score;
    CloseWindow();
    return 0;
}