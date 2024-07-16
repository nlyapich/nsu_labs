package ru.nsu.nlyapich.interpreter;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nsu.nlyapich.command.Command;
import ru.nsu.nlyapich.command.CommandFactory;
import ru.nsu.nlyapich.command.ExecutionContext;
import ru.nsu.nlyapich.util.ReaderLoopPosition;

import java.io.*;
import java.util.Map;
import java.util.Properties;

public class BrainFuckInterpreter {

    private static final Logger logger = LogManager.getLogger(BrainFuckInterpreter.class);
    private CommandFactory commandFactory;
    private ExecutionContext context;

    private String codeFileName;

    private final int contextSize = 30000;

    public BrainFuckInterpreter(String codeFileName, String configFileName) throws IOException {
        logger.info("Creating interpreter");
        this.codeFileName = codeFileName;
        try (FileInputStream fis = new FileInputStream(configFileName)){
            Properties prop = new Properties();
            prop.load(fis);

            commandFactory = new CommandFactory(prop);

            char startLoop = 0, endLoop = 0;

            for (Object c : prop.keySet()) {
                Character curChar = ((String) c).charAt(0);
                Command curCommand = commandFactory.getCommand(curChar);
                if (curCommand.isStartLoopCommand()) {
                    startLoop = curChar;
                }
                if (curCommand.isEndLoopCommand()) {
                    endLoop = curChar;
                }
            }

            try (FileInputStream codeStream = new FileInputStream(codeFileName)){
                Map<Integer, Integer> blocks = ReaderLoopPosition.read(codeStream, startLoop, endLoop);
                context = new ExecutionContext(contextSize, blocks);
            }

        } catch (IOException | ClassNotFoundException | InstantiationException | IllegalAccessException e) {
            logger.error("Failed to init interpreter: " + e.getMessage());
            throw new IOException("Failed to init interpreter: " + e.getMessage());
        }
    }

    public void run(OutputStream ostream, InputStream istream) throws IOException {
        logger.info("Start Interpreter");

        try (FileInputStream codeStream = new FileInputStream(codeFileName)){
            int codeLength = codeStream.available();
            byte[] code = new byte[codeLength];
            codeStream.read(code);

            int curPos = context.curPos;
            while (curPos < codeLength) {
                Command curCommand = commandFactory.getCommand((Character) (char) code[curPos]);
                if (curCommand == null) {
                    logger.error("An invalid character was found: " + (char) code[curPos] + "(ASCII - " + (int) code[curPos] + ")");
                    throw new IOException("An invalid character was found: " + (char) code[curPos] + "(ASCII - " + (int) code[curPos] + ")");
                }

                curCommand.execute(context, ostream, istream);

                curPos = context.curPos;
            }
        } catch (InstantiationException | IllegalAccessException | ClassNotFoundException e) {
            logger.error("Failed to run interpreter: " + e.getMessage());
            throw new IOException("Failed to run interpreter: " + e.getMessage());
        }

        logger.info("Finish Interpreter");
    }
}

