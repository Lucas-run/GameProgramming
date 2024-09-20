#include<SDL2/SDL.h>

//Solução temporária para a criação do paddle e da bola
//Vector2 Struct para armazenar as coordenadas x/y
struct Vector2
{
    float x;
    float y;
};


class Game
{
    public: 
        Game();
        //Initialize the game
        bool Initialize();
        //Runs the game Loope until the game is over
        void RunLoop();
        //Shutdown the game
        void Shutdown();

    private:
        //Helper functionsfor the Loop
        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();

        //Janela criada por SDL//
        SDL_Window* mWindow;
        // Render for 2D graphics
        SDL_Renderer* mRenderer;
        //delta time, número de ticks desde o início do jogo
        Uint32 mTicksCount;
        //game should continue to run
        bool mIsRunning;
        

        //pong specifcs//
        //posição do paddle
        Vector2 mPaddlePos;
        Vector2 mPaddle2Pos;
        //posição da bola
        Vector2 mBallPos;
        // velocidade da bola
        Vector2 mBallVel;
        // direção do paddle
        int mPaddleDir;
        int mPaddle2Dir;
        


        
};

