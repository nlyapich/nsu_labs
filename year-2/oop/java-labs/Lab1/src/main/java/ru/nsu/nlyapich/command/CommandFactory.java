package ru.nsu.nlyapich.command;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

public class CommandFactory {
    private static final Logger logger = LogManager.getLogger(CommandFactory.class);
    private Map<Character, String> commandMap;

    public CommandFactory(Properties prop) throws ClassNotFoundException, IOException {
        logger.info("Creating CommandFactory");
        commandMap = new HashMap<>();
        for (Enumeration en = prop.propertyNames(); en.hasMoreElements();){
            String commandName = (String) en.nextElement();
            String className = prop.getProperty(commandName);
            logger.info("Add new command: " + commandName + " " + className);
            commandMap.put(commandName.charAt(0), className);
        }
    }

    public Command getCommand(Character commandName) throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        String className = commandMap.get(commandName);
        return (Command) Class.forName(className).getClassLoader().loadClass(className).newInstance();
    }
}