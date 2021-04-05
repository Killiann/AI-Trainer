#include "InputManager.h"

InputManager::InputManager(ConsoleManager *c, Trainer* t) : consoleManager(c), trainer(t){
	consoleManager->AddMessage("=======INPUTS=======");
	consoleManager->AddMessage("throttle");
	consoleManager->AddMessage("brake");
	consoleManager->AddMessage("ebrake");
	consoleManager->AddMessage("left steer");
	consoleManager->AddMessage("right steer");
}

void InputManager::Update(){
	//throttle
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		throttle = 1;
	else throttle = 0;

	//brake
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		brake = 1;
	else brake = 0;

	//ebrake
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		ebrake = 1;
	else ebrake = 0;

	//left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		steerLeft = 1;
	else steerLeft = 0;

	//right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		steerRight = 1;
	else steerRight = 0;

	consoleManager->UpdateMessageValue("throttle", std::to_string(throttle));
	consoleManager->UpdateMessageValue("brake", std::to_string(brake));
	consoleManager->UpdateMessageValue("ebrake", std::to_string(ebrake));
	consoleManager->UpdateMessageValue("left steer", std::to_string(steerLeft));
	consoleManager->UpdateMessageValue("right steer", std::to_string(steerRight));
}

void InputManager::UpdateUIControls(sf::Event event, sf::Vector2f mouseCoords) {
    if (event.type == sf::Event::MouseButtonPressed)
    {
        //select cars
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            for (unsigned int i = 0; i < trainer->GetCars().size(); ++i) {
                if (trainer->GetCars()[i].containsPoint(mouseCoords)) {
                    if (i != trainer->GetCurrentID()) {
                        trainer->GetCars()[i].Select();
                        trainer->GetCars()[trainer->GetCurrentID()].Deselect();
                        trainer->SetCurrentID(i);
                    }
                }
            }
        }
    }
    if (event.type == sf::Event::KeyPressed) {
        //cycle through cars
        if (event.key.code == sf::Keyboard::Key::Num1) {
            if (!isCycleDown) {
                isCycleDown = true;
                trainer->GetCars()[trainer->GetCurrentID()].Deselect();
                if (trainer->GetCurrentID() == (trainer->GetCars().size() - 1))
                    trainer->SetCurrentID(0);
                else trainer->SetCurrentID(trainer->GetCurrentID() + 1);
                trainer->GetCars()[trainer->GetCurrentID()].Select();
            }
        }

        //reset scene
        if (event.key.code == sf::Keyboard::Key::Num0) {
            if (!isResetDown) {
                trainer->ResetScene();
                isResetDown = true;
            }
        }
        //reset scene
        if (event.key.code == sf::Keyboard::Key::Num2) {
            if (!isNextGenDown) {
                trainer->NextGeneration();
                isNextGenDown = true;
            }
        }
        //save best car
        if (event.key.code == sf::Keyboard::Key::S) {
            if (!isSaveDown) {
                trainer->SaveBestCar();
                isSaveDown = true;
            }
        }
        //toggle console
        if (event.key.code == (sf::Keyboard::Key::C)) {
            if (!isConsoleDown) {
                if (consoleManager->IsDisplayed()) consoleManager->Hide();
                else consoleManager->Show();
                isConsoleDown = true;
            }
        }
    }
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Key::Num1) {
            if (isCycleDown) isCycleDown = false;
        }
        if (event.key.code == sf::Keyboard::Key::Num0) {
            if (isResetDown) isResetDown = false;
        }
        if (event.key.code == sf::Keyboard::Key::Num2) {
            if (isNextGenDown) isNextGenDown = false;
        }
        if (event.key.code == sf::Keyboard::Key::S) {
            if (isSaveDown) isSaveDown = false;
        }
        if (event.key.code == sf::Keyboard::Key::C) {
            if (isConsoleDown) isConsoleDown = false;
        }
    }
}