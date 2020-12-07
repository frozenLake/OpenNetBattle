#include "bnGame.h"

#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnShaderResourceManager.h"
#include "bnWebClientMananger.h"
#include "bnNaviRegistration.h"
#include "bnInputManager.h"
#include "bnGameOverScene.h"
#include "bnMainMenuScene.h"
#include "bnFakeScene.h"
#include "bnConfigReader.h"
#include "bnConfigScene.h"
#include "bnFont.h"
#include "bnText.h"
#include "bnAppButton.h"
#include "bnQueueMobRegistration.h"
#include "bnQueueNaviRegistration.h"
#include "bnResourceHandle.h"
#include "bnTaskGroup.h"

#include "SFML/System.hpp"

#include <time.h>
#include <queue>
#include <atomic>
#include <cmath>
#include <Swoosh/ActivityController.h>
#include <Swoosh/Ease.h>

Game::Game() 
  : window(), 
  reader("config.ini"),
  configSettings(),
  textureManager(),
  audioManager(),
  shaderManager(),
  ActivityController(*window.GetRenderWindow()) {

  // Link the resource handle to use all the manangers created by the game
  ResourceHandle::audio    = &audioManager;
  ResourceHandle::textures = &textureManager;
  ResourceHandle::shaders  = &shaderManager;

  // Use the engine's window settings for this platform to create a properly 
  // sized render surface...
  loadSurface.create(window.GetView().getSize().x, window.GetView().getSize().y, window.GetRenderWindow()->getSettings());

  // Use our external render surface as the game's screen
  window.SetRenderSurface(loadSurface);
}

Game::~Game() {

}

void Game::Boot()
{
  // Load font symbols for use across the entire engine...
  textureManager.LoadImmediately(TextureType::FONT);

  // Initialize the engine and log the startup time
  const clock_t begin_time = clock();
  DrawWindow::WindowMode mode = configSettings.IsFullscreen() ? DrawWindow::WindowMode::fullscreen : DrawWindow::WindowMode::window;

  window.Initialize(windowMode);

  Logger::Logf("Engine initialized: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);

  atomic<int> progress(0);

  Callback<void()> graphics;
  graphics.Slot(std::bind(&Game::RunGraphicsInit, this, &progress));

  Callback<void()> Audio();
  Audio().Slot(std::bind(&Game::RunAudioInit, this, &progress));

  Callback<void()> shaders;
  //Callback<void> scripts;

  Callback<void()> navis;
  navis.Slot(std::bind(&Game::RunNaviInit, this, &progress));

  Callback<void()> mobs;
  mobs.Slot(std::bind(&Game::RunMobInit, this, &progress));

  TaskGroup tasks;
  tasks.AddTask("Init graphics", graphics);
  tasks.AddTask("Init Audio()", Audio());
  tasks.AddTask("Init shaders", shaders);
  tasks.AddTask("Load Navis", navis);
  tasks.AddTask("Load mobs", mobs);

  // Tell the input event loop how to behave when the app loses and regains focus
  INPUT.BindLoseFocusEvent(std::bind(&Game::LoseFocus, this));
  INPUT.BindRegainFocusEvent(std::bind(&Game::GainFocus, this));
  //INPUT.BindResizedEvent(std::bind(&Game::Resize, this));
  INPUT.SupportConfigSettings(reader);

  if (configSettings.IsOK()) {
    // If the file is good, use the Audio() and 
    // controller settings from the config
    audioManager.EnableAudio(reader.GetConfigSettings().IsAudioEnabled());
    audioManager.SetStreamVolume(((reader.GetConfigSettings().GetMusicLevel()) / 3.0f)*100.0f);
    audioManager.SetChannelVolume(((reader.GetConfigSettings().GetSFXLevel()) / 3.0f)*100.0f);
  }

  mouse.setTexture(textureManager.LoadTextureFromFile("resources/ui/mouse.png"));
  mouse.setScale(2.f, 2.f);
  mouseAnimation = Animation("resources/ui/mouse.animation");
  mouseAnimation << "DEFAULT" << Animator::Mode::Loop;
  mouseAlpha = 1.0;

  // set a loading spinner on the bottom-right corner of the screen
  spinner.setTexture(textureManager.LoadTextureFromFile("resources/ui/spinner.png"));
  spinner.setScale(2.f, 2.f);
  spinner.setPosition(float(window.GetView().getSize().x - 64), float(window.GetView().getSize().y - 50));

  spinnerAnimator = Animation("resources/ui/spinner.animation") << "SPIN" << Animator::Mode::Loop;
}

void Game::Poll()
{
  Clock clock;
  float elapsed = 0.0f;
  float speed = 1.0f;
  float messageCooldown = 3;

  bool inMessageState = true;

  while (window.Running()) {
    clock.restart();

    // Poll input
    INPUT.Update();

    // Prepare for next draw calls
    window.Clear();
  }
}

void Game::SetWindowMode(DrawWindow::WindowMode mode)
{
  windowMode = mode;
}

void Game::LoadConfigSettings()
{
  // try to read the config file
  configSettings = reader.GetConfigSettings();

  if (configSettings.IsOK()) {
    const WebServerInfo info = configSettings.GetWebServerInfo();
    const std::string version = info.version;
    const std::string URL = info.URL;
    const int port = info.port;
    const std::string username = info.user;
    const std::string password = info.password;

    if (URL.empty() || version.empty() || username.empty() || password.empty()) {
      Logger::Logf("One or more web server fields are empty in config.");
    }
    else {

      Logger::Logf("Connecting to web server @ %s:%i (Version %s)", URL.data(), port, version.data());

      WEBCLIENT.ConnectToWebServer(version.data(), URL.data(), port);

      auto result = WEBCLIENT.SendLoginCommand(username.data(), password.data());

      Logger::Logf("waiting for server...");

      int timeoutCount = 0;
      constexpr int MAX_TIMEOUT = 5;

      while (!is_ready(result) && timeoutCount < MAX_TIMEOUT) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        Logger::Logf("timeout %i", ++timeoutCount);
      }

      if (timeoutCount == MAX_TIMEOUT) {
        Logger::Logf("Could not communicate with the server. Aborting automatic login.");
      }
      else if (is_ready(result)) {
        bool success = result.get();
        if (success) {
          Logger::Logf("Logged in! Welcome %s!", username.data());
        }
        else {
          Logger::Logf("Could not authenticate. Aborting automatic login");
        }
      }
    }
  }
}

