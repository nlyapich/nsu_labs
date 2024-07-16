package ru.nsu.nlyapich.command;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import ru.nsu.nlyapich.App;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

public class CommandFactoryTest {
    @Test
    public void testGetCommand() throws IOException, ClassNotFoundException, InstantiationException, IllegalAccessException {
        String configFileName = App.class.getClassLoader().getResource("examples/commands.properties").getPath();
        FileInputStream fis = new FileInputStream(configFileName);
        Properties prop = new Properties();
        prop.load(fis);

        CommandFactory commandFactory = new CommandFactory(prop);

        for (Object c : prop.keySet()) {
            Character curChar = ((String) c).charAt(0);
            Command curCommand = commandFactory.getCommand(curChar);

            Assertions.assertEquals(prop.getProperty((String)c), curCommand.getClass().getName());
        }
    }

    @Test
    public void testException() throws IOException {
        String configFileName = App.class.getClassLoader().getResource("examples/exception_test/bad_commands.properties").getPath();
        FileInputStream fis = new FileInputStream(configFileName);
        Properties prop = new Properties();
        prop.load(fis);

        Assertions.assertThrows(ClassNotFoundException.class, () -> {
            CommandFactory commandFactory = new CommandFactory(prop);
            commandFactory.getCommand('^');
        });
    }
}
