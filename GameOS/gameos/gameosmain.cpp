#include "gameos.hpp"
#include "gos_render.h"
#include <cstdio>

#include "gos_input.h"

#include "utils/shader_builder.h"
#include "utils/gl_utils.h"
#include "utils/timing.h"

#include <csignal>

extern void gos_CreateRenderer(graphics::RenderContextHandle ctx_h, graphics::RenderWindowHandle win_h, int w, int h);
extern void gos_DestroyRenderer();
extern void gos_RendererBeginFrame();
extern void gos_RendererEndFrame();
extern void gos_RendererHandleEvents();
extern void gos_RenderUpdateDebugInput();
extern void gos_RenderEnableDebugDrawCalls();
extern bool gos_RenderGetEnableDebugDrawCalls();

extern bool gosExitGameOS();

extern bool gos_CreateAudio();
extern void gos_DestroyAudio();

static bool g_exit       = false;
static bool g_focus_lost = false;

static void handle_key_down(SDL_Keysym* keysym)
{
    switch (keysym->sym)
    {
        case SDLK_ESCAPE:
            if (keysym->mod & KMOD_RALT)
                g_exit = true;
            break;
        case 'd':
            if (keysym->mod & KMOD_RALT)
                gos_RenderEnableDebugDrawCalls();
            break;
    }
}

static void process_events()
{
    input::beginUpdateMouseState();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (g_focus_lost)
        {
            if (event.type != SDL_WINDOWEVENT_FOCUS_GAINED)
            {
                continue;
            }
            else
            {
                g_focus_lost = false;
            }
        }

        switch (event.type)
        {
            case SDL_KEYDOWN:
                handle_key_down(&event.key.keysym);
                // fallthrough
            case SDL_KEYUP:
                input::handleKeyEvent(&event);
                break;
            case SDL_QUIT:
                g_exit = true;
                break;
            case SDL_WINDOWEVENT_RESIZED:
            {
                auto w = (float)event.window.data1;
                auto h = (float)event.window.data2;
                glViewport(0, 0, (GLsizei)w, (GLsizei)h);
                SPEW(("INPUT", "resize event: w: %f h:%f\n", w, h));
            }
            break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                g_focus_lost = true;
                break;
            case SDL_MOUSEMOTION:
                input::handleMouseMotion(&event);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                //input::handleMouseButton(&event);
                break;
            case SDL_MOUSEWHEEL:
                input::handleMouseWheel(&event);
                break;
        }
    }

    input::updateMouseState();
    input::updateKeyboardState();
}

extern bool g_disable_quads;

static void draw_screen()
{
    g_disable_quads = false;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    const int viewport_w = Environment.drawableWidth;
    const int viewport_h = Environment.drawableHeight;
    glViewport(0, 0, viewport_w, viewport_h);

    CHECK_GL_ERROR;

    // TODO: reset all states to sane defaults!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gos_RendererBeginFrame();
    Environment.UpdateRenderers();
    gos_RendererEndFrame();

    glUseProgram(0);
    CHECK_GL_ERROR;
}

extern float frameRate;


const char* getStringForType(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            return "DEBUG_TYPE_ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "DEBUG_TYPE_PERFORMANCE";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "DEBUG_TYPE_PORTABILITY";
        case GL_DEBUG_TYPE_MARKER:
            return "DEBUG_TYPE_MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP:
            return "DEBUG_TYPE_PUSH_GROUP";
        case GL_DEBUG_TYPE_POP_GROUP:
            return "DEBUG_TYPE_POP_GROUP";
        case GL_DEBUG_TYPE_OTHER:
            return "DEBUG_TYPE_OTHER";
        default:
            return "(undefined)";
    }
}

const char* getStringForSource(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_SOURCE_API:
            return "DEBUG_SOURCE_API";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "DEBUG_SOURCE_SHADER_COMPILER";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "DEBUG_SOURCE_WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "DEBUG_SOURCE_THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "DEBUG_SOURCE_APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
            return "DEBUG_SOURCE_OTHER";
        default:
            return "(undefined)";
    }
}

