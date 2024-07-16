package ru.nsu.nlyapich.model;

import ru.nsu.nlyapich.Command;
import ru.nsu.nlyapich.controller.StatusSession;

import java.util.Arrays;
import java.util.Vector;

public class ModelHelp extends BaseModel {
    private String helpString;
    public ModelHelp(){
        super();
        buttons = new Vector<StatusSession>(Arrays.asList(new StatusSession[]{StatusSession.EXIT}));
        helpString = "Move Forward - W\n" +
                    "Move Backward - S\n" +
                    "Move Left - A\n" +
                    "Move Right - D\n" +
                    "Change of direction - arrows <-, ->\n" +
                    "Pause in game - P\n" +
                    "Attack - SPACE\n" +
                    "Block activation - E";
    }

    public String getHelpString(){
        return helpString;
    }
}
