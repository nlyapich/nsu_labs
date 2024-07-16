package ru.nsu.nlyapich.command;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.IOException;

public class IncrementValueCommandTest {
    @Test
    public void testExecute() throws IOException {
        final int contextSize = 256;
        ExecutionContext context = new ExecutionContext(contextSize, null);

        Command command = new IncrementValueCommand();

        final int incrementValue = 5;
        for (int i = 0; i < incrementValue; ++i){
            command.execute(context, null, null);
        }

        Assertions.assertEquals(incrementValue, context.getMemory().getCurrentData());
    }

    @Test
    public void testIsStartLoopCommand() {
        Command command = new IncrementValueCommand();

        Assertions.assertFalse(command.isStartLoopCommand());
    }

    @Test
    public void testIsEndLoopCommand() {
        Command command = new IncrementValueCommand();

        Assertions.assertFalse(command.isEndLoopCommand());
    }
}
