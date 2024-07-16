package ru.nsu.nlyapich;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.core.config.Configurator;
import ru.nsu.nlyapich.interpreter.BrainFuckInterpreter;

import java.io.IOException;

public class App {

    public static void main(String[] args ) throws IOException {
        String configLoggerFile = App.class.getClassLoader().getResource("log4j2.xml").getPath();
        Configurator.initialize(null, configLoggerFile);
        final Logger logger = LogManager.getLogger(App.class);

        switch (args.length) {
            case 0 -> {
                String codeFileName = App.class.getClassLoader().getResource("examples/english_alphabet.bf").getPath();
                String configFileName = App.class.getClassLoader().getResource("examples/commands.properties").getPath();

                BrainFuckInterpreter interpreter = new BrainFuckInterpreter(codeFileName, configFileName);
                interpreter.run(System.out, System.in);
            }
            case 1 -> {
                logger.error("Expected two file names: config and code");
            }
            case 2 -> {
                BrainFuckInterpreter interpreter = new BrainFuckInterpreter(args[0], args[1]);
                interpreter.run(System.out, System.in);
            }
            default -> {
                logger.error("Too many arguments");
            }
        }
    }

}
