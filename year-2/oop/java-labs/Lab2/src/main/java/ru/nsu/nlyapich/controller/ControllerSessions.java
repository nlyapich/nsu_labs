package ru.nsu.nlyapich.controller;

import ru.nsu.nlyapich.Display;
import ru.nsu.nlyapich.model.*;

import java.util.EventListener;

public class ControllerSessions implements Runnable {
    public enum TypeSession{
        SESSION_WORLD,
        SESSION_MENU,
        SESSION_HELP,
        SESSION_PAUSE,
        SESSION_END_GAME,
        SESSION_WIN
    }
    private ControllerWorld controllerWorld;
    private ControllerMenu controllerMenu;
    private ControllerHelp controllerHelp;
    private ControllerPause controllerPause;
    private ControllerEndGame controllerEndGame;
    private ControllerWinGame controllerWinGame;
    private TypeSession curTypeSession;
    private Session curSession;
    private Thread controllerThread;
    private boolean running;

    public ControllerSessions(ModelWorld world, ModelMenu menu, ModelHelp help, ModelPause pause, ModelEndGame endGame, ModelWinGame win){
        controllerWorld = new ControllerWorld(world);
        controllerMenu = new ControllerMenu(menu);
        controllerHelp = new ControllerHelp(help);
        controllerPause = new ControllerPause(pause);
        controllerEndGame = new ControllerEndGame(endGame);
        controllerWinGame = new ControllerWinGame(win);
        curSession = controllerMenu;
        curTypeSession = TypeSession.SESSION_MENU;

        running = false;
    }

    @Override
    public void run() {
        while (running) {
            if (curSession.isSwitchStatus()){
                switchController();
                curSession.updateSwitchStatus();
            }
        }
    }


    public TypeSession getCurTypeSession() {
        return curTypeSession;
    }


    public void start(){
        if (running){
            return;
        }

        running = true;

        Display.addEventListener((EventListener) curSession);

        controllerThread = new Thread((Runnable) this);
        controllerThread.start();
    }

    public void stop() {
        if (!running)
            return;

        running = false;

        try {
            controllerThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private void switchController(){
        Display.removeEventListener((EventListener) curSession);
        switch (curTypeSession) {
            case SESSION_WORLD -> {
               switch (controllerWorld.getStatus()) {
                   case GAME_OVER -> {
                       controllerWorld.stopGame();
                       curSession = controllerEndGame;
                       curTypeSession = TypeSession.SESSION_END_GAME;
                   }
                   case PAUSE -> {
                       controllerWorld.stopGame();
                       curSession = controllerPause;
                       curTypeSession = TypeSession.SESSION_PAUSE;
                   }
                   case WIN -> {
                       controllerWorld.stopGame();
                       curSession = controllerWinGame;
                       curTypeSession = TypeSession.SESSION_WIN;
                   }
               }
            }
            case SESSION_MENU -> {
                switch (controllerMenu.getStatus()) {
                    case START -> {
                        curSession = controllerWorld;
                        curTypeSession = TypeSession.SESSION_WORLD;
                        controllerWorld.startGame();
                    }
                    case HELP -> {
                        curSession = controllerHelp;
                        curTypeSession = TypeSession.SESSION_HELP;
                    }
                    case EXIT -> {
                        System.exit(0);
                    }
                }
            }
            case SESSION_HELP -> {
                switch (controllerHelp.getStatus()) {
                    case EXIT -> {
                        curSession = controllerMenu;
                        curTypeSession = TypeSession.SESSION_MENU;
                    }
                }
            }
            case SESSION_PAUSE -> {
                switch (controllerPause.getStatus()) {
                    case CONTINUE -> {
                        curSession = controllerWorld;
                        curTypeSession = TypeSession.SESSION_WORLD;
                        controllerWorld.continueGame();
                    }
                    case EXIT -> {
                        curSession = controllerMenu;
                        curTypeSession = TypeSession.SESSION_MENU;
                    }
                }
            }
            case SESSION_END_GAME -> {
                switch (controllerEndGame.getStatus()) {
                    case RESTART -> {
                        curSession = controllerWorld;
                        curTypeSession = TypeSession.SESSION_WORLD;
                        controllerWorld.startGame();
                    }
                    case EXIT -> {
                        curSession = controllerMenu;
                        curTypeSession = TypeSession.SESSION_MENU;
                    }
                }
            }
            case SESSION_WIN -> {
                switch (controllerWinGame.getStatus()) {
                    case RESTART -> {
                        curSession = controllerWorld;
                        curTypeSession = TypeSession.SESSION_WORLD;
                        controllerWorld.startGame();
                    }
                    case EXIT -> {
                        curSession = controllerMenu;
                        curTypeSession = TypeSession.SESSION_MENU;
                    }
                }
            }
            default -> {
            }
        }
        Display.addEventListener((EventListener) curSession);
    }
}
