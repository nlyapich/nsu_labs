package ru.nsu.nlyapich.model;

import ru.nsu.nlyapich.Command;
import ru.nsu.nlyapich.controller.StatusSession;

import java.util.Arrays;
import java.util.Vector;

public class ModelMenu extends BaseModel {
    public ModelMenu(){
        super();
        buttons = new Vector<StatusSession>(Arrays.asList(new StatusSession[]{StatusSession.START, StatusSession.HELP, StatusSession.EXIT}));
    }
}
