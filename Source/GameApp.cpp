#include "GameApp.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <sstream>

#include "SDLErrorReport.h"

GameApp::GameApp(std::string appname) : m_AppName(appname)
{
	
}

GameApp::~GameApp()
{
	Cleanup();
}

void GameApp::Cleanup()
{
	AppCleanup();

	m_Window.Release();

	// Shutdown SDL
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool GameApp::Init()
{
	// Initialise SDL, report error if it fails
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		Error2MsgBox("SDL Initialisation Failed!\n");
		return false;
	}

	//Initialize PNG/JPG loading 
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::string err_msg = "SDL_image could not initialize!\n";
		err_msg += IMG_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", err_msg.c_str(), nullptr);
		return false;
	}

	//Initialize SDL_ttf 
	if (TTF_Init() == -1)
	{
		std::string err_msg = "SDL_ttf could not initialize!\n";
		err_msg += TTF_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", err_msg.c_str(), nullptr);
		return false;
	}

	// Create a window, report error if window not created
	if (!m_Window.Create("Test Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_SHOWN /*| SDL_WINDOW_RESIZABLE*/))
	{
		Error2MsgBox("Window Creation Failed.\n");
		return false;
	}

	// Creates a renderer and clears the window
	if (!m_Window.CreateRenderer())
	{
		Error2MsgBox("Renderer Creation Failed.\n");
		return false;
	}

	m_Timer.Initialize();

	return AppInit();
}

void GameApp::HandleEvents()
{
	SDL_Event Event;

	while (SDL_PollEvent(&Event))
	{
		if (Event.type == SDL_WINDOWEVENT && Event.window.windowID == m_Window.GetID())
			m_Window.OnEvent(Event);
		else
			OnEvent(Event);
	}
}

void GameApp::MainLoop()
{
	m_Running = true;

	// Application will indicate quit by setting m_Running to false
	// until then we loop thro event handling and rendering each frame
	while (m_Running)
	{
		HandleEvents();

		if (m_Timer.Update())
		{
			// Gets time since last frame
			double deltaTime = m_Timer.GetDeltaTime();

			// Update the derived class
			AppUpdate(deltaTime);

			if (m_ShowFPS)
				DrawFramesPerSecond();

		}

		// Draw our frame
		Render();
	}
}

void GameApp::Render()
{
	// If we have valid window & renderer then render the frame
	if (m_Window.CanRender())
	{
		AppRender(m_Window.GetRenderer());
		m_Window.Present();
	}
}

int GameApp::Execute()
{
	// Initialise SDL and create window
	if (!Init())
		return -1;

	MainLoop();

	Cleanup();

	return 0;
}

void GameApp::DrawFramesPerSecond()
{
	std::stringstream strm;
	strm << m_AppName << "--Frames Per Second = " << round(m_Timer.GetFrameRate());
	strm << ", missed " << m_Timer.GetMissedFrames() << " frames";

	// Now set the new caption to the main window.
	m_Window.SetTitle(strm.str());
}