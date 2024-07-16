package ru.nsu.nlyapich.command;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class LoopStartCommandTest {
    @Test
    public void testExecute() throws IOException {
        final int contextSize = 256;
        Map<Integer, Integer> blocks = new HashMap<>();
        blocks.put(0, 5);
        blocks.put(5, 0);

        ExecutionContext context = new ExecutionContext(contextSize, blocks);

        Command commandLoop = new LoopStartCommand();

        commandLoop.execute(context, null , null);

        Assertions.assertEquals(5, context.curPos);

        context.getMemory().incrementData();

        commandLoop.execute(context, null, null);
        Assertions.assertEquals(6, context.curPos);
    }

    @Test
    public void testIsStartLoopCommand() {
        Command command = new LoopStartCommand();

        Assertions.assertTrue(command.isStartLoopCommand());
    }

    @Test
    public void testIsEndLoopCommand() {
        Command command = new LoopStartCommand();

        Assertions.assertFalse(command.isEndLoopCommand());
    }
}
