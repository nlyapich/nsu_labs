package ru.nsu.nlyapich.controller;

import ru.nsu.nlyapich.Command;
import ru.nsu.nlyapich.model.ModelWorld;

import java.awt.event.*;
import java.util.*;

public class ControllerWorld implements KeyListener, Session {
    private final Set<Integer> pressedKeys;

    private ModelWorld model;

    private int mX;
    private int mY;

    private boolean switchStatus;

    public ControllerWorld(ModelWorld model){
        this.model = model;
        this.pressedKeys = new HashSet<Integer>();
        this.mX = ModelWorld.WIDTH / 2;
        this.mY = ModelWorld.HEIGHT / 2;
        this.switchStatus = false;
    }

    public void startGame(){
        pressedKeys.clear();
        model.start();
    }
    public void stopGame(){
        model.stop();
    }
    public void continueGame(){
        pressedKeys.clear();
        model.continueGame();
    }

    @Override
    public synchronized void keyTyped(KeyEvent e) {}

    @Override
    public synchronized void keyPressed(KeyEvent e) {
        pressedKeys.add(e.getKeyCode());
        if (!pressedKeys.isEmpty()) {
            for (Iterator<Integer> it = pressedKeys.iterator(); it.hasNext();) {
                switch (it.next()) {
                    case KeyEvent.VK_E -> {
                        model.handleRequest(Command.ACTIVATE_BLOCK);
                    }
                    case KeyEvent.VK_P -> {
                        model.handleRequest(Command.PAUSE);
                        pressedKeys.remove(e.getKeyCode());
                        switchStatus = true;
                    }
                    case KeyEvent.VK_W -> {
                        model.handleRequest(Command.MOVE_UP);
                    }
                    case KeyEvent.VK_S -> {
                        model.handleRequest(Command.MOVE_DOWN);
                    }
                    case KeyEvent.VK_A -> {
                        model.handleRequest(Command.MOVE_LEFT);
                    }
                    case KeyEvent.VK_D -> {
                        model.handleRequest(Command.MOVE_RIGHT);
                    }
                    case KeyEvent.VK_LEFT -> {
                        model.handleRequest(Command.ROTATE_LEFT);
                    }
                    case KeyEvent.VK_RIGHT -> {
                        model.handleRequest(Command.ROTATE_RIGHT);
                    }
                    case KeyEvent.VK_SPACE -> {
                        model.handleRequest(Command.ATTACK);
                    }
                    default -> {
                    }
                }
            }
        }
    }

    @Override
    public synchronized void keyReleased(KeyEvent e) {
        pressedKeys.remove(e.getKeyCode());
    }

    @Override
    public boolean isSwitchStatus() {
//        System.out.println("In controllerWorld switch status is " + (switchStatus ? "true" : "false"));
        return (switchStatus || (model.getStatus() == StatusSession.GAME_OVER) || (model.getStatus() == StatusSession.WIN));
    }

    @Override
    public void updateSwitchStatus() {
//        pressedKeys.clear();
        switchStatus = false;
        StatusSession session = model.getStatus();
        if (session == StatusSession.PAUSE){
            model.setStatus(StatusSession.ACTIVE);
        }
    }

    @Override
    public StatusSession getStatus() {
        return model.getStatus();
    }
}
