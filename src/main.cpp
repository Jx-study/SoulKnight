#include "App.hpp"
#include "MainMenu_Scene.hpp"

#include <iostream>
#include "Core/Context.hpp"

#include "Test_Scene.hpp"

int main(int, char**) {
    auto context = Core::Context::GetInstance();
	context->SetWindowIcon(RESOURCE_DIR "/pet00icon.png");
	App app;
	app.SetCurrentScene(std::make_shared<TestScene>());

    while (!context->GetExit()) {
        switch (app.GetCurrentState()) {
            case App::State::START:
                app.Start();
                break;

        case App::State::UPDATE:
                app.Update();
                break;

            case App::State::END:
                app.End();
                context->SetExit(true);
                break;
        }

        context->Update();
    }
    return 0;
}
