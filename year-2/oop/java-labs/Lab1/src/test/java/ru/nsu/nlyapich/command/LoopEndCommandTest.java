package ru.nsu.nlyapich.command;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class LoopEndCommandTest {
    @Test
    public void testExecute() throws IOException {
        final int contextSize = 256;
        Map<Integer, Integer> blocks = new HashMap<>();
        blocks.put(0, 5);
        blocks.put(5, 0);

        ExecutionContext context = new ExecutionContext(contextSize, blocks);
        context.curPos = 5;
        context.getMemory().incrementData();

        Command commandLoop = new LoopEndCommand();

        commandLoop.execute(context, null , null);

        Assertions.assertEquals(0, context.curPos);

        commandLoop.execute(context, null, null);
        Assertions.assertEquals(5, context.curPos);

        context.getMemory().setCurrentData(0);
        commandLoop.execute(context, null, null);
        Assertions.assertEquals(6, context.curPos);
    }

    @Test
    public void testIsStartLoopCommand() {
        Command command = new LoopEndCommand();

        Assertions.assertFalse(command.isStartLoopCommand());
    }

    @Test
    public void testIsEndLoopCommand() {
        Command command = new LoopEndCommand();

        Assertions.assertTrue(command.isEndLoopCommand());
    }
}