void Game::RunNaviInit(std::atomic<int>* progress) {
  clock_t begin_time = clock();

  NAVIS.LoadAllNavis(*progress);

  Logger::Logf("Loaded registered navis: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);
}

void Game::RunMobInit(std::atomic<int>* progress) {
  clock_t begin_time = clock();

  MOBS.LoadAllMobs(*progress);

  Logger::Logf("Loaded registered mobs: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);
}

void Game::RunGraphicsInit(std::atomic<int> * progress) {
  clock_t begin_time = clock();
  textureManager.LoadAllTextures(*progress);

  Logger::Logf("Loaded textures: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);

  begin_time = clock();
  shaderManager.LoadAllShaders(*progress);

  Logger::Logf("Loaded shaders: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);
}

void Game::RunAudioInit(std::atomic<int> * progress) {
  const clock_t begin_time = clock();
  audioManager.LoadAllSources(*progress);

  Logger::Logf("Loaded Audio() sources: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);
}

void Game::GainFocus() {
  window.RegainFocus();
  audioManager.EnableAudio(true);

#ifdef __ANDROID__
  // TODO: Reload all graphics and somehow reassign all gl IDs to all allocated sfml graphics structures
  // Textures().RecoverLostGLContext();
  // ENGINE.RecoverLostGLContext(); // <- does the window need recreation too?
#endif
}

void Game::LoseFocus() {
  audioManager.EnableAudio(false);
}

void Game::Resize(int newWidth, int newHeight) {
  float windowRatio = (float)newWidth / (float)newHeight;
  float viewRatio = window.GetView().getSize().x / window.GetView().getSize().y;

  showScreenBars = windowRatio >= viewRatio;
  window.Resize(newWidth, newHeight);
}