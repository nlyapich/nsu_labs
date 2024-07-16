package ru.nsu.nlyapich.controller;

import ru.nsu.nlyapich.Command;
import ru.nsu.nlyapich.model.ModelMenu;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class ControllerMenu implements KeyListener, Session {

    private ModelMenu model;
    private boolean switchStatus;
    public ControllerMenu(ModelMenu menu){
        model = menu;
        switchStatus = false;
    }

    @Override
    public void keyTyped(KeyEvent e) {

    }

    @Override
    public void keyPressed(KeyEvent e) {
        switch (e.getKeyCode()) {
            case KeyEvent.VK_ENTER -> {
                switchStatus = true;
            }
            case KeyEvent.VK_UP -> {
                model.handleRequest(Command.PREV);
            }
            case KeyEvent.VK_DOWN -> {
                model.handleRequest(Command.NEXT);
            }
            default -> {
            }
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {

    }

    @Override
    public boolean isSwitchStatus() {
        System.out.println("In controllerMenu switch status is " + (switchStatus ? "true" : "false"));
        return switchStatus;
    }

    @Override
    public void updateSwitchStatus() {
        switchStatus = false;
        model.setNullPosition();
    }

    @Override
    public StatusSession getStatus() {
        return model.getCurStatus();
    }
}
