#include "GameEngine.h"

#include "Systems/Settings/SettingsManager.h"
#include "Systems/Window/Window.h"
#include "Systems/Screen/Screen.h"
#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"
#include "Systems/Physics/Physics.h"

#include "Managers/ShaderManager.h"
#include "Managers/SceneManager.h"
#include "Renderer/RenderManager.h"

#include "Arguments/ArgumentsAnalyzer.h"

#include "Core/Console/Console.h"

#include "Wrappers/OpenGL/GL.h"

// #include "Debug/Debugger.h"

#include "Modules/SDLModule.h"

#include "Editor/Editor.h"

#define ENGINE_SETTINGS_PATH "Assets/LiteEngine.ini"

/*
 * TODO (Medium Priority): Refactor initialization responsabilities
*/

void GameEngine::Init ()
{
	InitSettings ();

	SDLModule::Init ();

	Window::Init ();

	InitOpenGL ();

	// Debugger::Init ();

	Input::Init ();

	Physics::Init ();

	Editor::Init ();

	InitRenderer ();
	InitScene ();
}

void GameEngine::Clear ()
{
	ShaderManager::Instance()->Clear();
	SceneManager::Instance()->Clear();
	RenderManager::Instance()->Clear();

	Editor::Clear ();

	Window::Close ();

	SDLModule::Quit ();
}

void GameEngine::InitSettings ()
{
	std::string settingsPath = ENGINE_SETTINGS_PATH;

	Argument* arg = ArgumentsAnalyzer::Instance ()->GetArgument ("configpath");

	if (arg != nullptr) {
		settingsPath = arg->GetArgs () [0];
	}

	SettingsManager::Instance ()->Init (settingsPath);
}

void GameEngine::InitOpenGL ()
{
	// GL::Viewport(0, 0, 10, 10);

	GL::ClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	GL::ClearDepth( 1.0f );
	// GL::Hint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	GL::DepthFunc( GL_LEQUAL );
	GL::Enable( GL_DEPTH_TEST );
	GL::Enable(GL_CULL_FACE);
	GL::Enable(GL_BLEND);

	GL::CullFace (GL_BACK);
	GL::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::string error = (char*) glewGetErrorString(err);
		Console::LogError (error);
	}

	std::string glewvers = (char*) glewGetString(GLEW_VERSION);
	Console::Log ("Status: Using GLEW " + glewvers);

	if (glewIsSupported ("GL_VERSION_4_5")) {
		Console::Log ("Ready for OpenGL 4.5");
	} else {
		Console::LogError ("OpenGL 4.5 not supported");
	}
}

void GameEngine::InitRenderer ()
{
	RenderManager::Instance ()->Init ();

	Argument* arg = ArgumentsAnalyzer::Instance ()->GetArgument ("rendermode");

	if (arg == nullptr) {
		return;
	}

	RenderManager::Instance ()->SetRenderMode ((RenderMode) std::stoi (arg->GetArgs () [0]));
}

void GameEngine::InitScene ()
{
	Argument* arg = ArgumentsAnalyzer::Instance ()->GetArgument ("startscene");

	if (arg == nullptr)
	{
		Console::LogError ("There is no scene to load!");
		exit (0);
	}

	SceneManager::Instance ()->Load (arg->GetArgs () [0]);
}