const char* getStringForSeverity(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            return "DEBUG_SEVERITY_HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "DEBUG_SEVERITY_MEDIUM";
        case GL_DEBUG_SEVERITY_LOW:
            return "DEBUG_SEVERITY_LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "DEBUG_SEVERITY_NOTIFICATION";
        default:
            return "(undefined)";
    }
}
//typedef void (GLAPIENTRY *GLDEBUGPROCARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#ifdef PLATFORM_WINDOWS
void GLAPIENTRY OpenGLDebugLog(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
#else
void GLAPIENTRY OpenGLDebugLog(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
#endif
{
    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION && severity != GL_DEBUG_SEVERITY_LOW)
    {
        printf("Type: %s; Source: %s; ID: %d; Severity : %s\n", getStringForType(type), getStringForSource(source), id, getStringForSeverity(severity));
        printf("Message : %s\n", message);
    }
}

#ifndef DISABLE_GAMEOS_MAIN
int main(int argc, char** argv)
{
    //signal(SIGTRAP, SIG_IGN);

    // gather command line
    size_t cmdline_len = 0;
    for (int i = 0; i < argc; ++i)
    {
        cmdline_len += strlen(argv[i]);
        cmdline_len += 1;  // ' '
    }
    char* cmdline = new char[cmdline_len + 1];
    size_t offset = 0;
    for (int i = 0; i < argc; ++i)
    {
        size_t arglen = strlen(argv[i]);
        memcpy(cmdline + offset, argv[i], arglen);
        cmdline[offset + arglen] = ' ';
        offset += arglen + 1;
    }
    cmdline[cmdline_len] = '\0';

    // fills in Environment structure
    GetGameOSEnvironment(cmdline);

    delete[] cmdline;
    cmdline = nullptr;

    int w = Environment.screenWidth;
    int h = Environment.screenHeight;

    graphics::RenderWindowHandle win = graphics::create_window("mc2", w, h);
    if (!win)
    {
        return 1;
    }
    graphics::RenderContextHandle ctx = graphics::init_render_context(win);
    if (!ctx)
    {
        return 1;
    }

    graphics::make_current_context(ctx);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    //glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 76, 1, "My debug group");
    glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    glDebugMessageCallbackARB((GLDEBUGPROC)&OpenGLDebugLog, NULL);

    const char* glsl_version = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    SPEW(("GRAPHICS", "GLSL version supported: %s\n", glsl_version));

    int glsl_maj = 0, glsl_min = 0;
    sscanf(glsl_version, "%d.%d", &glsl_maj, &glsl_min);

    if (glsl_maj < 3 || (glsl_maj == 3 && glsl_min < 30))
    {
        SPEW(("GRAPHICS", "Minimum required OpenGL version is 330 ES, current: %d.%d\n", glsl_maj, glsl_min));
        return 1;
    }

    char version[16] = { 0 };
    snprintf(version, sizeof(version), "%d%d", glsl_maj, glsl_min);
    SPEW(("GRAPHICS", "Using %s shader version\n", version));

    //SDL_ShowCursor(false); // TODO disable because hard to DEBUG
    gos_CreateRenderer(ctx, win, w, h);
    if (!gos_CreateAudio())
    {  // not an error
        SPEW(("AUDIO", "Failed to create audio\n"));
    }

    Environment.InitializeGameEngine();

    timing::init();

    while (!g_exit)
    {
        uint64_t start_tick = timing::gettickcount();
        timing::sleep(10 * 1000000);

        if (gos_RenderGetEnableDebugDrawCalls())
        {
            gos_RenderUpdateDebugInput();
        }
        else
        {
            Environment.DoGameLogic();
        }

        process_events();

        gos_RendererHandleEvents();

        graphics::make_current_context(ctx);
        draw_screen();
        graphics::swap_window(win);

        g_exit |= gosExitGameOS();

        uint64_t end_tick = timing::gettickcount();
        uint64_t dt       = timing::ticks2ms(end_tick - start_tick);
        frameRate         = 1000.0f / (float)dt;
    }

    Environment.TerminateGameEngine();

    gos_DestroyRenderer();

    graphics::destroy_render_context(ctx);
    graphics::destroy_window(win);

    gos_DestroyAudio();

    return 0;
}
#endif  // DISABLE_GAMEOS_MAIN
