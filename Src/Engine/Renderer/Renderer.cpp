#include "Renderer.h"
#include "Game.h"
#include "Actor.h"
#include "Config.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include <algorithm>

Renderer::Renderer(Game* game)
: mGame(game)
, mWindow(nullptr)
, mRenderer(nullptr)
{}

bool Renderer::Initialize()
{
    // 窗口创建前，请求OpenGL属性
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // 设置GL版本3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // 设置RGBA颜色深度
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);        // 请求双缓冲
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);  // 请求硬件(GPU)加速

    mWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if(!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // 创建窗口后，创建上下文
    mContext = SDL_GL_CreateContext(mWindow);

    // 初始化GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        SDL_Log("Failed to initialize GLEW");
        return false;
    }

    // 清理无害错误
    glGetError();
	// 开启垂直同步，限制帧率到显示器刷新率，避免 GPU 跑满
	SDL_GL_SetSwapInterval(1);

    if(!LoadShaders())      // 编译着色器
    {
        SDL_Log("Failed to load shaders");
        return false;
    }

    CreateSpriteVerts();

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    // 字体初始化
    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        return false;
    }

    mFont = TTF_OpenFont("Assets/ARIAL.TTF", 18);
    if (!mFont) {
        SDL_Log("TTF_OpenFont failed: %s", TTF_GetError());
        return false;
    }

    return true;
}

void Renderer::Shutdown()
{
    if (mFont) TTF_CloseFont(mFont);
    TTF_Quit();

    delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	SDL_GL_DeleteContext(mContext);

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
    for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();
}

void Renderer::Draw()
{
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
    for(auto sprite : mSprites)
    {
        sprite->Draw(mSpriteShader);
    }

    SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	if(iter != mSprites.end()) mSprites.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

void Renderer::CreateSpriteVerts()
{
    // x, y, z, u, v 顶点坐标和纹理坐标
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f,
		 0.5f,  0.5f, 0.f, 1.f, 0.f,
		 0.5f, -0.5f, 0.f, 1.f, 1.f,
		-0.5f, -0.5f, 0.f, 0.f, 1.f
	};
    
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

bool Renderer::LoadShaders()
{
    // 2D精灵着色器
    mSpriteShader = new Shader();
    if(!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")){
        return false;
    }

    mSpriteShader->SetActive();
    // 设置 2D 的 VP 矩阵
    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

    return true;
}