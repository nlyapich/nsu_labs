package ru.nsu.nlyapich;

import ru.nsu.nlyapich.controller.ControllerSessions;
import ru.nsu.nlyapich.model.*;
import ru.nsu.nlyapich.view.View;

public class Game {

    private View view;
    private ControllerSessions controllerSessions;

    public Game(){
        ModelWorld world = new ModelWorld();
        ModelMenu menu = new ModelMenu();
        ModelHelp help = new ModelHelp();
        ModelPause pause = new ModelPause();
        ModelEndGame endGame = new ModelEndGame();
        ModelWinGame winGame = new ModelWinGame();

        controllerSessions = new ControllerSessions(world, menu, help, pause, endGame, winGame);

        view = new View(world, menu, help, pause, endGame, winGame, controllerSessions);
    }

    public void start(){
        controllerSessions.start();
        view.start();
    }
}
