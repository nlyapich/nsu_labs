package ru.nsu.nlyapich.model;

import ru.nsu.nlyapich.Command;
import ru.nsu.nlyapich.controller.StatusSession;

import java.util.Vector;

public class BaseModel {
    protected Vector<StatusSession> buttons;
    protected int curButton;

    public BaseModel(){
        curButton = 0;
    }

    public synchronized void handleRequest(Command command) {
        switch (command) {
            case PREV -> {
                curButton = (--curButton < 0) ? (buttons.size() - 1) : curButton;
            }
            case NEXT -> {
                curButton = (++curButton >= buttons.size()) ? 0 : curButton;
            }
            default -> {
            }
        }
    }

    public Vector<StatusSession> getButtons() {
        return buttons;
    }

    public int getCurButton(){
        return curButton;
    }

    public StatusSession getCurStatus() {
        return buttons.get(curButton);
    }

    public void setNullPosition() {
        curButton = 0;
    }
}
