#include<SDL2/SDL.h>
#include "game.h"

const int thickness = 15;
const float paddleH = 90.0f;

Game::Game()
:mIsRunning(true),
mTicksCount(0),
mPaddleDir(0)
{}

bool Game::Initialize()
{
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter1)", //título
        100, // Top left x-coordinate of window
        100, // Top left y-coordinate of window
        1024, //width of window
        768, //height of window
        0 //Flags   (0 for no flags set) 
    );
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow, //Window to create renderer for
        -1, //usually -1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 728.0f/2.0f;
    mPaddle2Pos.x = 1000.0f; //atenção
    mPaddle2Pos.y = 728.0f/2.0f;
    mBallPos.x = 1024.0f/2.0f;
    mBallPos.y = 728.0f/2.0f;
    mBallVel.x = 200.0f; //alterar***
    mBallVel.y = 235.0f;
    return true;
}


void Game::RunLoop()
{
    //SDL_Log("mIsRunning: %s", mIsRunning ? "true" : "false");
    //mIsRunning = true;
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
    
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            mIsRunning = false;
            break;
        }
    }
    const Uint8* state = SDL_GetKeyboardState(NULL); //Uint8 é um inteiro sem sinal de 8 bits (0 - 255)
        if (state[SDL_SCANCODE_ESCAPE])
            mIsRunning = false;

    mPaddleDir = 0;

    if (state[SDL_SCANCODE_W])
    {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
        mPaddleDir += 1;
    }
    
     mPaddle2Dir = 0;

    if (state[SDL_SCANCODE_I])
    {
        mPaddle2Dir -= 1;
    }
    if (state[SDL_SCANCODE_K])
    {
        mPaddle2Dir += 1;
    }

    
}

void Game::UpdateGame()
{
    //espera que passe 16ms desde o último frame
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
    //Delta time é a diferença em ticks do último frame, convertido para segundos
    float deltaTime = (SDL_GetTicks() - mTicksCount)/ 1000.0f; //SDL_GetTicks retorna os ticks em miliseconds
    //atualiza o contador de ticks
    mTicksCount = SDL_GetTicks();

    //força um valor máximo para o delta time - 50ms
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    /*atualiza tudo*/
    SDL_Log("delta time: %f", deltaTime);
    //atualiza paddle
    if(mPaddleDir != 0)
    {
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime; 

        //para não se mover para fora da tela
        if (mPaddlePos.y < (paddleH/2.0f + thickness)) //limite de cima
        {
            mPaddlePos.y = paddleH/2.0f + thickness;
        }
        else if (mPaddlePos.y > (768.0f - paddleH)) //limite de baixo
        {
            mPaddlePos.y = 768.0f - paddleH;
        }
    }

    if(mPaddle2Dir != 0)
    {
        mPaddle2Pos.y += mPaddle2Dir * 300.0f * deltaTime; 

        //para não se mover para fora da tela
        if (mPaddle2Pos.y < (paddleH/2.0f + thickness)) //limite de cima
        {
            mPaddle2Pos.y = paddleH/2.0f + thickness;
        }
        else if (mPaddle2Pos.y > (768.0f - paddleH)) //limite de baixo
        {
            mPaddle2Pos.y = 768.0f - paddleH;
        }
    }



    //atualiza a posição da bola
    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;

    //colisão com as paredes de cima e de baixo
    if (mBallPos.y <= thickness && mBallVel.y < 0.0f) //parede de cima
    {
        mBallVel.y *= -1;
    }
    if (mBallPos.y >= (728 - thickness) && mBallVel.y > 0.0f) //parede de baixo
    {
        mBallVel.y *= -1;
    }

    /*
    //colisão com parede da direita
    if (mBallPos.x >= (1024 - thickness) && mBallVel.x > 0.0f)
     {
        mBallVel.x *= -1.0f;
     }
     */
    // colisão com o paddle
    int diff = abs(mBallPos.y - mPaddlePos.y);
    int diff2 = abs(mBallPos.y - mPaddle2Pos.y);
    if (
        diff <= paddleH/2.0f &&
        mBallPos.x <= 25.0f || mBallPos.x >= 20.0f &&
        mBallPos.x < 0.0f
    )
    {
        mBallVel.x *= -1.0f;
    }
    
    if (
        diff2 <= paddleH/2.0f &&
        mBallPos.x >= 1000.0f || mBallPos.x <= 1005.0f &&
        mBallPos.x < 0.0f
    )
    {
        mBallVel.x *= -1.0f;
    }
}

void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(
        mRenderer,
        0, //Red
        0, //Green
        255, //Blue
        255 //Alpha
    );

    /*limpa o back buffer*/
    SDL_RenderClear(mRenderer);

    /*desenha*/
    SDL_SetRenderDrawColor(mRenderer, 255,255,255,255);

    /*parede de cima - top wall*/
    SDL_Rect Wall{
        0, //top left x
        0, //top left y
        1024, //width
        thickness //height
    };
    SDL_RenderFillRect(mRenderer, &Wall);

    /*Parede de baixo - bottom wall*/
    Wall.y = 728 - thickness;
    SDL_RenderFillRect(mRenderer, &Wall);
    
    /*Parede direita - right wall*/
    /*
    Wall.x = 1024 - thickness;
    Wall.y = 0;
    Wall.w = thickness;
    Wall.h = 1024; 
    SDL_RenderFillRect(mRenderer, &Wall);
    */
    //desenha a bola e o paddle
    SDL_Rect ball {
        static_cast<int>(mBallPos.x - thickness/2),
        static_cast<int>(mBallPos.y - thickness/2),
        thickness,
        thickness
    };
    SDL_RenderFillRect(mRenderer, &ball);   

    SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - (paddleH)/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

    SDL_Rect paddle2{
        static_cast<int>(mPaddle2Pos.x),
        static_cast<int>(mPaddle2Pos.y - (paddleH)/2),
        thickness,
        static_cast<int>(paddleH)
    };
    SDL_RenderFillRect(mRenderer, &paddle2);

    /*troca o back buffer pelo front buffer*/
    SDL_RenderPresent(mRenderer);
}


void Game::Shutdown()
{
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit();
}
